#include "TrinityPCH.h"

#if defined(TRNT_SUPPORT_VULKAN_RHI)

#pragma warning(push)
#pragma warning(disable: 26495)

#include "VulkanBuffer.h"
#include "VulkanDevice.h"

TVulkanBuffer::TVulkanBuffer(
    TVulkanDevice* VulkanDevice,
    VkDeviceSize InstanceSize,
    TUInt32 InstanceCount,
    VkBufferUsageFlags BufferUsage,
    VkMemoryPropertyFlags MemoryPropertyFlags,
    VkDeviceSize MinOffsetAlignment)
    :   VulkanDevice(VulkanDevice),
        AlignmentSize(GetAlignment(InstanceSize, MinOffsetAlignment)),
        BufferUsage(BufferUsage),
        MemoryProperty(MemoryPropertyFlags)
{
    BufferSize = AlignmentSize * InstanceCount;
}

TVulkanBuffer::~TVulkanBuffer()
{
}

TBool TVulkanBuffer::Initialize()
{
    VkDevice DeviceHandle = VulkanDevice->GetDevice();
    TUInt32 QueueFamilyIndices[] = {
        VulkanDevice->GetVulkanQueueByFlag(TVulkanQueueFlags::EGraphicsQueue).QueueFamilyIndex,
        VulkanDevice->GetVulkanQueueByFlag(TVulkanQueueFlags::EPresentQueue).QueueFamilyIndex,
        VulkanDevice->GetVulkanQueueByFlag(TVulkanQueueFlags::EComputeQueue).QueueFamilyIndex,
    };

    VkBufferCreateInfo BufferCreateInfo;
    TMemory::Memset(&BufferCreateInfo, 0, sizeof(BufferCreateInfo));
    BufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    BufferCreateInfo.size = BufferSize;
    BufferCreateInfo.usage = BufferUsage;
    BufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    BufferCreateInfo.pQueueFamilyIndices = QueueFamilyIndices;
    BufferCreateInfo.queueFamilyIndexCount = TRNT_GET_ARRAY_LENGTH(QueueFamilyIndices);

    if (TVulkanRHI::VulkanPFNFunctions.CreateBuffer(DeviceHandle, &BufferCreateInfo, nullptr, &Buffer) != VK_SUCCESS)
    {
        TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>("Failed to create buffer (size={}, BufferUsage={})", BufferSize, static_cast<unsigned>(BufferUsage));
        return false;
    }
    else
    {
        TLog::Success<TRNT_GET_LOG_INFO(VulkanRHI)>("Created buffer successfully (size={}, BufferUsage={})", BufferSize, static_cast<unsigned>(BufferUsage));
    }

    VkMemoryRequirements MemoryRequirements;
    vkGetBufferMemoryRequirements(DeviceHandle, Buffer, &MemoryRequirements);

    VkMemoryAllocateInfo MemoryAllocateInfo;
    TMemory::Memset(&MemoryAllocateInfo, 0, sizeof(MemoryAllocateInfo));
    MemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    MemoryAllocateInfo.allocationSize = MemoryRequirements.size;
    MemoryAllocateInfo.memoryTypeIndex = VulkanDevice->GetMemoryTypeIndex(MemoryRequirements.memoryTypeBits, MemoryProperty);

    if (TVulkanRHI::VulkanPFNFunctions.AllocateMemory(DeviceHandle, &MemoryAllocateInfo, nullptr, &BufferMemory) != VK_SUCCESS) 
    {
        TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>("Failed to allocate buffer memory (allocationSize={})", MemoryRequirements.size);
        return false;
    }
    else
    {
        TLog::Success<TRNT_GET_LOG_INFO(VulkanRHI)>("Allocated buffer memory succesfully (allocationSize={})", MemoryRequirements.size);
    }

    if (TVulkanRHI::VulkanPFNFunctions.BindBufferMemory(DeviceHandle, Buffer, BufferMemory, 0) != VK_SUCCESS)
    {
        TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>("Failed to bind buffer memory!");
        return false;
    }

    return true;
}

TBool TVulkanBuffer::Reinitialize(
    VkDeviceSize InstanceSize,
    TUInt32 InstanceCount,
    VkBufferUsageFlags BufferUsage,
    VkMemoryPropertyFlags MemoryPropertyFlags,
    VkDeviceSize MinOffsetAlignment)
{
    Destroy();
    this->AlignmentSize = GetAlignment(InstanceSize, MinOffsetAlignment);
    this->BufferSize = AlignmentSize * InstanceCount;
    this->BufferUsage = BufferUsage;
    this->MemoryProperty = MemoryPropertyFlags;

    return this->Initialize();
}

void TVulkanBuffer::Destroy()
{
    if (Buffer != VK_NULL_HANDLE)
    {
        VkDevice DeviceHandle = VulkanDevice->GetDevice();

        TVulkanRHI::VulkanPFNFunctions.DestroyBuffer(DeviceHandle, Buffer, nullptr);
        TVulkanRHI::VulkanPFNFunctions.FreeMemory(DeviceHandle, BufferMemory, nullptr);

        Buffer = VK_NULL_HANDLE;
        BufferMemory = VK_NULL_HANDLE;
    }
}

VkResult TVulkanBuffer::MapMemory(VkDeviceSize Size, VkDeviceSize Offset)
{
    return TVulkanRHI::VulkanPFNFunctions.MapMemory(VulkanDevice->GetDevice(), BufferMemory, Offset, Size, 0, &Data);
}

void TVulkanBuffer::UnmapMemory()
{
    if (Data)
    {
        TVulkanRHI::VulkanPFNFunctions.UnmapMemory(VulkanDevice->GetDevice(), BufferMemory);
        Data = nullptr;
    }
}

VkResult TVulkanBuffer::Flush(VkDeviceSize Size, VkDeviceSize Offset)
{
    VkMappedMemoryRange MappedMemoryRange{};
    MappedMemoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    MappedMemoryRange.memory = BufferMemory;
    MappedMemoryRange.offset = Offset;
    MappedMemoryRange.size = Size;
    return TVulkanRHI::VulkanPFNFunctions.FlushMappedMemoryRanges(VulkanDevice->GetDevice(), 1, &MappedMemoryRange);
}

VkResult TVulkanBuffer::Invalidate(VkDeviceSize Size, VkDeviceSize Offset)
{
    VkMappedMemoryRange MappedMemoryRange{};
    MappedMemoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    MappedMemoryRange.memory = BufferMemory;
    MappedMemoryRange.offset = Offset;
    MappedMemoryRange.size = Size;
    return TVulkanRHI::VulkanPFNFunctions.InvalidateMappedMemoryRanges(VulkanDevice->GetDevice(), 1, &MappedMemoryRange);
}

VkDescriptorBufferInfo TVulkanBuffer::GetDescriptorBufferInfo(VkDeviceSize Size, VkDeviceSize Offset)
{
    VkDescriptorBufferInfo DescriptorBufferInfo;
    DescriptorBufferInfo.buffer = Buffer;
    DescriptorBufferInfo.range = Size;
    DescriptorBufferInfo.offset = Offset;

    return DescriptorBufferInfo;
}

#pragma warning(disable: 6387)
#pragma warning(disable: 28183)

void TVulkanBuffer::WriteDataToBuffer(const void* NewData, VkDeviceSize Size, VkDeviceSize Offset)
{
    TRNT_ASSERT(NewData != nullptr);

    if (Data == nullptr)
    {
        TLog::Warning<TRNT_GET_LOG_INFO(VulkanRHI)>("Cannot copy to unmapped buffer!");
        return;
    }

    if (Size == VK_WHOLE_SIZE)
    {
        memcpy(Data, NewData, BufferSize);
    }
    else
    {
        TChar* MemOffset = (TChar*)Data;
        MemOffset += Offset;
        memcpy(MemOffset, NewData, Size);
    }
}

VkDeviceSize TVulkanBuffer::GetAlignment(VkDeviceSize InstanceSize, VkDeviceSize MinOffsetAlignment)
{
    if (MinOffsetAlignment > 0)
    {
        return (InstanceSize + MinOffsetAlignment - 1) & ~(MinOffsetAlignment - 1);
    }
    return InstanceSize;
}

#pragma warning(pop)

#endif