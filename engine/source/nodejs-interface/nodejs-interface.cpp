
#include "nodejs-interface.hpp"

#include "master.hpp"

namespace nodejs
{
nodejs::napi::Object __init__(nodejs::napi::Env    env,
                              nodejs::napi::Object exports)
{
    return nodejs::master::init(env, exports);
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, __init__)
} // namespace nodejs