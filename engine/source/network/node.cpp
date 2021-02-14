
#include "node.hpp"

namespace vs
{
namespace network
{
node::node(class context& context) : context(context) { }
void node::create(zmq::socket_type type)
{
    socket = zmq::socket_t(context.context, type);
}
} // namespace network
} // namespace vs