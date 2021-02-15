
#include "typeinfo.hpp"

#include <cxxabi.h>
#include <memory>

namespace vs
{
namespace util
{
std::string __demangle(const char* name)
{
    int  status = 0;
    auto __ptr  = abi::__cxa_demangle(name, nullptr, nullptr, &status);
    auto ptr    = std::unique_ptr<char>(__ptr);

    return !status ? ptr.get() : name;
}
} // namespace util
} // namespace vs