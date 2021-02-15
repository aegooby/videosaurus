
#pragma once
#include "../__common.hpp"
#include "zeromq.hpp"

namespace vs
{
namespace network
{
class context
{
public:
    static constexpr int io_threads = 1;
    zmq::context_t       engine     = zmq::context_t(io_threads);

public:
};
} // namespace network
} // namespace vs