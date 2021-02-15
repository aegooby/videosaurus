
#pragma once
#include "../__common.hpp"

namespace vs
{
namespace util
{
template<typename type>
void print_arg(const type& arg)
{
    std::cout << arg;
}
template<>
void print_arg(const bool& arg)
{
    std::cout << termcolor::bold << termcolor::yellow
              << (arg ? "true" : "false") << termcolor::reset;
}
template<>
void print_arg(const std::string& arg)
{
    std::cout << termcolor::magenta << "\"" << arg << "\"" << termcolor::reset;
}
void print_args() { }
template<typename type, typename... types>
void print_args(type&& arg, types&&... args)
{
    print_arg(std::forward<type>(arg));
    if constexpr (sizeof...(types)) std::cout << ", ";
    print_args(std::forward<types>(args)...);
}
} // namespace util
} // namespace vs