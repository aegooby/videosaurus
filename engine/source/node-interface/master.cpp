
#include "master.hpp"

#include <chrono>
#include <thread>

napi::Object master::init(napi::Env env, napi::Object exports)
{
    napi::Function func =
        DefineClass(env, "master",
                    { InstanceMethod("plus_one", &master::plus_one),
                      InstanceMethod("value", &master::value),
                      InstanceMethod("multiply", &master::multiply) });

    napi::FunctionReference* constructor = new napi::FunctionReference();
    *constructor                         = napi::Persistent(func);
    env.SetInstanceData(constructor);

    exports.Set("master", func);
    return exports;
}

master::master(const napi::CallbackInfo& info) : napi::ObjectWrap<master>(info)
{
    napi::Env env = info.Env();

    int length = info.Length();

    if (length <= 0 || !info[0].IsNumber())
    {
        napi::TypeError::New(env, "").ThrowAsJavaScriptException();
        return;
    }

    napi::Number value = info[0].As<napi::Number>();
    this->__value      = value.DoubleValue();
}

napi::Value master::value(const napi::CallbackInfo& info)
{
    /** @todo Async doesn't work. */
    const auto __sleep = []() -> void
    { std::this_thread::sleep_for(std::chrono::seconds(10)); };
    auto thread = std::thread(__sleep);
    thread.detach();
    double num = this->__value;

    return napi::Number::New(info.Env(), num);
}

napi::Value master::plus_one(const napi::CallbackInfo& info)
{
    this->__value = this->__value + 1;

    return master::value(info);
}

napi::Value master::multiply(const napi::CallbackInfo& info)
{
    napi::Number multiple;
    if (info.Length() <= 0 || !info[0].IsNumber())
        multiple = napi::Number::New(info.Env(), 1);
    else
        multiple = info[0].As<napi::Number>();

    napi::Object obj =
        info.Env().GetInstanceData<napi::FunctionReference>()->New(
            { napi::Number::New(info.Env(),
                                this->__value * multiple.DoubleValue()) });

    return obj;
}