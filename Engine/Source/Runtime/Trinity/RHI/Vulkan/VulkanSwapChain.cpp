#include "TrinityPCH.h"

#if defined(TRNT_SUPPORT_VULKAN_RHI)

#include "VulkanSwapChain.h"
#include "VulkanUtils.h"
#include "VulkanDevice.h"
#include "VulkanCommandBuffer.h"

#include "Trinity/Core/Math/Math.h"

TVulkanSwapChain::TVulkanSwapChain(TVulkanRHI* VulkanRHIPointer, TWindow* WindowHandle, TUInt32 Width, TUInt32 Height)
	:	SwapChain(VK_NULL_HANDLE),
		Surface(VK_NULL_HANDLE),
		RenderPass(nullptr),
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

TBool TVulkanSwapChain::Initialize(TRecreateDesc* RecreateDesc)
{
	TVulkanPhysicalDevice& PhysicalDevice = VulkanDevicePointer->GetPhysicalDevice();
	TRNT_ASSERT_IS_NOT_NULL(PhysicalDevice.Handle);

	const TVulkanQueue& GraphicsQueue = VulkanDevicePointer->GetVulkanQueueByFlag(TVulkanQueueFlags::EGraphicsQueue);
	
	if (Surface == VK_NULL_HANDLE)
	{
		if (RecreateDesc && RecreateDesc->OldSurfaceHandle != VK_NULL_HANDLE)
		{
			Surface = RecreateDesc->OldSurfaceHandle;
			RecreateDesc->OldSurfaceHandle = VK_NULL_HANDLE;
		}
		else if (TVulkanPlatform::CreateVulkanSurface(WindowHandle, VulkanRHIPointer->GetVkInstance(), &Surface) != VK_SUCCESS)
		{
			TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>("Failed to create Vulkan surface!");

			return false;
		}
	}

	VkBool32 Supported = VK_FALSE;
	TRNT_CHECK_VULKAN_RESULT(TVulkanRHI::VulkanPFNFunctions.GetPhysicalDeviceSurfaceSupportKHR(PhysicalDevice.Handle, GraphicsQueue.QueueFamilyIndex, Surface, &Supported));

	if (Supported == VK_FALSE)
	{
		TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>("Vulkan graphics queue doesn't support present.");
		return false;
	}

	// Get surface formats
	TUInt32 SurfaceFormatCount = 0;
	TRNT_CHECK_VULKAN_RESULT(TVulkanRHI::VulkanPFNFunctions.GetPhysicalDeviceSurfaceFormatsKHR(PhysicalDevice.Handle, Surface, &SurfaceFormatCount, nullptr));

	TDynamicArray<VkSurfaceFormatKHR> SurfaceFormats;
	if (SurfaceFormatCount > 0)
	{
		SurfaceFormats.Resize(SurfaceFormatCount);
		TRNT_CHECK_VULKAN_RESULT(TVulkanRHI::VulkanPFNFunctions.GetPhysicalDeviceSurfaceFormatsKHR(PhysicalDevice.Handle, Surface, &SurfaceFormatCount, SurfaceFormats.GetData()));

		TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>("Found {} surface formats.", SurfaceFormatCount);
		for (TUInt32 SurfaceFormatIndex = 0; SurfaceFormatIndex < SurfaceFormatCount; ++SurfaceFormatIndex)
		{
			TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>(
				"* Format={} | Colorspace={}", static_cast<unsigned>(SurfaceFormats[SurfaceFormatIndex].format), static_cast<unsigned>(SurfaceFormats[SurfaceFormatIndex].colorSpace));
		}
	}

	// Get surface present modes
	TUInt32 PresentModeCount = 0;
	TRNT_CHECK_VULKAN_RESULT(TVulkanRHI::VulkanPFNFunctions.GetPhysicalDeviceSurfacePresentModesKHR(PhysicalDevice.Handle, Surface, &PresentModeCount, nullptr));

	TDynamicArray<VkPresentModeKHR> PresentModes;
	if (PresentModeCount > 0)
	{
		PresentModes.Resize(PresentModeCount);
		TRNT_CHECK_VULKAN_RESULT(TVulkanRHI::VulkanPFNFunctions.GetPhysicalDeviceSurfacePresentModesKHR(PhysicalDevice.Handle, Surface, &PresentModeCount, PresentModes.GetData()));

		TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>("Found {} surface present modes.", PresentModeCount);
		for (TUInt32 PresentModeIndex = 0; PresentModeIndex < PresentModeCount; ++PresentModeIndex)
		{
			TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>("* Present mode {}: {}", PresentModeIndex + 1, static_cast<unsigned>(PresentModes[PresentModeIndex]));
		}
	}

	// Get surface capabilities
	VkSurfaceCapabilitiesKHR SurfaceCapabilities;
	TRNT_CHECK_VULKAN_RESULT(TVulkanRHI::VulkanPFNFunctions.GetPhysicalDeviceSurfaceCapabilitiesKHR(PhysicalDevice.Handle, Surface, &SurfaceCapabilities));

	TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>("SwapChain current extent: {}x{}.", SurfaceCapabilities.currentExtent.width, SurfaceCapabilities.currentExtent.height);

	VkExtent2D SwapChainExtent2D{};

	if (SurfaceCapabilities.currentExtent.width != 0xFFFFFFFF && SurfaceCapabilities.currentExtent.height != 0xFFFFFFFF)
	{
		SwapChainExtent2D = SurfaceCapabilities.currentExtent;
	}
	else
	{
		SwapChainExtent2D = { Width, Height };
		SwapChainExtent2D.width = TNsMath::Max(TNsMath::Min(SwapChainExtent2D.width, SurfaceCapabilities.maxImageExtent.width), SurfaceCapabilities.minImageExtent.width);
		SwapChainExtent2D.height = TNsMath::Max(TNsMath::Min(SwapChainExtent2D.height, SurfaceCapabilities.maxImageExtent.height), SurfaceCapabilities.minImageExtent.height);
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

	VkSwapchainCreateInfoKHR SwapChainCreateInfo;
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
	SwapChainCreateInfo.oldSwapchain = (RecreateDesc == nullptr) ? VK_NULL_HANDLE : RecreateDesc->OldSwapChainHandle;
	SwapChainCreateInfo.preTransform = SurfaceCapabilities.currentTransform;
	SwapChainCreateInfo.imageExtent = SwapChainExtent2D;

#if defined(TRNT_PLATFORM_WIN64)
	VkSurfaceFullScreenExclusiveInfoEXT SurfaceFullScreenExclusiveInfo;
	TMemory::Memset(&SurfaceFullScreenExclusiveInfo, 0, sizeof(SurfaceFullScreenExclusiveInfo));
	SurfaceFullScreenExclusiveInfo.sType = VK_STRUCTURE_TYPE_SURFACE_FULL_SCREEN_EXCLUSIVE_INFO_EXT;

	if (VulkanRHIPointer->GetVulkanRHIFeatures().SupportsFullscreenExclusive)
	{
		SurfaceFullScreenExclusiveInfo.fullScreenExclusive = WindowHandle->IsFullscreen() ? VK_FULL_SCREEN_EXCLUSIVE_ALLOWED_EXT : VK_FULL_SCREEN_EXCLUSIVE_DISALLOWED_EXT;
		SurfaceFullScreenExclusiveInfo.pNext = (void*)SwapChainCreateInfo.pNext;
		SwapChainCreateInfo.pNext = &SurfaceFullScreenExclusiveInfo;
	}
#endif

	TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>("SwapChain min image count = {}.", SwapChainCreateInfo.minImageCount);

	VkDevice VulkanDeviceHandle = VulkanDevicePointer->GetDevice();

	VkResult Result = TVulkanRHI::VulkanPFNFunctions.CreateSwapchainKHR(VulkanDeviceHandle, &SwapChainCreateInfo, nullptr, &SwapChain);
	if (Result != VK_SUCCESS)
	{
		TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>("Failed to create Vulkan SwapChain (result: {}).", TVulkanUtils::ConvertVkResultToCString(Result));
		SwapChain = VK_NULL_HANDLE;
		return false;
	}

	if (RecreateDesc && RecreateDesc->OldSwapChainHandle != VK_NULL_HANDLE)
	{
		if (RecreateDesc->OldSwapChainHandle != VK_NULL_HANDLE)
		{
			TVulkanRHI::VulkanPFNFunctions.DestroySwapchainKHR(VulkanDeviceHandle, RecreateDesc->OldSwapChainHandle, VK_NULL_HANDLE);
			RecreateDesc->OldSwapChainHandle = VK_NULL_HANDLE;
		}

		if (RecreateDesc->OldSurfaceHandle != VK_NULL_HANDLE)
		{
			TVulkanRHI::VulkanPFNFunctions.DestroySurfaceKHR(VulkanRHIPointer->GetVkInstance(), RecreateDesc->OldSurfaceHandle, VK_NULL_HANDLE);
			RecreateDesc->OldSurfaceHandle = VK_NULL_HANDLE;
		}
	}

	this->Width = SwapChainExtent2D.width;
	this->Height = SwapChainExtent2D.height;

	TLog::Success<TRNT_GET_LOG_INFO(VulkanRHI)>(
		"Created SwapChain successfully [{}x{}, format: {}, minimum image count: {}].", this->Width, this->Height, static_cast<unsigned>(SuitableSurfaceFormat.format), ImageCount);

	// Retrieving the swap chain images
	TUInt32 SwapChainImageCount = 0;
	TRNT_CHECK_VULKAN_RESULT(TVulkanRHI::VulkanPFNFunctions.GetSwapchainImagesKHR(VulkanDeviceHandle, SwapChain, &SwapChainImageCount, nullptr));

	SwapChainImages.Resize(SwapChainImageCount);
	TRNT_CHECK_VULKAN_RESULT(TVulkanRHI::VulkanPFNFunctions.GetSwapchainImagesKHR(VulkanDeviceHandle, SwapChain, &SwapChainImageCount, SwapChainImages.GetData()));

	if (!CreateVulkanImageViews(SuitableSurfaceFormat.format))
	{
		TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>(
			"Failed to create {} Vulkan image views with format {}", SwapChainImageViews.GetElementCount(), static_cast<TInt32>(SuitableSurfaceFormat.format));
		return false;
	}
	else
	{
		TLog::Success<TRNT_GET_LOG_INFO(VulkanRHI)>(
			" Created {} Vulkan image views with format {} successfully!", SwapChainImageViews.GetElementCount(), static_cast<TInt32>(SuitableSurfaceFormat.format));
	}

	this->SwapChainSurfaceFormat = SuitableSurfaceFormat;
	this->DepthFormat = FindDepthFormat();

	if (this->DepthFormat == VK_FORMAT_UNDEFINED)
	{
		TLog::Warning<TRNT_GET_LOG_INFO(VulkanRHI)>("Couldn't find supported depth format!");
	}
	else
	{
		TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>("Found swap chain depth format: {}.", static_cast<TUInt32>(this->DepthFormat));
	}

	if (!CreateVulkanDepthImageViews(this->DepthFormat))
	{
		TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>(
			"Failed to create {} Vulkan depth image views with format {}", SwapChainDepthImageViews.GetElementCount(), static_cast<TInt32>(this->DepthFormat));
		return false;
	}
	else
	{
		TLog::Success<TRNT_GET_LOG_INFO(VulkanRHI)>(
			" Created {} Vulkan depth image views with format {} successfully!", SwapChainDepthImageViews.GetElementCount(), static_cast<TInt32>(this->DepthFormat));
	}

	if (!CreateRenderPass())
	{
		TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>("Failed to create Vulkan render pass!");
		return false;
	}
	else
	{
		TLog::Success<TRNT_GET_LOG_INFO(VulkanRHI)>("Created Vulkan render pass successfully!");
	}

	if (!CreateSwapChainFrameBuffers())
	{
		TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>("Failed to create swap chain frame buffers!");
		return false;
	}
	else
	{
		TLog::Success<TRNT_GET_LOG_INFO(VulkanRHI)>("Created {} swap chain frame buffers successfully!", SwapChainFrameBuffers.GetElementCount());
	}

	if (!CreateSynchronousObjects())
	{
		return false;
	}
	else
	{
		TLog::Success<TRNT_GET_LOG_INFO(VulkanRHI)>("Created {} Vulkan semaphore(s) successfully!", ImageAcquiredSemaphores.GetElementCount());
		TLog::Success<TRNT_GET_LOG_INFO(VulkanRHI)>("Created {} Vulkan fence(s) successfully!", ImageAcquiredFences.GetElementCount());
	}

	CurrentImageIndex = 0;

	return true;
}

TBool TVulkanSwapChain::RecreateSwapChain()
{
	// If the window handle is iconified, that means window width = 0 and window height = 0
	while (WindowHandle->IsIconified()) 
	{
		WindowHandle->WaitEvents();
	}
	VulkanDevicePointer->WaitIdle();

	TRecreateDesc RecreateDesc{};
	RecreateDesc.OldSwapChainHandle = SwapChain;
	RecreateDesc.OldSurfaceHandle = Surface;
	SwapChain = VK_NULL_HANDLE;
	Surface = VK_NULL_HANDLE;

	Destroy();
	const TBool Result = Initialize(&RecreateDesc);

	if (RecreateDesc.OldSwapChainHandle != VK_NULL_HANDLE)
	{
		TLog::Warning<TRNT_GET_LOG_INFO(VulkanRHI)>("Old swap chain handle has not been deleted yet, it will be deleted automatically");
		TVulkanRHI::VulkanPFNFunctions.DestroySwapchainKHR(VulkanDevicePointer->GetDevice(), RecreateDesc.OldSwapChainHandle, nullptr);
		RecreateDesc.OldSwapChainHandle = VK_NULL_HANDLE;
	}

	if (RecreateDesc.OldSurfaceHandle != VK_NULL_HANDLE)
	{
		TLog::Warning<TRNT_GET_LOG_INFO(VulkanRHI)>("Old surface handle has not been deleted yet, it will be deleted automatically");
		TVulkanRHI::VulkanPFNFunctions.DestroySurfaceKHR(VulkanRHIPointer->GetVkInstance(), RecreateDesc.OldSurfaceHandle, nullptr);
		RecreateDesc.OldSurfaceHandle = VK_NULL_HANDLE;
	}

	return Result;
}

TBool TVulkanSwapChain::CreateVulkanImageViews(VkFormat Format)
{
	VkImageViewCreateInfo ImageViewCreateInfo;
	TMemory::Memset(&ImageViewCreateInfo, 0, sizeof(ImageViewCreateInfo));

	ImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	ImageViewCreateInfo.pNext = nullptr;
	ImageViewCreateInfo.flags = 0;
	ImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	ImageViewCreateInfo.format = Format;
	
	ImageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	ImageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	ImageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	ImageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

	ImageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	ImageViewCreateInfo.subresourceRange.baseMipLevel = 0;
	ImageViewCreateInfo.subresourceRange.levelCount = 1;
	ImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
	ImageViewCreateInfo.subresourceRange.layerCount = 1;

	SwapChainImageViews.Resize(SwapChainImages.GetElementCount());

	VkResult Result;
	for (TInt64 ImageIndex = 0, ImageCount = SwapChainImages.GetElementCount(); ImageIndex < ImageCount; ++ImageIndex)
	{
		ImageViewCreateInfo.image = SwapChainImages[ImageIndex];

		Result = TVulkanRHI::VulkanPFNFunctions.CreateImageView(VulkanDevicePointer->GetDevice(), &ImageViewCreateInfo, nullptr, &SwapChainImageViews[ImageIndex]);
		if (Result != VK_SUCCESS)
		{
			TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>("Failed to create VkImageView at index {}.", ImageIndex);
			return false;
		}
	}

	return true;
}

TBool TVulkanSwapChain::CreateVulkanDepthImageViews(VkFormat DepthFormat)
{
	VkDevice Device = VulkanDevicePointer->GetDevice();

	const TInt64 SwapChainImgCount = SwapChainImages.GetElementCount();
	SwapChainDepthImages.Resize(SwapChainImgCount);
	SwapChainDepthImageViews.Resize(SwapChainImgCount);
	SwapChainDepthImageMemories.Resize(SwapChainImgCount);

	for (TInt64 ImgIndex = 0; ImgIndex < SwapChainImgCount; ++ImgIndex)
	{
		VkImageCreateInfo ImageCreateInfo;
		TMemory::Memset(&ImageCreateInfo, 0, sizeof(ImageCreateInfo));

		ImageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		ImageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		ImageCreateInfo.extent.width = Width;
		ImageCreateInfo.extent.height = Height;
		ImageCreateInfo.extent.depth = 1;
		ImageCreateInfo.mipLevels = 1;
		ImageCreateInfo.arrayLayers = 1;
		ImageCreateInfo.format = DepthFormat;
		ImageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		ImageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		ImageCreateInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		ImageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		ImageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		ImageCreateInfo.flags = 0;

		if (!VulkanDevicePointer->CreateVulkanImageWithInfo(ImageCreateInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, SwapChainDepthImages[ImgIndex], SwapChainDepthImageMemories[ImgIndex]))
		{
			return false;
		}

		VkImageViewCreateInfo ImageViewCreateInfo;
		TMemory::Memset(&ImageViewCreateInfo, 0, sizeof(ImageViewCreateInfo));

		ImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		ImageViewCreateInfo.image = SwapChainDepthImages[ImgIndex];
		ImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		ImageViewCreateInfo.format = DepthFormat;
		ImageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		ImageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		ImageViewCreateInfo.subresourceRange.levelCount = 1;
		ImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		ImageViewCreateInfo.subresourceRange.layerCount = 1;

		if (TVulkanRHI::VulkanPFNFunctions.CreateImageView(Device, &ImageViewCreateInfo, nullptr, &SwapChainDepthImageViews[ImgIndex]) != VK_SUCCESS)
		{
			TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>("Failed to create depth image view (format: {}).", static_cast<TUInt32>(DepthFormat));
			return false;
		}
	}
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

VkFormat TVulkanSwapChain::FindDepthFormat()
{
	return VulkanDevicePointer->FindSupportedFormat(
		{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT }, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

TBool TVulkanSwapChain::CreateRenderPass()
{
	RenderPass = new TVulkanRenderPass(VulkanDevicePointer);

	if (!RenderPass->Initialize(SwapChainSurfaceFormat.format, DepthFormat))
	{
		return false;
	}

	return true;
}

TBool TVulkanSwapChain::CreateSwapChainFrameBuffers()
{
	VkDevice Device = VulkanDevicePointer->GetDevice();

	TInt64 ImageCount = SwapChainImages.GetElementCount();
	SwapChainFrameBuffers.Resize(ImageCount);

	for (TInt64 ImageIndex = 0; ImageIndex < ImageCount; ++ImageIndex)
	{
		VkImageView Attachment[] = { SwapChainImageViews[ImageIndex], SwapChainDepthImageViews[ImageIndex] };
		
		VkFramebufferCreateInfo FrameBufferCreateInfo;
		TMemory::Memset(&FrameBufferCreateInfo, 0, sizeof(FrameBufferCreateInfo));
		FrameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		FrameBufferCreateInfo.renderPass = RenderPass->GetVulkanRenderPass();
		FrameBufferCreateInfo.attachmentCount = TRNT_GET_ARRAY_LENGTH(Attachment);
		FrameBufferCreateInfo.pAttachments = Attachment;
		FrameBufferCreateInfo.width = Width;
		FrameBufferCreateInfo.height = Height;
		FrameBufferCreateInfo.layers = 1;

		if (TVulkanRHI::VulkanPFNFunctions.CreateFramebuffer(Device, &FrameBufferCreateInfo, nullptr, &SwapChainFrameBuffers[ImageIndex]) != VK_SUCCESS)
		{
			return false;
		}
	}

	return true;
}

TBool TVulkanSwapChain::CreateSynchronousObjects()
{
	const TInt64 ImageCount = SwapChainImages.GetElementCount();
	VkDevice Device = VulkanDevicePointer->GetDevice();

	ImageAcquiredSemaphores.Resize(ImageCount);
	RenderFinishedSemaphores.Resize(ImageCount);
	ImageAcquiredFences.Resize(ImageCount);

	VkSemaphoreCreateInfo SemaphoreCreateInfo;
	TMemory::Memset(&SemaphoreCreateInfo, 0, sizeof(SemaphoreCreateInfo));
	SemaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	SemaphoreCreateInfo.pNext = nullptr;

	VkFenceCreateInfo FenceCreateInfo;
	TMemory::Memset(&FenceCreateInfo, 0, sizeof(FenceCreateInfo));
	FenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	FenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	FenceCreateInfo.pNext = nullptr;

	for (TInt64 Index = 0; Index < ImageCount; ++Index)
	{
		if (TVulkanRHI::VulkanPFNFunctions.CreateSemaphore(Device, &SemaphoreCreateInfo, nullptr, &ImageAcquiredSemaphores[Index]) != VK_SUCCESS)
		{
			TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>("Failed to create image acquired semaphore at index {}.", Index);
			return false;
		}

		if (TVulkanRHI::VulkanPFNFunctions.CreateSemaphore(Device, &SemaphoreCreateInfo, nullptr, &RenderFinishedSemaphores[Index]) != VK_SUCCESS)
		{
			TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>("Failed to create render finished semaphore at index {}.", Index);
			return false;
		}

		if (TVulkanRHI::VulkanPFNFunctions.CreateFence(Device, &FenceCreateInfo, nullptr, &ImageAcquiredFences[Index]) != VK_SUCCESS)
		{
			TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>("Failed to create fence at index {}.", Index);
			return false;
		}

		TLog::Success<TRNT_GET_LOG_INFO(VulkanRHI)>("Created semaphores and fence at index {} successfully.", Index);
	}

	return true;
}

VkResult TVulkanSwapChain::AcquireNextImage(TUInt32* ImageIndex)
{
	const TInt64 ImageCount = SwapChainImages.GetElementCount();
	VkDevice Device = VulkanDevicePointer->GetDevice();

	VkFence CurrentAcquireFence = ImageAcquiredFences[CurrentImageIndex];

	TRNT_CHECK_VULKAN_RESULT(TVulkanRHI::VulkanPFNFunctions.WaitForFences(Device, 1, &CurrentAcquireFence, VK_TRUE, UINT64_MAX));
	TRNT_CHECK_VULKAN_RESULT(TVulkanRHI::VulkanPFNFunctions.ResetFences(Device, 1, &CurrentAcquireFence));

	VkResult Result = TVulkanRHI::VulkanPFNFunctions.AcquireNextImageKHR(Device, SwapChain, UINT64_MAX, ImageAcquiredSemaphores[CurrentImageIndex], CurrentAcquireFence, ImageIndex);

	if (Result == VK_ERROR_OUT_OF_DATE_KHR || Result == VK_SUBOPTIMAL_KHR)
	{
		TLog::Warning<TRNT_GET_LOG_INFO(VulkanRHI)>("Acquire image result {}", (Result == VK_ERROR_OUT_OF_DATE_KHR) ? TRNT_STRINGIFY(VK_ERROR_OUT_OF_DATE_KHR) : TRNT_STRINGIFY(VK_SUBOPTIMAL_KHR));

		if (Result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			if (!RecreateSwapChain())
			{
				TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>("Failed to recreate swap chain.");
			}
			else
			{
				TLog::Success<TRNT_GET_LOG_INFO(VulkanRHI)>("Recreated swap chain successfully.");
			}
		}
	}
	else if (Result != VK_SUCCESS)
	{
		TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>("Failed to acquire next image (result={})", TVulkanUtils::ConvertVkResultToCString(Result));
	}
	
	return Result;
}

VkResult TVulkanSwapChain::Present()
{
	VkPresentInfoKHR PresentInfoKHR;
	TMemory::Memset(&PresentInfoKHR, 0, sizeof(PresentInfoKHR));
	PresentInfoKHR.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	PresentInfoKHR.waitSemaphoreCount = 1;
	PresentInfoKHR.pWaitSemaphores = &RenderFinishedSemaphores[CurrentImageIndex];
	PresentInfoKHR.swapchainCount = 1;
	PresentInfoKHR.pSwapchains = &SwapChain;
	PresentInfoKHR.pNext = nullptr;
	PresentInfoKHR.pImageIndices = (TUInt32*)&CurrentImageIndex;

	VkResult Result = TVulkanRHI::VulkanPFNFunctions.QueuePresentKHR(VulkanDevicePointer->GetVulkanQueueByFlag(TVulkanQueueFlags::EPresentQueue).QueueHandle, &PresentInfoKHR);

	if (Result == VK_ERROR_OUT_OF_DATE_KHR || Result == VK_SUBOPTIMAL_KHR)
	{
		if (!RecreateSwapChain())
		{
			TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>("Failed to recreate swap chain.");
		}
		else
		{
			TLog::Success<TRNT_GET_LOG_INFO(VulkanRHI)>("Recreated swap chain successfully.");
		}

		return Result;
	}
	else if (Result != VK_SUCCESS)
	{
		TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>("Present result: {}", TVulkanUtils::ConvertVkResultToCString(Result));
	}

	CurrentImageIndex = (CurrentImageIndex + 1) % SwapChainImages.GetElementCount();

	return Result;
}

void TVulkanSwapChain::SubmitCommandBuffer(TVulkanCommandBuffer& CmdBuffer)
{
	CmdBuffer.SubmitCommandBuffer(
		VulkanDevicePointer->GetVulkanQueueByFlag(TVulkanQueueFlags::EGraphicsQueue),
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		ImageAcquiredSemaphores[CurrentImageIndex],
		RenderFinishedSemaphores[CurrentImageIndex],
		ImageAcquiredFences[CurrentImageIndex]);
}

void TVulkanSwapChain::Destroy()
{
	const TVulkanPFNFunctions& VulkanPFNFunctions = TVulkanRHI::VulkanPFNFunctions;
	const TInt64 ImageCount = SwapChainImages.GetElementCount();
	VkDevice Device = VulkanDevicePointer->GetDevice();

	for (TInt64 Index = 0; Index < ImageCount; ++Index)
	{
		VulkanPFNFunctions.DestroySemaphore(Device, ImageAcquiredSemaphores[Index], nullptr);
		VulkanPFNFunctions.DestroySemaphore(Device, RenderFinishedSemaphores[Index], nullptr);
		VulkanPFNFunctions.DestroyFence(Device, ImageAcquiredFences[Index], nullptr);
	}

	for (TInt64 ImageViewIndex = 0, ImageViewCount = SwapChainImageViews.GetElementCount(); ImageViewIndex < ImageViewCount; ++ImageViewIndex)
	{
		VulkanPFNFunctions.DestroyImageView(Device, SwapChainImageViews[ImageViewIndex], nullptr);
	}

	for (TInt64 DepthImgIndex = 0, DepthImageCount = SwapChainDepthImages.GetElementCount(); DepthImgIndex < DepthImageCount; ++DepthImgIndex)
	{
		VulkanPFNFunctions.DestroyImageView(Device, SwapChainDepthImageViews[DepthImgIndex], nullptr);
		VulkanPFNFunctions.DestroyImage(Device, SwapChainDepthImages[DepthImgIndex], nullptr);
		VulkanPFNFunctions.FreeMemory(Device, SwapChainDepthImageMemories[DepthImgIndex], nullptr);
	}

	for (VkFramebuffer& Framebuffer : SwapChainFrameBuffers)
	{
		VulkanPFNFunctions.DestroyFramebuffer(Device, Framebuffer, nullptr);
	}

	if (SwapChain)
	{
		VulkanPFNFunctions.DestroySwapchainKHR(Device, SwapChain, nullptr);
		SwapChain = VK_NULL_HANDLE;
	}

	if (Surface)
	{
		VulkanPFNFunctions.DestroySurfaceKHR(VulkanRHIPointer->GetVkInstance(), Surface, nullptr);
		Surface = VK_NULL_HANDLE;
	}

	if (RenderPass)
	{
		RenderPass->Destroy();
		delete RenderPass;
		RenderPass = nullptr;
	}
}

#endif