#include <napi.h>

#include <sstream>

#include "image_reader.h"
#include "webp/encode.h"

using namespace Napi;

#define NAPI_THROW_EMPTY_BUFFER(e) \
  NAPI_THROW(e, Napi::Buffer<unsigned char>::New(info.Env(), 0))

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
  "USER_ABORT: encoding abort requested by user"
};

int parseOptions(const Napi::Object& options, WebPConfig& config) {
  Napi::Value option_value;

  if (options.IsEmpty()) {
    return 0;
  }

  if (options.Has("sharpness")) {
    option_value = options.Get("sharpness");
    if (!option_value.IsNumber()) {
      return 2;
    }
    int num = option_value.As<Napi::Number>().Int32Value();
    if (num < 0 || num > 7) {
      return 3;
    }

    config.filter_sharpness = num;
  }

  if (options.Has("sns")) {
    option_value = options.Get("sns");
    if (!option_value.IsNumber()) {
      return 4;
    }
    int num = option_value.As<Napi::Number>().Int32Value();
    if (num < 0 || num > 100) {
      return 5;
    }

    config.sns_strength = num;
  }

  if (options.Has("autoFilter")) {
    option_value = options.Get("autoFilter");
    if (!option_value.IsBoolean()) {
      return 6;
    }
    if (option_value.As<Napi::Boolean>().Value()) {
      config.autofilter = 1;
    }
  }

  if (options.Has("alphaQuality")) {
    option_value = options.Get("alphaQuality");
    if (!option_value.IsNumber()) {
      return 7;
    }
    int num = option_value.As<Napi::Number>().Int32Value();
    if (num < 0 || num > 100) {
      return 8;
    }
    config.alpha_quality = num;
  }

  if (options.Has("alphaMethod")) {
    option_value = options.Get("alphaMethod");
    if (!option_value.IsNumber()) {
      return 9;
    }
    int num = option_value.As<Napi::Number>().Int32Value();
    if (num < 0 || num > 1) {
      return 10;
    }
    config.alpha_compression = num;
  }

  if (options.Has("filter")) {
    option_value = options.Get("filter");
    if (!option_value.IsNumber()) {
      return 11;
    }
    int num = option_value.As<Napi::Number>().Int32Value();
    if (num < 0 || num > 100) {
      return 12;
    }
    config.filter_strength = num;
  }

  return 0;
}

const char* parse_option_error_text(const unsigned code) {
  switch (code) {
    case 0:
      return "no error, everything went ok";
    // 1 is reserved for future use
    case 2:
      return "Wrong type for option 'sharpness'.";
    case 3:
      return "Value for option 'sharpness' must be between 0 and 7.";
    case 4:
      return "Wrong type for option 'sns'.";
    case 5:
      return "Value for option 'sns' must be between 0 and 100.";
    case 6:
      return "Wrong type for option 'autoFilter'.";
    case 7:
      return "Wrong type for option 'alphaQuality'.";
    case 8:
      return "Value for option 'alphaQuality' must be between 0 and 100.";
    case 9:
      return "Wrong type for option 'alphaMethod'.";
    case 10:
      return "Value for option 'alphaMethod' must be between 0 and 1.";
    case 11:
      return "Wrong type for option 'filter'.";
    case 12:
      return "Value for option 'filter' must be between 0 and 100.";
  }
  return "unknown error code";
}

Napi::Buffer<unsigned char> ConvertToWebpSync(
    const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Value option_value;

  int error = 0;
  int ok = 0;
  int keep_alpha = 1;
  int use_lossless_preset = -1;  // -1=unset, 0=don't use, 1=use it

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

    if (options.Has("quality")) {
      option_value = options.Get("quality");
      if (!option_value.IsNumber()) {
        NAPI_THROW_EMPTY_BUFFER(
          Napi::TypeError::New(env, "Wrong type for option 'quality'."));
      }
      int num = option_value.As<Napi::Number>().Int32Value(); // @todo should this be a float?
      if (num < 0 || num > 100) {
        Napi::Error::New(env, "Value for option 'quality' must be between 0 and 100.");
      }

      config.quality = num;
      use_lossless_preset = 0;   // disable -z option
    }

    if (options.Has("preset")) {
      option_value = options.Get("preset");
      if (!option_value.IsNumber()) {
        NAPI_THROW_EMPTY_BUFFER(
          Napi::TypeError::New(env, "Wrong type for option 'preset'."));
      }
      int presetNum = option_value.As<Napi::Number>().Int32Value();
      if (presetNum < 1 || presetNum > 6) {
        Napi::Error::New(env, "Value for option 'preset' must be between 1 and 6.");
      }

      WebPPreset preset;

      switch (presetNum) {
        case 1:
          preset = WEBP_PRESET_DEFAULT;
          break;
        case 2:
          preset = WEBP_PRESET_PHOTO;
          break;
        case 3:
          preset = WEBP_PRESET_PICTURE;
          break;
        case 4:
          preset = WEBP_PRESET_DRAWING;
          break;
        case 5:
          preset = WEBP_PRESET_ICON;
          break;
        case 6:
          preset = WEBP_PRESET_TEXT;
          break;
      }
      if (!WebPConfigPreset(&config, preset, config.quality)) {
        Napi::Error::New(env, "Could not initialize configuration with preset.");
      }
    }

    if (options.Has("method")) {
      option_value = options.Get("method");
      if (!option_value.IsNumber()) {
        NAPI_THROW_EMPTY_BUFFER(
          Napi::TypeError::New(env, "Wrong type for option 'method'."));
      }
      int num = option_value.As<Napi::Number>().Int32Value();
      if (num < 0 || num > 6) {
        Napi::Error::New(env, "Value for option 'method' must be between 0 and 6.");
      }

      config.method = num;
      use_lossless_preset = 0;   // disable -z option
    }

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

    error = parseOptions(options, config);
    if (error) {
      NAPI_THROW_EMPTY_BUFFER(
          Napi::TypeError::New(env, parse_option_error_text(error)));
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

  WebPImageReader reader = WebPGuessImageReader(inputBufferData, inputBufferSize);
  ok = reader(inputBufferData, inputBufferSize, &picture, keep_alpha, &metadata);
  if (!ok) {
    NAPI_THROW_EMPTY_BUFFER(
        Napi::Error::New(env, "could not read buffer as an image."));
  }

  picture.writer = WebPMemoryWrite;
  picture.custom_ptr = (void*)&memory_writer;

  // crop - WebPPictureView
  // resize

  if (!WebPEncode(&config, &picture)) {
    std::ostringstream errstr;
      errstr << "Cannot encode picture as WebP. Error code: " << picture.error_code << " (" << kErrorMessages[picture.error_code] << ")" ;
    NAPI_THROW_EMPTY_BUFFER(
        Napi::Error::New(env, errstr.str()));
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