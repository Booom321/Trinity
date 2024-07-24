#pragma once

#if defined(TRNT_SUPPORT_VULKAN_RHI)

#include "IncludeVulkanHeader.h"

#include "VulkanDevice.h"

class TRNT_API TVulkanCommandPool
{
public:
	TVulkanCommandPool(TVulkanDevice* VulkanDevice, TUInt32 QueueFamilyIndex);

	~TVulkanCommandPool();

public:
	TRNT_FORCE_INLINE VkCommandPool GetCommandPoolHandle() const { return CommandPool; }

private:
	TVulkanDevice* VulkanDevice;

	VkCommandPool CommandPool;
};

#endif