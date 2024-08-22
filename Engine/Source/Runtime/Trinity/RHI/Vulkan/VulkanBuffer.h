#pragma once

#if defined(TRNT_SUPPORT_VULKAN_RHI)

#include "VulkanRHI.h"

class TRNT_API TVulkanBuffer
{
public:
    TVulkanBuffer(
        TVulkanDevice* VulkanDevice,
        VkDeviceSize InstanceSize,
        TUInt32 InstanceCount,
        VkBufferUsageFlags BufferUsage,
        VkMemoryPropertyFlags MemoryPropertyFlags,
        VkDeviceSize MinOffsetAlignment = 1);

    ~TVulkanBuffer();

    TBool Initialize();
    TBool Reinitialize(
        VkDeviceSize InstanceSize,
        TUInt32 InstanceCount,
        VkBufferUsageFlags BufferUsage,
        VkMemoryPropertyFlags MemoryPropertyFlags,
        VkDeviceSize MinOffsetAlignment = 1);

    void Destroy();

    VkResult MapMemory(VkDeviceSize Size = VK_WHOLE_SIZE, VkDeviceSize Offset = 0);
    void UnmapMemory();
    VkResult Flush(VkDeviceSize Size = VK_WHOLE_SIZE, VkDeviceSize Offset = 0);
    VkResult Invalidate(VkDeviceSize Size = VK_WHOLE_SIZE, VkDeviceSize Offset = 0);
    VkDescriptorBufferInfo GetDescriptorBufferInfo(VkDeviceSize Size = VK_WHOLE_SIZE, VkDeviceSize Offset = 0);

    void WriteDataToBuffer(const void* NewData, VkDeviceSize Size = VK_WHOLE_SIZE, VkDeviceSize Offset = 0);

public:
    TRNT_FORCE_INLINE VkBuffer GetBuffer() { return Buffer; }
    TRNT_FORCE_INLINE VkBuffer GetBuffer() const { return Buffer; }
    TRNT_FORCE_INLINE VkDeviceSize GetBufferSize() const { return BufferSize; }
    TRNT_FORCE_INLINE VkDeviceSize GetAlignmentSize() const { return AlignmentSize; }
    TRNT_FORCE_INLINE VkBufferUsageFlags GetBufferUsage() const { return BufferUsage; }
    TRNT_FORCE_INLINE VkMemoryPropertyFlags GetMemoryProperty() const { return MemoryProperty; }

private:
    VkDeviceSize GetAlignment(VkDeviceSize InstanceSize, VkDeviceSize MinOffsetAlignment);

    TVulkanDevice* VulkanDevice;
    
    VkBuffer Buffer = VK_NULL_HANDLE; 
    VkDeviceMemory BufferMemory = VK_NULL_HANDLE;
    VkDeviceSize BufferSize;
    VkDeviceSize AlignmentSize;
    VkBufferUsageFlags BufferUsage;
    VkMemoryPropertyFlags MemoryProperty;
    void* Data = nullptr;
};

#endif