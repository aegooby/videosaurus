
#pragma once
#include "__common.hpp"

#include <napi.h>

class master : public napi::ObjectWrap<master>
{
public:
    static napi::Object init(napi::Env env, napi::Object exports);
    master(const napi::CallbackInfo& info);

private:
    napi::Value value(const napi::CallbackInfo&);
    napi::Value plus_one(const napi::CallbackInfo&);
    napi::Value multiply(const napi::CallbackInfo&);

    double __value;
};