#pragma once

#if defined(TRNT_SUPPORT_VULKAN_RHI)

#include "VulkanRHI.h"

#include "Trinity/Core/Containers/HashMap.h"

class TRNT_API TVulkanDescriptorSetLayout
{
public:
	TVulkanDescriptorSetLayout(TVulkanDevice* VulkanDevice);
	~TVulkanDescriptorSetLayout();

	void AddLayoutBinding(TUInt32 Binding, VkDescriptorType DescriptorType, TUInt32 DescriptorCount, VkShaderStageFlags StageFlags);
	TBool Build();
	void Destroy();

public:
	TRNT_NODISCARD TRNT_FORCE_INLINE VkDescriptorSetLayout GetDescriptorSetLayout() const { return DescriptorSetLayout; }
	TRNT_NODISCARD TRNT_FORCE_INLINE TBool Built() const { return IsBuilt; }

private:
	TBool IsBuilt = false;
	TVulkanDevice* VulkanDevice;
	VkDescriptorSetLayout DescriptorSetLayout;
	THashMap<TUInt32, VkDescriptorSetLayoutBinding> DescriptorSetLayoutBindings;
};

class TRNT_API TVulkanDescriptorPool
{
public:
	TVulkanDescriptorPool(TVulkanDevice* VulkanDevice);

private:
	TVulkanDevice* VulkanDevice;
};

#endif