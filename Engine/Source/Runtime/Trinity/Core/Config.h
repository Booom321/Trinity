#pragma once

#if !defined(NDEBUG) || defined(_DEBUG)
#	define TRNT_ENABLE_ASSERTIONS 1
#endif 

#if defined(TRNT_SUPPORTS_GLFW)
#	define TRNT_USE_GLFW
#endif

#if defined(TRNT_SUPPORTS_VULKAN)
#	define TRNT_USE_VULKAN_RHI 1
#elif defined(TRNT_SUPPORTS_DIRECTX)
#	define TRNT_USE_DIRECTX_RHI 1
#else
#	define TRNT_USE_UNKNOWN_RHI 0
#endif

#define TRNT_ENGINE_VERSION_MAJOR 1
#define TRNT_ENGINE_VERSION_MINOR 0
#define TRNT_ENGINE_VERSION_PATCH 0
