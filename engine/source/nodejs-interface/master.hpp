
#pragma once
#include "../__common.hpp"
#include "../network.hpp"
#include "nodejs-interface.hpp"

namespace vs
{
namespace nodejs
{
class master : public napi::ObjectWrap<master>
{
public:
    static napi::Object init(napi::Env env, napi::Object exports);
    master(const napi::CallbackInfo& info);

private:
    napi::Value create_node(const napi::CallbackInfo&);
    napi::Value send_message(const napi::CallbackInfo&);
    napi::Value receive_message(const napi::CallbackInfo&);

    network::context context;
    network::node    node;
};
} // namespace nodejs
} // namespace vs