
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
    auto functions = { InstanceMethod("sleep", &master::sleep),
                       InstanceMethod("start", &master::start) };

    auto __class = DefineClass(env, "master", functions);

    auto* constructor = new napi::FunctionReference();
    *constructor      = napi::Persistent(__class);
    env.SetInstanceData(constructor);

    exports.Set("master", __class);
    return exports;
}

master::master(const napi::CallbackInfo& info)
    : napi::ObjectWrap<master>(info), node(context)
{ }

napi::Value master::start(const napi::CallbackInfo& info)
{
    if (!info[0].IsBoolean())
        throw std::runtime_error("master::master(): wrong socket flag type");
    if (!info[1].IsString())
        throw std::runtime_error("master::master(): wrong endpoint type");
    const bool        host     = info[0].As<napi::Boolean>();
    const std::string endpoint = info[1].As<napi::String>();

    const auto function = [this](bool host, const std::string& endpoint)
    {
        if (host)
        {
            node.create(zmq::socket_type::rep);
            node.socket.bind(endpoint.c_str());
        }
        else
        {
            node.create(zmq::socket_type::req);
            node.socket.connect(endpoint.c_str());
        }
    };

    return async::promise(info, function, host, endpoint);
}

napi::Value master::sleep(const napi::CallbackInfo& info)
{
    const auto function = []()
    { std::this_thread::sleep_for(std::chrono::seconds(5)); };
    return async::promise(info, function);
}
} // namespace nodejs
} // namespace vs