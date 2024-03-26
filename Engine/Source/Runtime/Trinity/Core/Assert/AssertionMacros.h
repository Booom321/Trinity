#pragma once

#include "Trinity/Core/Defines.h"
#include "Trinity/Core/PlatformDetection.h"

#include "Trinity/Core/Config.h"

#include "Trinity/Core/Utilities/OutputStream.h"

#ifdef TRNT_DEBUG
#	if defined(TRNT_PLATFORM_WIN32) || defined(TRNT_PLATFORM_WIN64)
#		define TRNT_DEBUG_BREAK __debugbreak()
#	elif defined(TRNT_PLATFORM_LINUX)
#		include <signal.h>
#		define TRNT_DEBUG_BREAK raise(SIGTRAP)
#	else
#		error "Current platform doesn't support debug break!'"
#	endif
#else 
#	define TRNT_DEBUG_BREAK
#endif

#ifdef TRNT_ENABLE_ASSERTIONS
#	define TRNT_ASSERT(Expression)\
	do {\
		if (!(Expression))\
		{\
			Printf("Assertion failed: {} [File: {}, Line: {}]\n", #Expression, __FILE__, __LINE__);\
			TRNT_DEBUG_BREAK;\
		}\
	} while (0)

#	define TRNT_ASSERT_MESSAGE(Expression, Message) \
	do {\
		if (!(Expression))\
		{\
			Printf("Assertion failed: {} [File: {}, Line: {}, Message: {}]\n", #Expression, __FILE__, __LINE__, Message);\
			TRNT_DEBUG_BREAK;\
		}\
	} while (0)

#	define TRNT_ASSERT_EQUAL(First, Second) TRNT_ASSERT(First == Second)

#	define TRNT_ASSERT_NOT_EQUAL(First, Second) TRNT_ASSERT(First != Second)

#   define TRNT_ASSERT_IS_NULL(Pointer) TRNT_ASSERT(Pointer == nullptr)

#   define TRNT_ASSERT_IS_NOT_NULL(Pointer) TRNT_ASSERT(Pointer != nullptr)

#	define TRNT_ASSERT_AT_COMPILE_TIME(Expression) static_assert(Expression);

#	define TRNT_ASSERT_AT_COMPILE_TIME_MESSAGE(Expression, Message) static_assert(Expression, Message);

#else
#	define TRNT_ASSERT(Expression)

#	define TRNT_ASSERT_MESSAGE(Expression, Message)

#	define TRNT_ASSERT_EQUAL(First, Second)

#	define TRNT_ASSERT_NOT_EQUAL(First, Second)

#   define TRNT_ASSERT_IS_NULL(Pointer) 

#   define TRNT_ASSERT_IS_NOT_NULL(Pointer)

#	define TRNT_ASSERT_AT_COMPILE_TIME(Expression)

#	define TRNT_ASSERT_AT_COMPILE_TIME_MESSAGE(Expression, Message)
#endif