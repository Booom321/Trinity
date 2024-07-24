#pragma once

#if defined(TRNT_SUPPORT_VULKAN_RHI)

#include "VulkanRHI.h"

class TRNT_API TVulkanRenderPass
{
public:
	TVulkanRenderPass(TVulkanDevice* VulkanDevice);
	~TVulkanRenderPass();

	TBool Initialize(VkFormat SwapChainSurfaceFormat, VkFormat DepthFormat);
	void Destroy();

public:
	TRNT_FORCE_INLINE VkRenderPass GetVulkanRenderPass() const { return RenderPass; }

private:
	TVulkanDevice* VulkanDevice;
	VkRenderPass RenderPass;
};

#endif