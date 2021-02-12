
#include "node-interface.hpp"

#include "master.hpp"

napi::Object __init__(napi::Env env, napi::Object exports)
{
    return master::init(env, exports);
}

NODE_API_MODULE(videosaurus, __init__)
