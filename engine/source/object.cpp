
#include "object.hpp"

namespace example
{
int add(int x, int y)
{
    return (x + y);
}
node::Number __add(const node::CallbackInfo& info)
{
    node::Env env = info.Env();
    // check if arguments are integer only.
    if (info.Length() < 2 || !info[0].IsNumber() || !info[1].IsNumber())
    {
        node::TypeError::New(env, "arg1::Number, arg2::Number expected")
            .ThrowAsJavaScriptException();
    }
    // convert javascripts datatype to c++
    node::Number first  = info[0].As<node::Number>();
    node::Number second = info[1].As<node::Number>();
    // run c++ function return value and return it in javascript
    node::Number returnValue =
        node::Number::New(env, add(first.Int32Value(), second.Int32Value()));

    return returnValue;
}
node::Object init(node::Env env, node::Object exports)
{
    // export Napi function
    exports.Set("add", node::Function::New(env, __add));
    return exports;
}
} // namespace example
