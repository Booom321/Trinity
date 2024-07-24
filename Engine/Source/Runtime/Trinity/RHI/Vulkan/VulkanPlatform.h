#pragma once

#if defined(TRNT_SUPPORT_VULKAN_RHI)

#include "Trinity/Core/Defines.h"
#include "Trinity/Core/PlatformDetection.h"
#include "Trinity/Core/Containers/DynamicArray.h"

#include "Trinity/Window/Window.h"

#include "IncludeVulkanHeader.h"

class TRNT_API TVulkanPlatform
{
public:
	static TBool LoadVulkanLibrary();
	static void FreeVulkanLibrary();
	static void GetInstanceExtension(TDynamicArray<const TChar*>& Extensions);
	static VkResult CreateVulkanSurface(TWindow* Window, VkInstance Instance, VkSurfaceKHR* Surface);

	static PFN_vkGetInstanceProcAddr VulkanGetInstanceProcAddr;
};

#endif