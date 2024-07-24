#include "TrinityPCH.h"

#if defined(TRNT_SUPPORT_VULKAN_RHI)

#include "VulkanCommandPool.h"
#include "VulkanUtils.h"

TVulkanCommandPool::TVulkanCommandPool(TVulkanDevice* VulkanDevice, TUInt32 QueueFamilyIndex)
	: VulkanDevice(VulkanDevice), CommandPool(VK_NULL_HANDLE)
{
	VkCommandPoolCreateInfo CmdPoolCreateInfo;
	TMemory::Memset(&CmdPoolCreateInfo, 0, sizeof(CmdPoolCreateInfo));
	CmdPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	CmdPoolCreateInfo.queueFamilyIndex = QueueFamilyIndex;
	CmdPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	if (TVulkanRHI::VulkanPFNFunctions.CreateCommandPool(VulkanDevice->GetDevice(), &CmdPoolCreateInfo, nullptr, &CommandPool) != VK_SUCCESS)
	{
		TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>("Failed to create vulkan command pool (queue family index: {}).", QueueFamilyIndex);
		return;
	}

	TLog::Success<TRNT_GET_LOG_INFO(VulkanRHI)>("Created vulkan command pool successfully (queue family index: {}).", QueueFamilyIndex);
}

TVulkanCommandPool::~TVulkanCommandPool()
{
	VkDevice Device = VulkanDevice->GetDevice();

	if (CommandPool != VK_NULL_HANDLE)
	{
		TVulkanRHI::VulkanPFNFunctions.DestroyCommandPool(Device, CommandPool, nullptr);
	}
}

#endif