
#pragma once
#include "../__common.hpp"
#include "context.hpp"
#include "zeromq.hpp"

namespace vs
{
namespace network
{
class node
{
private:
    class context& context;

public:
    zmq::socket_t socket;

public:
    node(class context&);
    void create(zmq::socket_type);
};
} // namespace network
} // namespace vs