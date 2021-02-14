
#include "master.hpp"

#include "async.hpp"

#include <chrono>
#include <thread>

namespace vs
{
namespace nodejs
{
napi::Object master::init(napi::Env env, napi::Object exports)
{
    auto functions = { InstanceMethod("sleep", &master::sleep) };

    napi::Function func = DefineClass(env, "master", functions);

    napi::FunctionReference* constructor = new napi::FunctionReference();
    *constructor                         = napi::Persistent(func);
    env.SetInstanceData(constructor);

    exports.Set("master", func);
    return exports;
}

master::master(const napi::CallbackInfo& info)
    : napi::ObjectWrap<master>(info), node(context, zmq::socket_type::rep)
{ }

napi::Value master::sleep(const napi::CallbackInfo& info)
{
    const std::function<void()> __sleep = []()
    { std::this_thread::sleep_for(std::chrono::seconds(5)); };
    return async::async(info, __sleep);
}
} // namespace nodejs
} // namespace vs