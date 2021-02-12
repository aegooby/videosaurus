
#pragma once

#if defined(VS_DEBUG)
#    define _DEBUG
#    undef NDEBUG
#else
#    define NDEBUG
#    undef _DEBUG
#endif

#if defined(__cplusplus)
#    if __cplusplus <= 199711L
#        define VS_CPP_VER 98
#    elif __cplusplus <= 201103L
#        define VS_CPP_VER 11
#    elif __cplusplus <= 201402L
#        define VS_CPP_VER 14
#    elif __cplusplus <= 201703L
#        define VS_CPP_VER 17
#    elif __cplusplus <= 202002L
#        define VS_CPP_VER 20
#    else
#        error Unknown C++ version
#    endif
#else
#    error Not C++
#endif

#if !defined(__x86_64__)
#    error Not x86
#endif

#if defined(__clang__)
#    define VS_COMPILER_CLANG
#elif defined(_MSC_VER)
#    define VS_COMPILER_MSVC
#elif defined(__INTEL_COMPILER)
#    define VS_COMPILER_INTEL
#elif defined(__GNUC__)
#    define VS_COMPILER_GCC
#else
#    define VS_COMPILER_UNKNOWN
#endif

#if defined(_WIN32) || defined(_WIN64)
#    define VS_OS_WINDOWS
#elif defined(__APPLE__) && defined(__MACH__)
#    define VS_OS_MACOS
#elif defined(__linux__)
#    define VS_OS_LINUX
#else
#    define VS_OS_UNKNOWN
#endif

#if defined(__unix__) || defined(P201_OS_MACOS)
#    include <unistd.h>
#endif

#define VS_EVAL_DISCARD(statement) ((void)statement)

#include <cstddef>
#include <cstdint>

namespace p201
{
static constexpr const char* name    = "videosaurus";
static constexpr const char* version = "1.0.0";
#if defined(VS_DEBUG)
static constexpr bool __debug__ = true;
#else
static constexpr bool __debug__ = false;
#endif
} // namespace p201
