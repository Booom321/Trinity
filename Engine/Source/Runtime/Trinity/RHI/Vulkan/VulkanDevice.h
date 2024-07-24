#pragma once

#if defined(TRNT_SUPPORT_VULKAN_RHI)

#include "Trinity/Core/Defines.h"
#include "Trinity/Core/Types/DataTypes.h"

#include "VulkanRHI.h"

enum class TVulkanQueueFlags
{
	EGraphicsQueue = 0,
	EPresentQueue = 1,
	EComputeQueue = 2,
	ETransferQueue = 3,
	ESparseBindingQueue = 4,
	EDecodeVideoQueue = 5,
	EEncodeVideoQueue = 6,
	EMax
};

class TVulkanQueue
{
public:
	VkQueue QueueHandle = VK_NULL_HANDLE;
	TUInt32 QueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
};

class TRNT_API TVulkanDevice
{
public:
	friend class TVulkanCommandBuffer;

	TVulkanDevice(const TVulkanPhysicalDevice& PhysicalDevice, TVulkanRHI* VulkanRHIPointer);
	~TVulkanDevice();

	TBool Initialize();
	void Destroy();

public:
	TRNT_NODISCARD TRNT_INLINE VkDevice GetDevice() const { return Device; }
	
	TRNT_NODISCARD TRNT_INLINE VkPhysicalDeviceMemoryProperties& GetMemoryProperties() { return MemoryProperties; }
	TRNT_NODISCARD TRNT_INLINE const VkPhysicalDeviceMemoryProperties& GetMemoryProperties() const { return MemoryProperties; }

	TRNT_NODISCARD TRNT_INLINE TVulkanPhysicalDevice& GetPhysicalDevice() { return PhysicalDevice; }
	TRNT_NODISCARD TRNT_INLINE const TVulkanPhysicalDevice& GetPhysicalDevice() const { return PhysicalDevice; }

	TRNT_NODISCARD TRNT_INLINE TVulkanQueue* GetVulkanQueueArray() { return Queues; }
	TRNT_NODISCARD TRNT_INLINE TVulkanQueue& GetVulkanQueueByFlag(TVulkanQueueFlags QueueFlag) { return Queues[static_cast<TUInt32>(QueueFlag)]; }
	TRNT_NODISCARD TRNT_INLINE TUInt32 GetTimestampValidBits() const { return TimestampValidBits; }
	TRNT_NODISCARD TRNT_INLINE TVersion GetVulkanAPIVersion() const { return VulkanRHIPointer->VulkanAPIVersion; }

	TRNT_NODISCARD VkFormat FindSupportedFormat(const TDynamicArray<VkFormat>& Formats, VkImageTiling Tiling, VkFormatFeatureFlags FormatFeatures);
	TRNT_NODISCARD TBool CreateVulkanImageWithInfo(const VkImageCreateInfo& ImageCreateInfo, VkMemoryPropertyFlags MemoryProp, VkImage& Image, VkDeviceMemory& ImageMemory);
	TRNT_NODISCARD TUInt32 GetMemoryTypeIndex(TUInt32 TypeFilter, VkMemoryPropertyFlags MemoryProperty);

	void WaitIdle();

private:
	TVulkanRHI* VulkanRHIPointer;

	VkDevice Device;
	VkPhysicalDeviceMemoryProperties MemoryProperties;
	VkPhysicalDeviceFeatures EnabledPhysicalDeviceFeatures;

	void GetEnabledPhysicalDeviceFeatures();

	TVulkanPhysicalDevice PhysicalDevice;

	TDynamicArray<const TChar*> EnabledDeviceExtensions;
	TBool GetEnabledDeviceExtensions(const TDynamicArray<VkExtensionProperties>& AvailableDeviceExtensions, const TUInt32* QueueFamilyIndices);

private:
	TVulkanQueue Queues[(TUInt32)TVulkanQueueFlags::EMax];
	TUInt32 TimestampValidBits = 0;
};

#endif