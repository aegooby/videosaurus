
#pragma once
#include "../__common.hpp"
#include "zeromq.hpp"

namespace vs
{
class newtork;
class node
{
private:
    class network& network;

public:
    zmq::socket_t socket;

public:
    node(class network&);
};
} // namespace vs