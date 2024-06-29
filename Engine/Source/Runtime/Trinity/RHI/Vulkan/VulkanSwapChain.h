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

public:
	TRNT_FORCE_INLINE VkSwapchainKHR GetVulkanSwapChain() const { return SwapChain; }
	TRNT_FORCE_INLINE VkSurfaceKHR GetVulkanSurface() const { return Surface; }

private:
	VkSurfaceFormatKHR ChooseSuitableSurfaceFormat(const TDynamicArray<VkSurfaceFormatKHR>& SurfaceFormats, VkColorSpaceKHR DesiredColorSpace);
	VkPresentModeKHR ChoosePresentMode(const TDynamicArray<VkPresentModeKHR>& PresentModes);

	TBool CreateVulkanImageViews();

	VkSwapchainKHR SwapChain;
	VkSurfaceKHR Surface;
	TUInt32 Width;
	TUInt32 Height;
	VkSurfaceFormatKHR SwapChainSurfaceFormat;

	TDynamicArray<VkImage> SwapChainImages;
	TDynamicArray<VkImageView> SwapChainImageViews;

	TVulkanRHI* VulkanRHIPointer;
	TVulkanDevice* VulkanDevicePointer;
	TWindow* WindowHandle;
};

#endif