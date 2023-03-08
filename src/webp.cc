#include <napi.h>

#include <sstream>

using namespace Napi;

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  return exports;
}

NODE_API_MODULE(addon, Init)