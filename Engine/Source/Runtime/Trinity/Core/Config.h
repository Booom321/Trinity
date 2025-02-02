#pragma once

#include "PlatformDetection.h"

#if defined(_DEBUG)
	#define TRNT_ENABLE_ASSERTIONS 1
#endif

#if defined(TRNT_SUPPORT_GLFW)
	#define TRNT_USE_GLFW
#else
	#if defined(TRNT_PLATFORM_WIN64)
		#define TRNT_USE_WINDOWS_WINDOW
	#elif defined(TRNT_PLATFORM_IOS) || defined(TRNT_PLATFORM_IOS_SIMULATOR)
	//...
	#elif defined(TRNT_PLATFORM_MAC)
	//...
	#elif defined(TRNT_PLATFORM_ANDROID)
	//...
	#elif defined(TRNT_PLATFORM_LINUX)
	//...
	#elif defined(TRNT_PLATFORM_UNIX)
	//...
	#elif defined(TRNT_PLATFORM_POSIX)
	//...
	#endif
#endif

#define TRNT_ENGINE_VERSION_MAJOR 1
#define TRNT_ENGINE_VERSION_MINOR 0
#define TRNT_ENGINE_VERSION_PATCH 0
