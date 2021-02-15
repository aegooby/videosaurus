
#pragma once
#include "../__common.hpp"

namespace vs
{
namespace util
{
std::string __demangle(const char*);
template<typename type>
std::string demangle()
{
    auto       result = __demangle(typeid(type).name());
    const auto end    = (result.find_first_of('(') != std::string::npos
                             ? result.find_first_of('(')
                             : result.length());
    result            = result.substr(0, end);
    const auto begin  = (result.find_last_of(':') != std::string::npos
                             ? result.find_last_of(':')
                             : 0);
    result            = result.substr(begin + 1, result.length());
    return result;
}
template<typename type>
std::string demangle(const type&)
{
    const auto result = __demangle(typeid(type).name());
    const auto begin  = (result.find_last_of(':') != std::string::npos
                             ? result.find_last_of(':')
                             : 0);
    const auto end    = (result.find_first_of('(') != std::string::npos
                             ? result.find_first_of('(')
                             : result.length() - 1);
    return result.substr(begin, end);
}
} // namespace util
} // namespace vs