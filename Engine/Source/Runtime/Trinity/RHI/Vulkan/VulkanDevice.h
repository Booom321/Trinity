#pragma once

#if defined(TRNT_USE_VULKAN_RHI)

#include "Trinity/Core/Defines.h"
#include "Trinity/Core/Types/DataTypes.h"

#include "VulkanRHI.h"

enum class TVulkanQueueFlags
{
	EGraphicsQueue = 0,
	EComputeQueue = 1,
	ETransferQueue = 2,
	ESparseBindingQueue = 3,
	EDecodeVideoQueue = 4,
	EEncodeVideoQueue = 5,
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
	TVulkanDevice(const TVulkanPhysicalDevice& PhysicalDevice, TVulkanRHI* VulkanRHIPointer);
	~TVulkanDevice();

	TBool Initialize();
	void Destroy();

public:
	TRNT_NODISCARD TRNT_INLINE VkDevice GetDevice() const { return Device; }
	
	TRNT_NODISCARD TRNT_INLINE VkPhysicalDeviceMemoryProperties& GetMemoryProperties() { return MemoryProperties; }
	TRNT_NODISCARD TRNT_INLINE const VkPhysicalDeviceMemoryProperties& GetMemoryProperties() const { return MemoryProperties; }

	TRNT_NODISCARD TRNT_INLINE VkPhysicalDevice GetPhysicalDeviceHandle() const { return PhysicalDevice.Handle; }

	TRNT_NODISCARD TRNT_INLINE const TVulkanQueue* GetVulkanQueuesData() const { return Queues; }
	TRNT_NODISCARD TRNT_INLINE TUInt32 GetTimestampValidBits() const { return TimestampValidBits; }

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