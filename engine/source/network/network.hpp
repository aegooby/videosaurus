
#pragma once
#include "../__common.hpp"
#include "zeromq.hpp"

namespace vs
{
class network
{
public:
    static constexpr int io_threads = 1;
    zmq::context_t       context    = zmq::context_t(io_threads);

public:
};
} // namespace vs