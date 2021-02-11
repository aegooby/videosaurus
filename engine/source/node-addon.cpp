
#include "master.hpp"

#include <napi.h>

napi::Object __init__(napi::Env env, napi::Object exports)
{
    return master::init(env, exports);
}

NODE_API_MODULE(addon, __init__)