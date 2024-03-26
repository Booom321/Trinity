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