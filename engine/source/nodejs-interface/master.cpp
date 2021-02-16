
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
    auto functions = { InstanceMethod("createNode", &master::create_node),
                       InstanceMethod("sendMessage", &master::send_message),
                       InstanceMethod("receiveMessage",
                                      &master::receive_message) };

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
napi::Value master::create_node(const napi::CallbackInfo& info)
{
    if (!info[0].IsBoolean())
        throw std::runtime_error("start(): wrong socket flag type");
    if (!info[1].IsString())
        throw std::runtime_error("start(): wrong endpoint type");
    const bool        host     = info[0].As<napi::Boolean>();
    const std::string endpoint = info[1].As<napi::String>();

    const auto function = [this](bool host, std::string endpoint) -> void
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
napi::Value master::send_message(const napi::CallbackInfo& info)
{
    if (!info[0].IsString())
        throw std::runtime_error("send_message(): wrong message type");
    const std::string message = info[0].As<napi::String>();

    const auto function = [this](std::string string) -> void
    {
        const auto bytes = string.size() * sizeof(char);
        assert(bytes <= 100);

        auto message = zmq::message_t(100);
        std::memcpy(message.data(), string.data(), bytes);

        if (!node.socket.send(message, zmq::send_flags::none))
            throw std::runtime_error("send_message(): failed");
    };
    return async::promise(info, function, message);
}
napi::Value master::receive_message(const napi::CallbackInfo& info)
{
    const auto function = [this]() -> std::string
    {
        auto message = zmq::message_t(100);
        if (!node.socket.recv(message, zmq::recv_flags::none))
            throw std::runtime_error("receive_message(): failed");

        auto       string = std::string(100, '\0');
        const auto bytes  = string.size() * sizeof(char);
        assert(bytes <= 100);

        std::memcpy(string.data(), message.data(), bytes);
        return string;
    };
    return async::promise(info, function);
}
} // namespace nodejs
} // namespace vs