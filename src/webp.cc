#include <napi.h>

#include <sstream>

#include "image_reader.h"
#include "webp/encode.h"

using namespace Napi;

#define NAPI_THROW_EMPTY_BUFFER(e) \
  NAPI_THROW(e, Napi::Buffer<unsigned char>::New(info.Env(), 0))

enum {
  METADATA_EXIF = (1 << 0),
  METADATA_ICC = (1 << 1),
  METADATA_XMP = (1 << 2),
  METADATA_ALL = METADATA_EXIF | METADATA_ICC | METADATA_XMP
};

static const char* const kErrorMessages[VP8_ENC_ERROR_LAST] = {
    "OK",
    "OUT_OF_MEMORY: Out of memory allocating objects",
    "BITSTREAM_OUT_OF_MEMORY: Out of memory re-allocating byte buffer",
    "NULL_PARAMETER: NULL parameter passed to function",
    "INVALID_CONFIGURATION: configuration is invalid",
    "BAD_DIMENSION: Bad picture dimension. Maximum width and height "
    "allowed is 16383 pixels.",
    "PARTITION0_OVERFLOW: Partition #0 is too big to fit 512k.\n"
    "To reduce the size of this partition, try using less segments "
    "with the -segments option, and eventually reduce the number of "
    "header bits using -partition_limit. More details are available "
    "in the manual (`man cwebp`)",
    "PARTITION_OVERFLOW: Partition is too big to fit 16M",
    "BAD_WRITE: Picture writer returned an I/O error",
    "FILE_TOO_BIG: File would be too big to fit in 4G",
    "USER_ABORT: encoding abort requested by user"};

int parseOptions(const Napi::Object& options, WebPConfig& config, Napi::Env env,
                 Napi::Error& error) {
  Napi::Value option_value;
  int lossless_preset = 6;
  int use_lossless_preset = -1;  // -1=unset, 0=don't use, 1=use it

  if (options.IsEmpty()) {
    return 0;
  }

  if (options.Has("quality")) {
    option_value = options.Get("quality");
    if (!option_value.IsNumber()) {
      error = Napi::TypeError::New(env, "Wrong type for option 'quality'.");
      return 1;
    }
    int num = option_value.As<Napi::Number>()
                  .Int32Value();  // @todo should this be a float?
    if (num < 0 || num > 100) {
      error = Napi::Error::New(
          env, "Value for option 'quality' must be between 0 and 100.");
      return 1;
    }

    config.quality = num;
    use_lossless_preset = 0;  // disable -z option
  }

  if (options.Has("preset")) {
    option_value = options.Get("preset");
    if (!option_value.IsString()) {
      error = Napi::TypeError::New(env, "Wrong type for option 'preset'.");
      return 1;
    }
    std::string presetName = option_value.As<Napi::String>();
    WebPPreset preset;

    if (presetName.compare("default") == 0) {
      preset = WEBP_PRESET_DEFAULT;
    } else if (presetName.compare("photo") == 0) {
      preset = WEBP_PRESET_PHOTO;
    } else if (presetName.compare("picture") == 0) {
      preset = WEBP_PRESET_PICTURE;
    } else if (presetName.compare("drawing") == 0) {
      preset = WEBP_PRESET_DRAWING;
    } else if (presetName.compare("icon") == 0) {
      preset = WEBP_PRESET_ICON;
    } else if (presetName.compare("text") == 0) {
      preset = WEBP_PRESET_TEXT;
    } else {
      error = Napi::Error::New(
          env, "Value for option 'preset' must be a valid preset.");
      return 1;
    }

    if (!WebPConfigPreset(&config, preset, config.quality)) {
      error = Napi::Error::New(
          env, "Could not initialize configuration with preset.");
      return 1;
    }
  }

  if (options.Has("method")) {
    option_value = options.Get("method");
    if (!option_value.IsNumber()) {
      error = Napi::TypeError::New(env, "Wrong type for option 'method'.");
      return 1;
    }
    int num = option_value.As<Napi::Number>().Int32Value();
    if (num < 0 || num > 6) {
      error = Napi::Error::New(
          env, "Value for option 'method' must be between 0 and 6.");
      return 1;
    }

    config.method = num;
    use_lossless_preset = 0;  // disable -z option
  }

  if (options.Has("losslessPreset")) {
    option_value = options.Get("losslessPreset");
    if (!option_value.IsNumber()) {
      error =
          Napi::TypeError::New(env, "Wrong type for option 'losslessPreset'.");
    }
    int num = option_value.As<Napi::Number>().Int32Value();
    if (num < 0 || num > 9) {
      error = Napi::Error::New(
          env, "Value for option 'losslessPreset' must be between 0 and 9.");
    }

    lossless_preset = num;
    if (use_lossless_preset != 0) use_lossless_preset = 1;
  }

  if (options.Has("hint")) {
    option_value = options.Get("hint");
    if (!option_value.IsString()) {
      error = Napi::TypeError::New(env, "Wrong type for option 'hint'");
      return 1;
    }
    std::string presetName = option_value.As<Napi::String>();

    if (presetName.compare("photo") == 0) {
      config.image_hint = WEBP_HINT_PHOTO;
    } else if (presetName.compare("picture") == 0) {
      config.image_hint = WEBP_HINT_PICTURE;
    } else if (presetName.compare("graph") == 0) {
      config.image_hint = WEBP_HINT_GRAPH;
    } else {
      error = Napi::Error::New(
          env,
          "Value for option 'hint' must be 'photo', 'picture', or 'graph'.");
      return 1;
    }
  }

  if (options.Has("sharpness")) {
    option_value = options.Get("sharpness");
    if (!option_value.IsNumber()) {
      error = Napi::TypeError::New(env, "Wrong type for option 'sharpness'");
      return 1;
    }
    int num = option_value.As<Napi::Number>().Int32Value();
    if (num < 0 || num > 7) {
      error = Napi::Error::New(
          env, "Value for option 'sharpness' must be between 0 and 7.");
      return 1;
    }

    config.filter_sharpness = num;
  }

  if (options.Has("sns")) {
    option_value = options.Get("sns");
    if (!option_value.IsNumber()) {
      error = Napi::TypeError::New(env, "Wrong type for option 'sns'");
      return 1;
    }
    int num = option_value.As<Napi::Number>().Int32Value();
    if (num < 0 || num > 100) {
      error = Napi::Error::New(
          env, "Value for option 'sns' must be between 0 and 100.");
      return 1;
    }

    config.sns_strength = num;
  }

  if (options.Has("autoFilter")) {
    option_value = options.Get("autoFilter");
    if (!option_value.IsBoolean()) {
      error = Napi::TypeError::New(env, "Wrong type for option 'autoFilter'");
      return 1;
    }
    if (option_value.As<Napi::Boolean>().Value()) {
      config.autofilter = 1;
    }
  }

  if (options.Has("lossless")) {
    option_value = options.Get("lossless");
    if (!option_value.IsBoolean()) {
      error = Napi::TypeError::New(env, "Wrong type for option 'lossless'");
      return 1;
    }
    if (option_value.As<Napi::Boolean>().Value()) {
      config.lossless = 1;
    }
  }

  if (options.Has("nearLossless")) {
    option_value = options.Get("nearLossless");
    if (!option_value.IsNumber()) {
      error = Napi::TypeError::New(env, "Wrong type for option 'nearLossless'");
      return 1;
    }
    int num = option_value.As<Napi::Number>().Int32Value();
    if (num < 0 || num > 100) {
      error = Napi::Error::New(
          env, "Value for option 'nearLossless' must be between 0 and 100.");
      return 1;
    }
    config.near_lossless = num;
    config.lossless = 1;  // use near-lossless only with lossless
  }

  if (options.Has("alphaQuality")) {
    option_value = options.Get("alphaQuality");
    if (!option_value.IsNumber()) {
      error = Napi::TypeError::New(env, "Wrong type for option 'alphaQuality'");
      return 1;
    }
    int num = option_value.As<Napi::Number>().Int32Value();
    if (num < 0 || num > 100) {
      error = Napi::Error::New(
          env, "Value for option 'alphaQuality' must be between 0 and 100.");
      return 1;
    }
    config.alpha_quality = num;
  }

  if (options.Has("alphaMethod")) {
    option_value = options.Get("alphaMethod");
    if (!option_value.IsNumber()) {
      error = Napi::TypeError::New(env, "Wrong type for option 'alphaMethod'");
      return 1;
    }
    int num = option_value.As<Napi::Number>().Int32Value();
    if (num < 0 || num > 1) {
      error = Napi::Error::New(
          env, "Value for option 'alphaMethod' must be between 0 and 1.");
      return 1;
    }
    config.alpha_compression = num;
  }

  if (options.Has("filter")) {
    option_value = options.Get("filter");
    if (!option_value.IsNumber()) {
      error = Napi::TypeError::New(env, "Wrong type for option 'filter'");
      return 1;
    }
    int num = option_value.As<Napi::Number>().Int32Value();
    if (num < 0 || num > 100) {
      error = Napi::Error::New(
          env, "Value for option 'filter' must be between 0 and 100.");
      return 1;
    }
    config.filter_strength = num;
  }

  if (options.Has("pass")) {
    option_value = options.Get("pass");
    if (!option_value.IsNumber()) {
      error = Napi::TypeError::New(env, "Wrong type for option 'pass'");
      return 1;
    }
    int num = option_value.As<Napi::Number>().Int32Value();
    if (num < 0 || num > 6) {
      error = Napi::Error::New(
          env, "Value for option 'pass' must be between 0 and 10.");
      return 1;
    }

    config.pass = num;
  }

  if (options.Has("size")) {
    option_value = options.Get("size");
    if (!option_value.IsNumber()) {
      error = Napi::TypeError::New(env, "Wrong type for option 'size'");
      return 1;
    }
    int num = option_value.As<Napi::Number>().Int32Value();
    if (num < 0) {
      error = Napi::Error::New(
          env, "Value for option 'size' must be larger than 0.");
      return 1;
    }
    config.target_size = num;
  }

  if (use_lossless_preset == 1) {
    if (!WebPConfigLosslessPreset(&config, lossless_preset)) {
      error = Napi::Error::New(env, "invalid losslessPreset");
      return 1;
    }
  }

  // If a target size was given, but somehow the pass option was
  // omitted, force a reasonable value.
  if (config.target_size > 0) {
    if (config.pass == 1) config.pass = 6;
  }

  return 0;
}

Napi::Buffer<unsigned char> ConvertToWebpSync(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Value option_value;
  Napi::Error errorValue;

  int error = 0;
  int ok = 0;
  int keep_alpha = 1;
  int keep_metadata = 0;

  Metadata metadata;
  WebPPicture picture;
  WebPConfig config;
  WebPMemoryWriter memory_writer;

  // @todo: currently we don't ever free these
  MetadataInit(&metadata);
  WebPMemoryWriterInit(&memory_writer);
  WebPPictureInit(&picture);
  WebPConfigInit(&config);

  if (info.Length() < 1 || !info[0].IsBuffer()) {
    NAPI_THROW_EMPTY_BUFFER(
        Napi::TypeError::New(env, "input must be a buffer"));
  }

  if (info.Length() >= 2) {
    if (!info[1].IsObject()) {
      NAPI_THROW_EMPTY_BUFFER(
          Napi::TypeError::New(env, "options must be an object"));
    }
    Napi::Object options = info[1].ToObject();

    if (options.Has("noAlpha")) {
      option_value = options.Get("noAlpha");
      if (!option_value.IsBoolean()) {
        NAPI_THROW_EMPTY_BUFFER(
            Napi::TypeError::New(env, "Wrong type for option 'noAlpha'."));
      }
      if (option_value.As<Napi::Boolean>().Value()) {
        keep_alpha = 0;
      }
    }

    if (options.Has("metadata")) {
      option_value = options.Get("metadata");
      if (option_value.IsString()) {
        const std::string metadataOption = option_value.As<Napi::String>();

        if (metadataOption.compare("none") == 0) {
          keep_metadata = 0;
        } else if (metadataOption.compare("all") == 0) {
          keep_metadata = METADATA_ALL;
        } else {
          NAPI_THROW_EMPTY_BUFFER(
              Napi::Error::New(env,
                               "Value for option 'metadata' must be 'none', "
                               "'all', or an array of metadata types."));
        }
      } else if (option_value.IsArray()) {
        const Napi::Array inputArray = option_value.As<Napi::Array>();
        const uint32_t length = inputArray.Length();
        for (uint32_t i = 0; i < length; ++i) {
          const Napi::Value val = inputArray[i];
          if (!val.IsString()) {
            NAPI_THROW_EMPTY_BUFFER(
                Napi::TypeError::New(env, "Wrong type for option 'metadata'."));
          }
          const std::string metadataOption = option_value.As<Napi::String>();
          if (metadataOption.compare("exif") == 0) {
            keep_metadata |= METADATA_EXIF;
          } else if (metadataOption.compare("icc") == 0) {
            keep_metadata |= METADATA_ICC;
          } else if (metadataOption.compare("xmp") == 0) {
            keep_metadata |= METADATA_XMP;
          } else {
            NAPI_THROW_EMPTY_BUFFER(
                Napi::Error::New(env,
                                 "Value for option 'metadata' must be 'none', "
                                 "'all', or an array of metadata types."));
          }
        }
      } else {
        NAPI_THROW_EMPTY_BUFFER(
            Napi::TypeError::New(env, "Wrong type for option 'metadata'."));
      }
    }

    error = parseOptions(options, config, env, errorValue);
    if (error) {
      NAPI_THROW_EMPTY_BUFFER(errorValue);
    }
  }

  if (!WebPValidateConfig(&config)) {
    NAPI_THROW_EMPTY_BUFFER(
        Napi::Error::New(env, "invalid webp configuration"));
  }

  Napi::Buffer<unsigned char> inputBuffer =
      info[0].As<Napi::Buffer<unsigned char>>();
  size_t inputBufferSize = inputBuffer.Length();
  const unsigned char* inputBufferData = inputBuffer.Data();

  // Read the input. We need to decide if we prefer ARGB or YUVA
  // samples, depending on the expected compression mode (this saves
  // some conversion steps).
  picture.use_argb = (config.lossless || config.use_sharp_yuv ||
                      config.preprocessing > 0);

  WebPImageReader reader =
      WebPGuessImageReader(inputBufferData, inputBufferSize);
  ok =
      reader(inputBufferData, inputBufferSize, &picture, keep_alpha, &metadata);
  if (!ok) {
    NAPI_THROW_EMPTY_BUFFER(
        Napi::Error::New(env, "could not read buffer as an image."));
  }

  picture.writer = WebPMemoryWrite;
  picture.custom_ptr = (void*)&memory_writer;

  if (!WebPEncode(&config, &picture)) {
    std::ostringstream errstr;
    errstr << "Cannot encode picture as WebP. Error code: "
           << picture.error_code << " (" << kErrorMessages[picture.error_code]
           << ")";
    NAPI_THROW_EMPTY_BUFFER(Napi::Error::New(env, errstr.str()));
  }

  return Napi::Buffer<unsigned char>::Copy(
      info.Env(), reinterpret_cast<unsigned char*>(memory_writer.mem),
      memory_writer.size);

  return Napi::Buffer<unsigned char>::New(info.Env(), 0);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "convertToWebpSync"),
              Napi::Function::New(env, ConvertToWebpSync));

  return exports;
}

NODE_API_MODULE(addon, Init)