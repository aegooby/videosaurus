
#include "node.hpp"

namespace vs
{
namespace network
{
node::node(class context& context, zmq::socket_type type)
    : context(context), socket(context.context, type)
{ }
} // namespace network
} // namespace vs