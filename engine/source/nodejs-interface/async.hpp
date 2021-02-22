
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
static constexpr const char* header = "@libvs-async";
} // namespace debug

template<typename... conditions>
struct type_and : std::true_type
{ };
template<typename condition, typename... conditions>
struct type_and<condition, conditions...>
    : std::conditional_t<condition::value, type_and<conditions...>,
                         std::false_type>
{ };

template<typename... conditions>
struct type_or : std::false_type
{ };
template<typename condition, typename... conditions>
struct type_or<condition, conditions...>
    : std::conditional_t<condition::value, std::true_type,
                         type_or<conditions...>>
{ };

template<typename... types>
using has_reference = type_or<std::is_reference<types>...>;
template<typename... types>
inline constexpr bool has_reference_v = has_reference<types...>::value;

template<typename function_type, typename... types>
napi::Promise promise(const napi::CallbackInfo& info,
                      const function_type&      function, types... args)
{
    /** @note Since the lambda is called asynchronously, any references     */
    /**       passed in at the time of creation may be destroyed before     */
    /**       the execution point of the lambda.                            */
    static_assert(!has_reference_v<types...>);

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