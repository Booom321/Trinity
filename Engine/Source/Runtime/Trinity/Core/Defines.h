#pragma once

#if defined(_DEBUG)
	#define TRNT_DEBUG 1
#endif

#if !defined(TRNT_NODISCARD) && defined(__has_cpp_attribute)
	#if __has_cpp_attribute(nodiscard)
		#define TRNT_NODISCARD [[nodiscard]]
	#else
		#define TRNT_NODISCARD
	#endif
#endif

#if !defined(TRNT_NORETURN) && defined(__has_cpp_attribute)
	#if __has_cpp_attribute(noreturn)
		#define TRNT_NORETURN [[noreturn]]
	#else
		#define TRNT_NORETURN
	#endif
#endif

#if !defined(TRNT_DEPRECATED) && defined(__has_cpp_attribute)
	#if __has_cpp_attribute(deprecated)
		#define TRNT_DEPRECATED [[deprecated]]
	#else
		#define TRNT_DEPRECATED
	#endif
#endif

#if !defined(TRNT_MAYBE_UNUSED) && defined(__has_cpp_attribute)
	#if __has_cpp_attribute(maybe_unused)
		#define TRNT_MAYBE_UNUSED [[maybe_unused]]
	#else
		#define TRNT_MAYBE_UNUSED
	#endif
#endif

#if __cpp_constexpr
	#define TRNT_CONSTEXPR constexpr
#else
	#define TRNT_CONSTEXPR
#endif

#if defined(__cpp_consteval)
	#define TRNT_CONSTEVAL consteval
#else
	#define TRNT_CONSTEVAL TRNT_CONSTEXPR
#endif

#if defined(__clang__) || defined(__gcc__)
	#define TRNT_DLL_EXPORT __attribute__((visibility("default")))
	#define TRNT_DLL_IMPORT
	#define TRNT_INLINE		  __attribute__((always_inline)) inline
	#define TRNT_FORCE_INLINE __attribute__((always_inline)) inline
	#define TRNT_NOINLINE	  __attribute__((noinline))
	#define TRNT_RESTRICT	  __restrict__
#elif defined(_MSC_VER)
	#define TRNT_DLL_EXPORT	  __declspec(dllexport)
	#define TRNT_DLL_IMPORT	  __declspec(dllimport)
	#define TRNT_INLINE		  __inline
	#define TRNT_FORCE_INLINE __forceinline
	#define TRNT_NOINLINE	  __declspec(noinline)
	#define TRNT_RESTRICT	  __restrict
#else
	#define TRNT_DLL_EXPORT __attribute__((visibility("default")))
	#define TRNT_DLL_IMPORT
	#define TRNT_INLINE		  inline
	#define TRNT_FORCE_INLINE inline
	#define TRNT_NOINLINE
	#define TRNT_RESTRICT restrict
#endif

#ifdef TRNT_DYNAMIC_LINK_LIBRARY
	#ifdef TRNT_EXPORT
		#define TRNT_API TRNT_DLL_EXPORT
	#else
		#define TRNT_API TRNT_DLL_IMPORT
	#endif
#else
	#define TRNT_API
#endif

#ifndef TRNT_DISALLOW_COPY_AND_ASSIGN
	#define TRNT_DISALLOW_COPY_AND_ASSIGN(ClassName)    \
		ClassName(const ClassName&) = delete;           \
		ClassName& operator=(const ClassName&) = delete
#endif

#ifndef TRNT_NON_MOVABLE
	#define TRNT_NON_MOVABLE(ClassName)            \
		ClassName(ClassName&&) = delete;           \
		ClassName& operator=(ClassName&&) = delete
#endif

#ifndef TRNT_MIN
	#define TRNT_MIN(X, Y) ((X > Y) ? Y : X)
#endif

#ifndef TRNT_MAX
	#define TRNT_MAX(X, Y) ((X > Y) ? X : Y)
#endif

#ifndef TRNT_STRINGIFY
	#define TRNT_STRINGIFY(X) #X
#endif

#ifndef TRNT_CONCATENATE
	#define TRNT_CONCATENATE(X, Y) X##Y
#endif

#ifndef TRNT_GET_ARRAY_LENGTH
	#define TRNT_GET_ARRAY_LENGTH(Array) (sizeof(Array) / sizeof(Array[0]))
#endif

#ifndef TRNT_CHECK_IS_NULL
	#define TRNT_CHECK_IS_NULL(Pointer) (Pointer == nullptr)
#endif

#ifndef TRNT_CHECK_IS_NOT_NULL
	#define TRNT_CHECK_IS_NOT_NULL(Pointer) (Pointer != nullptr)
#endif

#ifndef TRNT_BIT
	#define TRNT_BIT(n) (1ull << (n))
#endif

#ifdef _MSC_VER
	#define TRNT_PRAGMA_WARNING_PUSH			__pragma(warning(push))
	#define TRNT_PRAGMA_WARNING_POP				__pragma(warning(pop))
	#define TRNT_DISABLE_WARNING(WarningNumber) __pragma(warning(disable : WarningNumber))
#else
	#define TRNT_DO_PRAGMA(X)					_Pragma(#X)
	#define TRNT_PRAGMA_WARNING_PUSH			TRNT_DO_PRAGMA(warning(push))
	#define TRNT_PRAGMA_WARNING_POP				TRNT_DO_PRAGMA(warning(pop))
	#define TRNT_DISABLE_WARNING(WarningNumber) TRNT_DO_PRAGMA(warning(disable : WarningNumber))
#endif