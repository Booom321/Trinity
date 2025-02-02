#pragma once

#if defined(_WIN32) || defined(WIN32) || defined(__WIN32__)
	#if defined(_WIN64)
		#define TRNT_PLATFORM_WIN64
	#else
		#error "x86 are not supported!"
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	#define TRNT_PLATFORM_APPLE
	#if TARGET_IPHONE_SIMULATOR
		#define TRNT_PLATFORM_IOS
		#define TRNT_PLATFORM_IOS_SIMULATOR
	#elif TARGET_OS_IPHONE
		#define TRNT_PLATFORM_IOS
	#elif TARGET_OS_MAC
		#define TRNT_PLATFORM_MAC
	#else
		#error "Unknown Apple platform!"
	#endif
#elif defined(__ANDROID__)
	#define TRNT_PLATFORM_ANDROID
#elif defined(__linux__) || defined(__gnu_linux__)
	#define TRNT_PLATFORM_LINUX
#elif defined(__unix__)
	#define TRNT_PLATFORM_UNIX
#elif defined(_POSIX_VERSION)
	#define TRNT_PLATFORM_POSIX
#else
	#error "Unknown platform!"
#endif