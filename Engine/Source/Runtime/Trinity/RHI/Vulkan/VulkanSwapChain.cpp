#include "TrinityPCH.h"

#if defined(TRNT_USE_VULKAN_RHI)


#include "VulkanSwapChain.h"
#include "VulkanUtils.h"
#include "VulkanDevice.h"

TVulkanSwapChain::TVulkanSwapChain(TVulkanRHI* VulkanRHIPointer, TWindow* WindowHandle, TUInt32 Width, TUInt32 Height)
	:	SwapChain(VK_NULL_HANDLE), 
		Surface(VK_NULL_HANDLE),
		VulkanRHIPointer(VulkanRHIPointer),
		WindowHandle(WindowHandle),
		Width(Width),
		Height(Height)
{
	TRNT_ASSERT(VulkanRHIPointer != nullptr && WindowHandle != nullptr);

	VulkanDevicePointer = VulkanRHIPointer->GetVulkanDevice();
}

TVulkanSwapChain::~TVulkanSwapChain()
{

}

TBool TVulkanSwapChain::Initialize()
{
	const TVulkanPFNFunctions& VulkanPFNFunctions = TVulkanRHI::GetVulkanPFNFunctions();
	VkPhysicalDevice PhysicalDevice = VulkanDevicePointer->GetPhysicalDeviceHandle();
	TRNT_ASSERT_IS_NOT_NULL(PhysicalDevice);

	const TVulkanQueue* Queues = VulkanDevicePointer->GetVulkanQueuesData();

	if (TVulkanPlatform::CreateVulkanSurface(WindowHandle, VulkanRHIPointer->GetVkInstance(), &Surface) != VK_SUCCESS)
	{
		TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>("Failed to create Vulkan surface!");

		return false;
	}

	VkBool32 Supported = VK_FALSE;
	TRNT_CHECK_VULKAN_RESULT(VulkanPFNFunctions.GetPhysicalDeviceSurfaceSupportKHR(PhysicalDevice, Queues[(TUInt32)TVulkanQueueFlags::EGraphicsQueue].QueueFamilyIndex, Surface, &Supported));

	if (Supported == VK_FALSE)
	{
		TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>("Vulkan graphics queue doesn't support present.");
		return false;
	}

	// Get surface formats
	TUInt32 SurfaceFormatCount = 0;
	TRNT_CHECK_VULKAN_RESULT(VulkanPFNFunctions.GetPhysicalDeviceSurfaceFormatsKHR(PhysicalDevice, Surface, &SurfaceFormatCount, nullptr));

	TDynamicArray<VkSurfaceFormatKHR> SurfaceFormats;
	if (SurfaceFormatCount > 0)
	{
		SurfaceFormats.Resize(SurfaceFormatCount);
		TRNT_CHECK_VULKAN_RESULT(VulkanPFNFunctions.GetPhysicalDeviceSurfaceFormatsKHR(PhysicalDevice, Surface, &SurfaceFormatCount, SurfaceFormats.GetData()));

		TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>("Found {} surface formats.", SurfaceFormatCount);
	}

	// Get surface present modes
	TUInt32 PresentModeCount = 0;
	TRNT_CHECK_VULKAN_RESULT(VulkanPFNFunctions.GetPhysicalDeviceSurfacePresentModesKHR(PhysicalDevice, Surface, &PresentModeCount, nullptr));

	TDynamicArray<VkPresentModeKHR> PresentModes;
	if (PresentModeCount > 0)
	{
		PresentModes.Resize(PresentModeCount);
		TRNT_CHECK_VULKAN_RESULT(VulkanPFNFunctions.GetPhysicalDeviceSurfacePresentModesKHR(PhysicalDevice, Surface, &PresentModeCount, PresentModes.GetData()));

		TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>("Found {} surface present modes.", PresentModeCount);
	}

	// Get surface capabilities
	VkSurfaceCapabilitiesKHR SurfaceCapabilities;
	TRNT_CHECK_VULKAN_RESULT(VulkanPFNFunctions.GetPhysicalDeviceSurfaceCapabilitiesKHR(PhysicalDevice, Surface, &SurfaceCapabilities));

	TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>("SwapChain current extent: {}x{}.", SurfaceCapabilities.currentExtent.width, SurfaceCapabilities.currentExtent.height);

	VkExtent2D SwapChainExtent2D = {};
	if (SurfaceCapabilities.currentExtent.width != 0xFFFFFFFF && SurfaceCapabilities.currentExtent.height != 0xFFFFFFFF)
	{
		SwapChainExtent2D = SurfaceCapabilities.currentExtent;
	}
	else
	{
		SwapChainExtent2D = { Width, Height };
		SwapChainExtent2D.width = TMath::Max(TMath::Min(SwapChainExtent2D.width, SurfaceCapabilities.maxImageExtent.width), SurfaceCapabilities.minImageExtent.width);
		SwapChainExtent2D.height = TMath::Max(TMath::Min(SwapChainExtent2D.height, SurfaceCapabilities.maxImageExtent.height), SurfaceCapabilities.minImageExtent.height);
	}

	VkColorSpaceKHR DesiredColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
	VkSurfaceFormatKHR SuitableSurfaceFormat = ChooseSuitableSurfaceFormat(SurfaceFormats, DesiredColorSpace);
	VkPresentModeKHR PresentMode = ChoosePresentMode(PresentModes);
	TUInt32 ImageCount = SurfaceCapabilities.minImageCount + 1;

	if (SurfaceCapabilities.maxImageCount > 0 && ImageCount > SurfaceCapabilities.maxImageCount)
	{
		ImageCount = SurfaceCapabilities.maxImageCount;
	}

	VkCompositeAlphaFlagBitsKHR CompositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	if (SurfaceCapabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR)
	{
		CompositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	}
	else if (SurfaceCapabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR)
	{
		CompositeAlpha = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;
	}
	else if (SurfaceCapabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR)
	{
		CompositeAlpha = VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR;
	}
	else if (SurfaceCapabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR)
	{
		CompositeAlpha = VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR;
	}

	VkSwapchainCreateInfoKHR SwapChainCreateInfo = {};
	TMemory::Memset(&SwapChainCreateInfo, 0, sizeof(SwapChainCreateInfo));

	SwapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	SwapChainCreateInfo.surface = Surface;
	SwapChainCreateInfo.imageFormat = SuitableSurfaceFormat.format;
	SwapChainCreateInfo.imageColorSpace = SuitableSurfaceFormat.colorSpace;
	SwapChainCreateInfo.compositeAlpha = CompositeAlpha;
	SwapChainCreateInfo.presentMode = PresentMode;
	SwapChainCreateInfo.minImageCount = ImageCount;
	SwapChainCreateInfo.imageArrayLayers = 1;
	SwapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	SwapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	SwapChainCreateInfo.clipped = VK_TRUE;
	SwapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;
	SwapChainCreateInfo.preTransform = SurfaceCapabilities.currentTransform;
	SwapChainCreateInfo.imageExtent = SwapChainExtent2D;
	SwapChainCreateInfo.pNext = nullptr;

#if defined(TRNT_PLATFORM_WIN64)
	if (VulkanRHIPointer->GetVulkanRHIFeatures().SupportsFullscreenExclusive)
	{
		VkSurfaceFullScreenExclusiveInfoEXT SurfaceFullScreenExclusiveInfo;
		TMemory::Memset(&SurfaceFullScreenExclusiveInfo, 0, sizeof(SurfaceFullScreenExclusiveInfo));
	
		SurfaceFullScreenExclusiveInfo.fullScreenExclusive = WindowHandle->IsFullscreen() ? VK_FULL_SCREEN_EXCLUSIVE_ALLOWED_EXT : VK_FULL_SCREEN_EXCLUSIVE_DISALLOWED_EXT;
		SwapChainCreateInfo.pNext = &SurfaceFullScreenExclusiveInfo;
	}
#endif

	TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>("SwapChain min image count = {}.", SwapChainCreateInfo.minImageCount);

	VkResult Result = VulkanPFNFunctions.CreateSwapchainKHR(VulkanDevicePointer->GetDevice(), &SwapChainCreateInfo, nullptr, &SwapChain);
	if (Result != VK_SUCCESS)
	{
		TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>("Failed to create Vulkan SwapChain!");
		SwapChain = VK_NULL_HANDLE;
		return false;
	}

	this->Width = SwapChainExtent2D.width;
	this->Height = SwapChainExtent2D.height;
	this->SwapChainSurfaceFormat = SuitableSurfaceFormat;

	TLog::Success<TRNT_GET_LOG_INFO(VulkanRHI)>(
		"Created SwapChain successfully [{}x{}, format: {}, min image count: {}].", this->Width, this->Height, static_cast<unsigned>(this->SwapChainSurfaceFormat.format), ImageCount);

	return true;
}

static const VkFormat PreferredFormats[] = {
	VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_A2B10G10R10_UNORM_PACK32, VK_FORMAT_A2R10G10B10_UNORM_PACK32, VK_FORMAT_A8B8G8R8_UNORM_PACK32, VK_FORMAT_B8G8R8A8_SRGB
};

VkSurfaceFormatKHR TVulkanSwapChain::ChooseSuitableSurfaceFormat(const TDynamicArray<VkSurfaceFormatKHR>& SurfaceFormats, VkColorSpaceKHR DesiredColorSpace)
{
	VkSurfaceFormatKHR Format = SurfaceFormats[0];

	const auto PreferredFormatsContainsSurfaceFormat = [](const VkFormat SurfaceFormat) -> TBool
	{
		const TInt32 PreferredFormatsCount = TRNT_GET_ARRAY_LENGTH(PreferredFormats);

		for (TInt32 Index = 0; Index < PreferredFormatsCount; ++Index)
		{
			if (PreferredFormats[Index] == SurfaceFormat)
			{
				return true;
			}
		}

		return false;
	};
	
	for (const VkSurfaceFormatKHR& SurfaceFormat : SurfaceFormats)
	{
		TBool Found = false;
		if (SurfaceFormat.colorSpace == DesiredColorSpace && PreferredFormatsContainsSurfaceFormat(SurfaceFormat.format))
		{
			Found = true;
		}
		else
		{
			if (SurfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR && PreferredFormatsContainsSurfaceFormat(SurfaceFormat.format))
			{
				Found = true;
			}
		}

		if (Found)
		{
			Format = SurfaceFormat;
			break;
		}
	}

	return Format;
}

VkPresentModeKHR TVulkanSwapChain::ChoosePresentMode(const TDynamicArray<VkPresentModeKHR>& PresentModes)
{
	VkPresentModeKHR PresentMode = VK_PRESENT_MODE_FIFO_KHR;

	for (VkPresentModeKHR Mode : PresentModes)
	{
		if (Mode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			PresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
			break;
		}
		else if (Mode == VK_PRESENT_MODE_IMMEDIATE_KHR)
		{
			PresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
		}
	}

	return PresentMode;
}

void TVulkanSwapChain::Destroy()
{
	const TVulkanPFNFunctions& VulkanPFNFunctions = TVulkanRHI::GetVulkanPFNFunctions();

	if (Surface)
	{
		VulkanPFNFunctions.DestroySurfaceKHR(VulkanRHIPointer->GetVkInstance(), Surface, nullptr);
		Surface = VK_NULL_HANDLE;
	}

	if (SwapChain)
	{
		VulkanPFNFunctions.DestroySwapchainKHR(VulkanDevicePointer->GetDevice(), SwapChain, nullptr);
		SwapChain = VK_NULL_HANDLE;
	}
}

#endif