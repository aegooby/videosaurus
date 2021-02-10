
#include <iostream>
#include <napi.h>
#include <zmq.hpp>

namespace example
{
namespace node = Napi;

int add(int x, int y);

node::Number __add(const node::CallbackInfo&);
node::Object init(node::Env, node::Object);

NODE_API_MODULE(addon, init)
} // namespace example