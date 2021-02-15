
#pragma once
#include "../__common.hpp"
#include "../util.hpp"
#include "nodejs-interface.hpp"

#include <functional>
#include <iostream>
#include <sstream>
#include <type_traits>
#include <utility>

namespace vs
{
namespace nodejs
{
namespace async
{
template<typename function_type>
class worker_base : public napi::AsyncWorker
{
public:
    using __base = napi::AsyncWorker;

protected:
    napi::Promise::Deferred deferred;
    function_type           function;

public:
    worker_base(const napi::Env& env, const function_type& function)
        : __base(env), deferred(env), function(function)
    { }
    void queue()
    {
        __base::Queue();
    }
    napi::Promise promise()
    {
        return deferred.Promise();
    }

    void OnError(const napi::Error& error)
    {
        deferred.Reject(error.Value());
    }
};
template<typename function_type,
         typename result_type = std::result_of_t<function_type()>>
class worker : public worker_base<function_type>
{
public:
    using __base = worker_base<function_type>;

protected:
    result_type result;

public:
    worker(const napi::Env& env, const function_type& function)
        : __base(env, function)
    { }

    void Execute()
    {
        result = __base::function();
    }
    void OnOK()
    {
        __base::deferred.Resolve(napi::Value::From(this->Env(), result));
    }
};
template<typename function_type>
class worker<function_type, void> : public worker_base<function_type>
{
public:
    using __base = worker_base<function_type>;

protected:
public:
    worker(const napi::Env& env, const function_type& function)
        : __base(env, function)
    { }

    void Execute()
    {
        __base::function();
    }
    void OnOK()
    {
        __base::deferred.Resolve(this->Env().Undefined());
    }
};

namespace debug
{
static constexpr const char* header = "@native-async";
} // namespace debug

template<typename function_type, typename... types>
napi::Promise promise(const napi::CallbackInfo& info,
                      const function_type&      function, types&&... args)
{
    if constexpr (__debug__)
    {
        std::cout << termcolor::bold << debug::header << termcolor::reset
                  << " : " << termcolor::bold << termcolor::blue
                  << util::demangle<function_type>() << termcolor::reset << "(";
        util::print_args(args...);
        std::cout << ")" << std::endl;
    }
    const auto bound    = std::bind(function, std::forward<types>(args)...);
    auto*      __worker = new worker(info.Env(), bound);
    __worker->queue();
    return __worker->promise();
}
} // namespace async
} // namespace nodejs
} // namespace vs