#pragma once

#if defined(TRNT_USE_VULKAN_RHI)

#include "IncludeVulkanHeader.h"
#include "VulkanRHI.h"

class TWindow;

class TRNT_API TVulkanSwapChain
{
public:
	TVulkanSwapChain(TVulkanRHI* VulkanRHIPointer, TWindow* WindowHandle, TUInt32 Width, TUInt32 Height);
	~TVulkanSwapChain();

	TBool Initialize();
	void Destroy();

private:
	VkSurfaceFormatKHR ChooseSuitableSurfaceFormat(const TDynamicArray<VkSurfaceFormatKHR>& SurfaceFormats, VkColorSpaceKHR DesiredColorSpace);
	VkPresentModeKHR ChoosePresentMode(const TDynamicArray<VkPresentModeKHR>& PresentModes);

	VkSwapchainKHR SwapChain;
	VkSurfaceKHR Surface;
	TUInt32 Width;
	TUInt32 Height;
	VkSurfaceFormatKHR SwapChainSurfaceFormat;

	TVulkanRHI* VulkanRHIPointer;
	TVulkanDevice* VulkanDevicePointer;
	TWindow* WindowHandle;
};

#endif