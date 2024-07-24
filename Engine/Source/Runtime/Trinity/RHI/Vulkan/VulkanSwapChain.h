#pragma once

#if defined(TRNT_SUPPORT_VULKAN_RHI)

#include "VulkanRHI.h"
#include "VulkanRenderPass.h"

class TWindow;
class TVulkanCommandBuffer;

class TRNT_API TVulkanSwapChain
{
public:
	class TRecreateDesc
	{
	public:
		VkSwapchainKHR OldSwapChainHandle = VK_NULL_HANDLE;
		VkSurfaceKHR OldSurfaceHandle = VK_NULL_HANDLE;
	};

	TVulkanSwapChain(TVulkanRHI* VulkanRHIPointer, TWindow* WindowHandle, TUInt32 Width, TUInt32 Height);
	~TVulkanSwapChain();

	TBool Initialize(TRecreateDesc* RecreateDesc = nullptr);
	TBool RecreateSwapChain();
	void Destroy();

public:
	TRNT_FORCE_INLINE VkSurfaceKHR GetVulkanSurface() const { return Surface; }
	TRNT_FORCE_INLINE VkSwapchainKHR GetVulkanSwapChain() const { return SwapChain; }
	TRNT_FORCE_INLINE TVulkanRenderPass* GetRenderPass() const { return RenderPass; }
	TRNT_FORCE_INLINE VkFormat GetSwapChainDepthFormat() const { return DepthFormat; }
	TRNT_FORCE_INLINE VkSurfaceFormatKHR GetSwapChainSurfaceFormat() const { return SwapChainSurfaceFormat; }

	TRNT_FORCE_INLINE TUInt32 GetWidth() const { return Width; }
	TRNT_FORCE_INLINE TUInt32 GetHeight() const { return Height; }

	TRNT_FORCE_INLINE VkFramebuffer* GetSwapChainFrameBufferAt(TInt64 Index) { return &SwapChainFrameBuffers[Index]; }
	TRNT_FORCE_INLINE TInt64 GetSwapChainImageCount() const { return SwapChainImages.GetElementCount(); };
	TRNT_FORCE_INLINE TInt32 GetCurrentImageIndex() const { return CurrentImageIndex; };

	VkResult AcquireNextImage(TUInt32* ImageIndex);
	VkResult Present();

	void SubmitCommandBuffer(TVulkanCommandBuffer& CmdBuffer);

private:
	VkSurfaceFormatKHR ChooseSuitableSurfaceFormat(const TDynamicArray<VkSurfaceFormatKHR>& SurfaceFormats, VkColorSpaceKHR DesiredColorSpace);
	VkPresentModeKHR ChoosePresentMode(const TDynamicArray<VkPresentModeKHR>& PresentModes);
	VkFormat FindDepthFormat();
	TBool CreateRenderPass();
	TBool CreateVulkanImageViews(VkFormat Format);
	TBool CreateVulkanDepthImageViews(VkFormat DepthFormat);
	TBool CreateSwapChainFrameBuffers();
	TBool CreateSynchronousObjects();

	VkSwapchainKHR SwapChain;
	VkSurfaceKHR Surface;
	TUInt32 Width;
	TUInt32 Height;
	TInt32 CurrentImageIndex;

	VkSurfaceFormatKHR SwapChainSurfaceFormat;
	VkFormat DepthFormat;

	TDynamicArray<VkImage> SwapChainImages;
	TDynamicArray<VkImageView> SwapChainImageViews;

	TDynamicArray<VkImage> SwapChainDepthImages;
	TDynamicArray<VkImageView> SwapChainDepthImageViews;
	TDynamicArray<VkDeviceMemory> SwapChainDepthImageMemories;

	TDynamicArray<VkFramebuffer> SwapChainFrameBuffers;

	TDynamicArray<VkSemaphore> ImageAcquiredSemaphores;
	TDynamicArray<VkSemaphore> RenderFinishedSemaphores;
	TDynamicArray<VkFence> ImageAcquiredFences;

	TVulkanRenderPass* RenderPass;
	TVulkanRHI* VulkanRHIPointer;
	TVulkanDevice* VulkanDevicePointer;
	TWindow* WindowHandle;
};

#endif