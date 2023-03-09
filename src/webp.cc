#include <napi.h>

#include <sstream>

#include "./libwebp/imageio/image_dec.h"
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

Napi::Buffer<unsigned char> ConvertToWebpSync(
    const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  int ok = 0;
  int keep_alpha = 1;

  Metadata metadata;
  WebPPicture picture;
  WebPConfig config;
  WebPMemoryWriter memory_writer;

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

    // set config options
  }

  if (!WebPValidateConfig(&config)) {
    NAPI_THROW_EMPTY_BUFFER(
        Napi::TypeError::New(env, "invalid webp configuration"));
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