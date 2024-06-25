#pragma once

#if defined(TRNT_USE_VULKAN_RHI)

#include "Trinity/Core/Config.h"
#include "Trinity/Core/Logging/Log.h"

#if defined(TRNT_PLATFORM_WIN64)
#	define VK_USE_PLATFORM_WIN32_KHR
#elif defined(TRNT_PLATFORM_IOS) || defined(TRNT_PLATFORM_IOS_SIMULATOR)
#	define VK_USE_PLATFORM_IOS_MVK
#elif defined(TRNT_PLATFORM_MAC)
#	define VK_USE_PLATFORM_MACOS_MVK
#elif defined(TRNT_PLATFORM_ANDROID)
#	define VK_USE_PLATFORM_ANDROID_KHR
#elif defined(TRNT_PLATFORM_LINUX)
#	define VK_USE_PLATFORM_XLIB_KHR
#elif defined(TRNT_PLATFORM_UNIX)
#	define __UNKNOWN__
#elif defined(TRNT_PLATFORM_POSIX)
#	define __UNKNOWN__
#endif 

#if defined(TRNT_PLATFORM_APPLE)
#	define VK_USE_PLATFORM_METAL_EXT
#endif

#ifdef TRNT_USE_GLFW
#	define GLFW_INCLUDE_VULKAN
#	include <GLFW/glfw3.h>
#else
#	include <vulkan/vulkan.h>
#endif

#if !defined(TRNT_USE_VULKAN_DEBUG_UTILS) && defined(VK_EXT_debug_utils)
#	define TRNT_USE_VULKAN_DEBUG_UTILS 1
#endif 

#if !defined(TRNT_USE_VULKAN_DEBUG_REPORT) && defined(VK_EXT_debug_report)
#	define TRNT_USE_VULKAN_DEBUG_REPORT 1
#endif 

TRNT_DECLARE_LOG_INFO(VulkanRHI, TLogLevel::EDebug);

#endif