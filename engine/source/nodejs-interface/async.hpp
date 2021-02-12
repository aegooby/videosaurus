
#pragma once
#include "../__common.hpp"
#include "nodejs-interface.hpp"

#include <functional>
#include <iostream>
#include <utility>

namespace nodejs
{
namespace async
{
template<typename result_type>
class worker : public napi::AsyncWorker
{
public:
    using __base = napi::AsyncWorker;

protected:
    napi::Promise::Deferred      deferred;
    std::function<result_type()> function;
    result_type                  result;

public:
    worker(const napi::Env& env) : __base(env), deferred(env) { }
    ~worker() = default;

    template<typename... types>
    void bind(const std::function<result_type(types...)>& function,
              types&&... args)
    {
        this->function = std::bind(function, std::forward<types>(args)...);
    }
    void queue()
    {
        __base::Queue();
    }

    void Execute()
    {
        result = function();
    }
    void OnOK()
    {
        deferred.Resolve(napi::Value::From(Env(), result));
    }
    void OnError(const napi::Error& error)
    {
        deferred.Reject(error.Value());
    }

    napi::Promise promise()
    {
        return deferred.Promise();
    }
};
template<>
class worker<void> : public napi::AsyncWorker
{
public:
    using __base = napi::AsyncWorker;

protected:
    napi::Promise::Deferred deferred;
    std::function<void()>   function;

public:
    worker(const napi::Env& env) : __base(env), deferred(env) { }
    ~worker() = default;

    template<typename... types>
    void bind(const std::function<void(types...)>& function, types&&... args)
    {
        this->function = std::bind(function, std::forward<types>(args)...);
    }
    void queue()
    {
        __base::Queue();
    }

    void Execute()
    {
        function();
    }
    void OnOK()
    {
        deferred.Resolve(Env().Undefined());
    }
    void OnError(const napi::Error& error)
    {
        deferred.Reject(error.Value());
    }

    napi::Promise promise()
    {
        return deferred.Promise();
    }
};

template<typename result_type, typename... types>
napi::Promise async(const napi::CallbackInfo&            info,
                    std::function<result_type(types...)> function,
                    types&&... args)
{
    auto* __worker = new worker<result_type>(info.Env());
    __worker->bind(function, std::forward<types>(args)...);
    __worker->queue();

    return __worker->promise();
}
} // namespace async
} // namespace nodejs