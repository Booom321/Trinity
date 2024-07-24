#include "TrinityPCH.h"

#if defined(TRNT_SUPPORT_VULKAN_RHI)

#include "Trinity/Core/Containers/StaticArray.h"

#include "VulkanCommandBuffer.h"
#include "VulkanRenderPass.h"
#include "VulkanUtils.h"

TVulkanCommandBuffer::TVulkanCommandBuffer(TVulkanDevice* VulkanDevice, TVulkanCommandPool* VulkanCmdPool, TBool Secondary)	
	:	CommandBuffer(VK_NULL_HANDLE), 
		VulkanDevice(VulkanDevice), 
		VulkanCmdPool(VulkanCmdPool),
		Secondary(Secondary),
		CommandBufferState(TState::Idle)
{
}

TVulkanCommandBuffer::~TVulkanCommandBuffer()
{
}

TBool TVulkanCommandBuffer::Allocate()
{
	VkCommandBufferAllocateInfo CmdBufferAllocateInfo;
	TMemory::Memset(&CmdBufferAllocateInfo, 0, sizeof(CmdBufferAllocateInfo));
	CmdBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	CmdBufferAllocateInfo.commandPool = VulkanCmdPool->GetCommandPoolHandle();
	CmdBufferAllocateInfo.level = (Secondary) ? VK_COMMAND_BUFFER_LEVEL_SECONDARY : VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	CmdBufferAllocateInfo.commandBufferCount = 1;

	const TChar* SecondaryAsCString = (Secondary) ? "Secondary" : "Primary";

	if (TVulkanRHI::VulkanPFNFunctions.AllocateCommandBuffers(VulkanDevice->GetDevice(), &CmdBufferAllocateInfo, &CommandBuffer) != VK_SUCCESS)
	{
		TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>("Failed to allocate Vulkan command buffer with level \"{}\"", SecondaryAsCString);
		return false;
	}
	TLog::Success<TRNT_GET_LOG_INFO(VulkanRHI)>("Allocated Vulkan command buffer with level \"{}\" successfully", SecondaryAsCString);
	CommandBufferState = TState::Allocated;

	return true;
}

void TVulkanCommandBuffer::Free()
{
	if (CommandBuffer == VK_NULL_HANDLE)
	{
		return;
	}

	TVulkanRHI::VulkanPFNFunctions.FreeCommandBuffers(VulkanDevice->GetDevice(), VulkanCmdPool->GetCommandPoolHandle(), 1, &CommandBuffer);
	CommandBuffer = VK_NULL_HANDLE;

	CommandBufferState = TState::Freed;
}

void TVulkanCommandBuffer::Begin()
{
	TRNT_ASSERT_MESSAGE(!Secondary, "Only primary command buffer can be called by BeginSecondary().");

	if (CommandBufferState == TState::RequestReset)
	{
		TRNT_CHECK_VULKAN_RESULT(TVulkanRHI::VulkanPFNFunctions.ResetCommandBuffer(CommandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT));
	}
	else
	{
		TRNT_ASSERT_MESSAGE(CommandBufferState != TState::Began, "Can not call Begin() while already in progress.");
	}

	VkCommandBufferBeginInfo CommandBufferBeginInfo;
	TMemory::Memset(&CommandBufferBeginInfo, 0, sizeof(CommandBufferBeginInfo));
	CommandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	CommandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	TRNT_CHECK_VULKAN_RESULT(TVulkanRHI::VulkanPFNFunctions.BeginCommandBuffer(CommandBuffer, &CommandBufferBeginInfo));
	CommandBufferState = TState::Began;
}

void TVulkanCommandBuffer::BeginSecondary(TVulkanRenderPass* VulkanRenderPass, VkFramebuffer* Framebuffer)
{
	TRNT_ASSERT_MESSAGE(Secondary, "Only secondary command buffer can be called by BeginSecondary().");

	if (CommandBufferState == TState::RequestReset)
	{
		TRNT_CHECK_VULKAN_RESULT(TVulkanRHI::VulkanPFNFunctions.ResetCommandBuffer(CommandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT));
	}
	else
	{
		TRNT_ASSERT_MESSAGE(CommandBufferState != TState::Began, "Can not call Begin() while command buffer already in progress.");
	}

	VkCommandBufferInheritanceInfo CmdBufferInheritanceInfo;
	TMemory::Memset(&CmdBufferInheritanceInfo, 0, sizeof(CmdBufferInheritanceInfo));
	CmdBufferInheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
	CmdBufferInheritanceInfo.subpass = 0;
	CmdBufferInheritanceInfo.renderPass = VulkanRenderPass->GetVulkanRenderPass();
	CmdBufferInheritanceInfo.framebuffer = *Framebuffer;
	CmdBufferInheritanceInfo.occlusionQueryEnable = VK_FALSE;
	CmdBufferInheritanceInfo.queryFlags = 0;
	CmdBufferInheritanceInfo.pipelineStatistics = 0;

	VkCommandBufferBeginInfo CommandBufferBeginInfo;
	TMemory::Memset(&CommandBufferBeginInfo, 0, sizeof(CommandBufferBeginInfo));
	CommandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	CommandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
	CommandBufferBeginInfo.pInheritanceInfo = &CmdBufferInheritanceInfo;

	TRNT_CHECK_VULKAN_RESULT(TVulkanRHI::VulkanPFNFunctions.BeginCommandBuffer(CommandBuffer, &CommandBufferBeginInfo));
	CommandBufferState = TState::Began;
}

void TVulkanCommandBuffer::End()
{
	TRNT_ASSERT_MESSAGE(CommandBufferState != TState::Ended, "Can not call End() while command buffer is already ended.");
	TRNT_CHECK_VULKAN_RESULT(TVulkanRHI::VulkanPFNFunctions.EndCommandBuffer(CommandBuffer));

	CommandBufferState = TState::Ended;
}

void TVulkanCommandBuffer::BeginRenderPass(TVulkanRenderPass* VulkanRenderPass, VkFramebuffer* Framebuffer, TUInt32 Width, TUInt32 Height)
{
	TRNT_ASSERT_MESSAGE(CommandBufferState != TState::Ended, "Can not call BeginRenderPass() while command buffer is already ended.");

	TStaticArray<VkClearValue, 2> ClearValues{};
	ClearValues[0].color = { 0.01f, 0.01f, 0.01f, 1.0f };
	ClearValues[1].depthStencil = { 1.0f, 0 };
	

	VkRenderPassBeginInfo RenderPassBeginInfo;
	TMemory::Memset(&RenderPassBeginInfo, 0, sizeof(RenderPassBeginInfo));
	RenderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	RenderPassBeginInfo.renderPass = VulkanRenderPass->GetVulkanRenderPass();
	RenderPassBeginInfo.framebuffer = *Framebuffer;
	RenderPassBeginInfo.renderArea.offset = { 0,0 };
	RenderPassBeginInfo.renderArea.extent = { Width, Height };
	RenderPassBeginInfo.clearValueCount = static_cast<TUInt32>(ClearValues.GetElementCount());
	RenderPassBeginInfo.pClearValues = ClearValues.GetData();

	if (VulkanDevice->VulkanRHIPointer->VulkanRHIFeatures.SupportsCreateRenderPass2KHR)
	{
		VkSubpassBeginInfo SubpassBeginInfo;
		TMemory::Memset(&SubpassBeginInfo, 0, sizeof(SubpassBeginInfo));
		SubpassBeginInfo.sType = VK_STRUCTURE_TYPE_SUBPASS_BEGIN_INFO;
		SubpassBeginInfo.contents = VK_SUBPASS_CONTENTS_INLINE;
		TVulkanRHI::VulkanPFNFunctions.CmdBeginRenderPass2KHR(CommandBuffer, &RenderPassBeginInfo, &SubpassBeginInfo);
	}
	else
	{
		TVulkanRHI::VulkanPFNFunctions.CmdBeginRenderPass(CommandBuffer, &RenderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
	}
}

void TVulkanCommandBuffer::EndRenderPass()
{
	TRNT_ASSERT_MESSAGE(CommandBufferState != TState::Ended, "Can not call EndRenderPass() while command buffer is already ended.");

	if (VulkanDevice->VulkanRHIPointer->VulkanRHIFeatures.SupportsCreateRenderPass2KHR)
	{
		VkSubpassEndInfo SubpassEndInfo;
		TMemory::Memset(&SubpassEndInfo, 0, sizeof(SubpassEndInfo));
		SubpassEndInfo.sType = VK_STRUCTURE_TYPE_SUBPASS_END_INFO;
		TVulkanRHI::VulkanPFNFunctions.CmdEndRenderPass2KHR(CommandBuffer, &SubpassEndInfo);
	}
	else
	{
		TVulkanRHI::VulkanPFNFunctions.CmdEndRenderPass(CommandBuffer);
	}
}

void TVulkanCommandBuffer::Reset()
{
	TRNT_CHECK_VULKAN_RESULT(TVulkanRHI::VulkanPFNFunctions.ResetCommandBuffer(CommandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT));
	return;
}

void TVulkanCommandBuffer::SubmitCommandBuffer(TVulkanQueue Queue, VkPipelineStageFlags PipelineStage, VkSemaphore WaitSemaphore, VkSemaphore SignalSemaphore, VkFence Fence)
{
	TRNT_ASSERT_MESSAGE(CommandBufferState == TState::Ended, "Can not call SubmitCommandBuffer() while command buffer is already ended.");
	TRNT_ASSERT_MESSAGE(!Secondary, "function SubmitCommandBuffer() can only be called for primary command buffer!");

	VkSubmitInfo SubmitInfo{};
	SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	SubmitInfo.waitSemaphoreCount = 1;
	SubmitInfo.pWaitSemaphores = &WaitSemaphore;
	SubmitInfo.signalSemaphoreCount = 1;
	SubmitInfo.pSignalSemaphores = &SignalSemaphore;

	SubmitInfo.pWaitDstStageMask = &PipelineStage;
	SubmitInfo.commandBufferCount = 1;
	SubmitInfo.pCommandBuffers = &CommandBuffer;

	TRNT_CHECK_VULKAN_RESULT(TVulkanRHI::VulkanPFNFunctions.WaitForFences(VulkanDevice->GetDevice(), 1, &Fence, VK_TRUE, UINT64_MAX));
	TRNT_CHECK_VULKAN_RESULT(TVulkanRHI::VulkanPFNFunctions.ResetFences(VulkanDevice->GetDevice(), 1, &Fence));

	if (TVulkanRHI::VulkanPFNFunctions.QueueSubmit(Queue.QueueHandle, 1, &SubmitInfo, Fence) != VK_SUCCESS)
	{
		TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>("Failed to submit command buffer! [Family index: {}, Pipeline stage: {}]", Queue.QueueFamilyIndex, static_cast<TUInt32>(PipelineStage));
	}

	TVulkanRHI::VulkanPFNFunctions.QueueWaitIdle(Queue.QueueHandle);
	CommandBufferState = TState::Submitted;
}

void TVulkanCommandBuffer::SetViewport(TUInt32 Width, TUInt32 Height, TFloat MinDepth, TFloat MaxDepth)
{
	VkViewport Viewport{};
	Viewport.x = 0.0f;
	Viewport.y = 0.0f;
	Viewport.width = static_cast<TFloat>(Width);
	Viewport.height = static_cast<TFloat>(Height);
	Viewport.minDepth = MinDepth;
	Viewport.maxDepth = MaxDepth;

	VkRect2D Scissor{};
	Scissor.offset = { 0, 0 };
	Scissor.extent = { Width, Height };

	vkCmdSetViewport(CommandBuffer, 0, 1, &Viewport);
	vkCmdSetScissor(CommandBuffer, 0, 1, &Scissor);
}

#endif