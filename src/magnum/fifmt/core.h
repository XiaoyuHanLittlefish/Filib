#pragma once

#include <corecrt.h>
#include <cstring>
#include <string>
#include <type_traits>

#define FIFMT_MAJOR_VERSION 1
#define FIFMT_MINOR_VERSION 0
#define FIFMT_PATCH_VERSION 0

#define FIFMT_VERSION                                                          \
  FIFMT_MAJOR_VERSION * 10000 + FIFMT_MINOR_VERSION * 100 + FIFMT_PATCH_VERSION

#ifdef _MSVC_LANG
#define FIFMT_CPLUSPLUS _MSVC_LANG
#else
#define FIFMT_CPLUSPLUS __cplusplus
#endif

#ifdef __has_feature
#define FIFMT_HAS_FEATURE(x) __has_feature(x)
#else
#define FIFMT_HAS_FEATURE(x) 0
#endif

#ifdef __has_cpp_attribute
#define FIFMT_HAS_CPP_ATTRIBUTE(x) __has_cpp_attribute(x)
#else
#define FIFMT_HAS_CPP_ATTRIBUTE(x) 0
#endif

#if defined(__clang__) && !defined(__ibmxl__)
#define FIFMT_CLANG_VERSION (__clang_major__ * 100 + __clang_minor__)
#else
#define FIFMT_CLANG_VERSION 0
#endif

#if defined(__GNUC__) && !defined(__clang__) && !defined(__INTEL_COMPILER) &&  \
    !defined(__NVCOMPILER)
#define FIFMT_GCC_VERSION (__GNUC__ * 100 + __GNUC_MINOR__)
#else
#define FIFMT_GCC_VERSION 0
#endif

#ifdef _MSC_VER
#define FIFMT_MSC_VERSION _MSC_VER
#define FIFMT_MSC_WARNING(...) __pragma(warning(__VA_ARGS__))
#else
#define FIFMT_MSC_VERSION 0
#define FIFMT_MSC_WARNING(...)
#endif

#ifndef FIFMT_UNICODE
#define FIFMT_UNICODE !FIFMT_MSC_VERSION
#endif

#ifndef FIFMT_USE_CONSTEXPR
#if (FIFMT_HAS_FEATURE(cxx_relaxed_constexpr) || FIFMT_MSC_VERSION >= 1912 ||  \
     (FIFMT_GCC_VERSION >= 600 && FIFMT_CPLUSPLUS >= 201402L)) &&              \
    !FMT_ICC_VERSION && !defined(__NVCC__)
#define FIFMT_USE_CONSTEXPR 1
#else
#define FIFMT_USE_CONSTEXPR 0
#endif
#endif

#if FIFMT_USE_CONSTEXPR
#define FIFMT_CONSTEXPR constexpr
#else
#define FIFMT_CONSTEXPR
#endif

// Check if exceptions are disabled.
#ifndef FIFMT_EXCEPTIONS
#if (defined(__GNUC__) && !defined(__EXCEPTIONS)) ||                           \
    (FIFMT_MSC_VERSION && !_HAS_EXCEPTIONS)
#define FIFMT_EXCEPTIONS 0
#else
#define FIFMT_EXCEPTIONS 1
#endif
#endif

// Disable [[noreturn]] on MSVC/NVCC because of bogus unreachable code warnings.
#if FIFMT_EXCEPTIONS && FIFMT_HAS_CPP_ATTRIBUTE(noreturn) &&                   \
    !FIFMT_MSC_VERSION && !defined(__NVCC__)
#define FIFMT_NORETURN [[noreturn]]
#else
#define FIFMT_NORETURN
#endif

#ifndef FIFMT_INLINE
#if FIFMT_GCC_VERSION || FIFMT_CLANG_VERSION
#define FIFMT_INLINE inline __attribute__((always_inline))
#else
#define FIFMT_INLINE inline
#endif
#endif

#ifndef FIFMT_API
#define FIFMT_API
#endif

namespace fifmt {

namespace detail {
FIFMT_NORETURN FIFMT_API void assert_fail(const char *file, int line,
                                          const char *message);

#ifndef FIFMT_ASSERT
#ifdef NDEBUG
// FIFMT_ASSERT is not empty to avoid -Wempty-body.
#define FIFMT_ASSERT(condition, message)                                       \
  fmt::detail::ignore_unused((condition), (message))
#else
#define FIFMT_ASSERT(condition, message)                                       \
  ((condition) /* void() fails with -Winvalid-constexpr on clang 4.0.1 */      \
       ? (void)0                                                               \
       : fifmt::detail::assert_fail(__FILE__, __LINE__, (message)))
#endif
#endif

FIFMT_CONSTEXPR inline auto is_utf8() -> bool {
  FIFMT_MSC_WARNING(suppress : 4566)
  constexpr unsigned char section[] = "\u00A7";

  using uchar = unsigned char;
  return FIFMT_UNICODE || (sizeof(section) == 3 && uchar(section[0]) == 0xC2 &&
                           uchar(section[1]) == 0xA7);
}

template <typename Int>
FIFMT_CONSTEXPR auto to_unsigned(Int value) ->
    typename std::make_unsigned<Int>::type {
  //  非法转换时RE
  FIFMT_ASSERT(std::is_unsigned<Int>::value || value >= 0, "negative value");

  return static_cast<typename std::make_unsigned<Int>::type>(value);
}

template <typename T> constexpr FIFMT_INLINE auto const_check(T value) -> T {
  return value;
}

template <typename... T> FIFMT_CONSTEXPR void ignore_unused(const T &...) {}

constexpr FIFMT_INLINE auto
is_constant_evaluated(bool default_value = false) noexcept -> bool {
// Workaround for incompatibility between libstdc++ consteval-based
// std::is_constant_evaluated() implementation and clang-14.
// https://github.com/fmtlib/fmt/issues/3247
#if FIFMT_CPLUSPLUS >= 202002L && defined(_GLIBCXX_RELEASE) &&                 \
    _GLIBCXX_RELEASE >= 12 &&                                                  \
    (FIFMT_CLANG_VERSION >= 1400 && FIFMT_CLANG_VERSION < 1500)
  ignore_unused(default_value);
  return __builtin_is_constant_evaluated();
#elif defined(__cpp_lib_is_constant_evaluated)
  ignore_unused(default_value);
  return std::is_constant_evaluated();
#else
  return default_value;
#endif
}
} // namespace detail

template <typename Char> class basic_string_view {
public:
  using value_type = Char;
  using iterator = const Char *;

  constexpr basic_string_view() noexcept : data(nullptr), size(0) {}

  constexpr basic_string_view(const Char *_data, size_t _size) noexcept
      : data(_data), size(_size) {}

  FIFMT_INLINE basic_string_view(const Char *_data)
      : data(_data),
        size(detail::const_check(std::is_same<Char, char>::value &&
                                 !detail::is_constant_evaluated(true))
                 ? std::strlen(reinterpret_cast<const char *>(_data))
                 : std::char_traits<Char>::length(_data)) {}

private:
  const Char *data;
  size_t size;
};

} // namespace fifmt