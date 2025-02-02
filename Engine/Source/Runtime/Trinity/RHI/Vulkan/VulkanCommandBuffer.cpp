#include "TrinityPCH.h"

#if defined(TRNT_SUPPORT_VULKAN_RHI)

	#include "Trinity/Core/Containers/StaticArray.h"
	#include "VulkanBuffer.h"
	#include "VulkanCommandBuffer.h"
	#include "VulkanPipeline.h"
	#include "VulkanRenderPass.h"
	#include "VulkanUtils.h"

TVulkanCommandBuffer::TVulkanCommandBuffer(TVulkanDevice* VulkanDevice, TVulkanCommandPool* VulkanCmdPool, TBool Secondary)
	: CommandBuffer(VK_NULL_HANDLE),
	  VulkanDevice(VulkanDevice),
	  VulkanCmdPool(VulkanCmdPool),
	  Secondary(Secondary),
	  RenderPassState(TRenderPassState::EIdle),
	  CommandBufferState(TState::EIdle)
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
	CommandBufferState = TState::EAllocated;

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

	CommandBufferState = TState::EFreed;
}

void TVulkanCommandBuffer::Begin()
{
	TRNT_ASSERT_MESSAGE("Only primary command buffer can be called by BeginSecondary().", !Secondary);

	if (CommandBufferState == TState::ERequestReset)
	{
		TRNT_CHECK_VULKAN_RESULT(TVulkanRHI::VulkanPFNFunctions.ResetCommandBuffer(CommandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT));
	}
	else
	{
		TRNT_ASSERT_MESSAGE("Can not call Begin() while already in progress.", CommandBufferState != TState::EBegan);
	}

	VkCommandBufferBeginInfo CommandBufferBeginInfo;
	TMemory::Memset(&CommandBufferBeginInfo, 0, sizeof(CommandBufferBeginInfo));
	CommandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	CommandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	TRNT_CHECK_VULKAN_RESULT(TVulkanRHI::VulkanPFNFunctions.BeginCommandBuffer(CommandBuffer, &CommandBufferBeginInfo));
	CommandBufferState = TState::EBegan;
}

void TVulkanCommandBuffer::BeginSecondary(TVulkanRenderPass* VulkanRenderPass, VkFramebuffer* Framebuffer)
{
	TRNT_ASSERT_MESSAGE("Only secondary command buffer can be called by BeginSecondary().", Secondary);

	if (CommandBufferState == TState::ERequestReset)
	{
		TRNT_CHECK_VULKAN_RESULT(TVulkanRHI::VulkanPFNFunctions.ResetCommandBuffer(CommandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT));
	}
	else
	{
		TRNT_ASSERT_MESSAGE("Can not call Begin() while command buffer already in progress.", CommandBufferState != TState::EBegan);
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
	CommandBufferState = TState::EBegan;
}

void TVulkanCommandBuffer::End()
{
	TRNT_ASSERT_MESSAGE("Can not call End() while command buffer is already ended.", CommandBufferState != TState::EEnded);
	TRNT_CHECK_VULKAN_RESULT(TVulkanRHI::VulkanPFNFunctions.EndCommandBuffer(CommandBuffer));

	CommandBufferState = TState::EEnded;
}

void TVulkanCommandBuffer::BeginRenderPass(TVulkanRenderPass* VulkanRenderPass, VkFramebuffer* Framebuffer, TUInt32 Width, TUInt32 Height)
{
	TRNT_ASSERT_MESSAGE("Can not call BeginRenderPass() while command buffer is already ended.", CommandBufferState != TState::EEnded);

	TStaticArray<VkClearValue, 2> ClearValues{};
	ClearValues[0].color = { 0.01f, 0.01f, 0.01f, 1.0f };
	ClearValues[1].depthStencil = { 1.0f, 0 };

	VkRenderPassBeginInfo RenderPassBeginInfo;
	TMemory::Memset(&RenderPassBeginInfo, 0, sizeof(RenderPassBeginInfo));
	RenderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	RenderPassBeginInfo.renderPass = VulkanRenderPass->GetVulkanRenderPass();
	RenderPassBeginInfo.framebuffer = *Framebuffer;
	RenderPassBeginInfo.renderArea.offset = { 0, 0 };
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

	RenderPassState = TRenderPassState::EBegan;
}

void TVulkanCommandBuffer::EndRenderPass()
{
	TRNT_ASSERT_MESSAGE("Can not call EndRenderPass() while command buffer is already ended.", CommandBufferState != TState::EEnded);

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

	RenderPassState = TRenderPassState::EEnded;
}

void TVulkanCommandBuffer::Reset()
{
	TRNT_CHECK_VULKAN_RESULT(TVulkanRHI::VulkanPFNFunctions.ResetCommandBuffer(CommandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT));
	return;
}

void TVulkanCommandBuffer::SubmitCommandBuffer(TVulkanQueue Queue, VkPipelineStageFlags PipelineStage, VkSemaphore WaitSemaphore, VkSemaphore SignalSemaphore, VkFence Fence)
{
	TRNT_ASSERT_MESSAGE("Can not call SubmitCommandBuffer() while command buffer is already ended.", CommandBufferState == TState::EEnded);
	TRNT_ASSERT_MESSAGE("Function SubmitCommandBuffer() can only be called for primary command buffer!", !Secondary);

	const bool WaitSemaphoreIsNull = WaitSemaphore == VK_NULL_HANDLE;
	const bool SignalSemaphoreIsNull = SignalSemaphore == VK_NULL_HANDLE;

	VkSubmitInfo SubmitInfo{};
	SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	SubmitInfo.waitSemaphoreCount = (WaitSemaphoreIsNull) ? 0 : 1;
	SubmitInfo.pWaitSemaphores = (WaitSemaphoreIsNull) ? nullptr : &WaitSemaphore;
	SubmitInfo.signalSemaphoreCount = (SignalSemaphoreIsNull) ? 0 : 1;
	SubmitInfo.pSignalSemaphores = (SignalSemaphoreIsNull) ? nullptr : &SignalSemaphore;

	SubmitInfo.pWaitDstStageMask = &PipelineStage;
	SubmitInfo.commandBufferCount = 1;
	SubmitInfo.pCommandBuffers = &CommandBuffer;

	if (Fence != VK_NULL_HANDLE)
	{
		TRNT_CHECK_VULKAN_RESULT(TVulkanRHI::VulkanPFNFunctions.WaitForFences(VulkanDevice->GetDevice(), 1, &Fence, VK_TRUE, UINT64_MAX));
		TRNT_CHECK_VULKAN_RESULT(TVulkanRHI::VulkanPFNFunctions.ResetFences(VulkanDevice->GetDevice(), 1, &Fence));
	}

	if (TVulkanRHI::VulkanPFNFunctions.QueueSubmit(Queue.QueueHandle, 1, &SubmitInfo, Fence) != VK_SUCCESS)
	{
		TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>("Failed to submit command buffer! [Family index: {}, Pipeline stage: {}]", Queue.QueueFamilyIndex, static_cast<TUInt32>(PipelineStage));
	}

	TVulkanRHI::VulkanPFNFunctions.QueueWaitIdle(Queue.QueueHandle);
	CommandBufferState = TState::ESubmitted;
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

	TVulkanRHI::VulkanPFNFunctions.CmdSetViewport(CommandBuffer, 0, 1, &Viewport);
	TVulkanRHI::VulkanPFNFunctions.CmdSetScissor(CommandBuffer, 0, 1, &Scissor);
}

void TVulkanCommandBuffer::BindPipeline(VkPipelineBindPoint PipelineBindPoint, TVulkanPipelineBase* Pipeline)
{
	TRNT_ASSERT_MESSAGE("Can not call BindPipeline() while command buffer is already ended.", CommandBufferState != TState::EEnded);
	TVulkanRHI::VulkanPFNFunctions.CmdBindPipeline(CommandBuffer, PipelineBindPoint, Pipeline->GetVulkanPipeline());
}

void TVulkanCommandBuffer::Draw(TUInt32 VertexCount, TUInt32 InstanceCount, TUInt32 FirstVertex, TUInt32 FirstInstance)
{
	TRNT_ASSERT_MESSAGE("Function Draw() must only be called inside of a render pass instance.", RenderPassState != TRenderPassState::EEnded);
	TVulkanRHI::VulkanPFNFunctions.CmdDraw(CommandBuffer, VertexCount, InstanceCount, FirstVertex, FirstInstance);
}

void TVulkanCommandBuffer::BindVertexBuffer(TUInt32 FirstBinding, TVulkanBuffer* VertexBuffer, const VkDeviceSize* Offsets)
{
	TRNT_ASSERT_MESSAGE("Function BindVertexBuffer() must only be called inside of a render pass instance.", RenderPassState != TRenderPassState::EEnded);

	VkBuffer VertexBufferHandle = VertexBuffer->GetBuffer();
	TVulkanRHI::VulkanPFNFunctions.CmdBindVertexBuffers(CommandBuffer, FirstBinding, 1, &VertexBufferHandle, Offsets);
}

void TVulkanCommandBuffer::BindVertexBuffers(
	TUInt32 FirstBinding,
	TUInt32 BindingCount,
	const TDynamicArray<TVulkanBuffer>& VertexBuffers,
	const VkDeviceSize* Offsets)
{
	TRNT_ASSERT_MESSAGE("Function BindVertexBuffers() must only be called inside of a render pass instance.", RenderPassState != TRenderPassState::EEnded);

	const TInt64 VertexBufferCount = VertexBuffers.GetElementCount();

	TDynamicArray<VkBuffer> VertexBufferHandles;
	VertexBufferHandles.Resize(VertexBufferCount);

	for (TInt64 Index = 0; Index < VertexBufferCount; ++Index)
	{
		VertexBufferHandles[Index] = VertexBuffers[Index].GetBuffer();
	}

	TVulkanRHI::VulkanPFNFunctions.CmdBindVertexBuffers(CommandBuffer, FirstBinding, BindingCount, VertexBufferHandles.GetData(), Offsets);
}

void TVulkanCommandBuffer::BindIndexBuffers(TVulkanBuffer* IndexBuffer, VkDeviceSize Offset, VkIndexType IndexType)
{
	TRNT_ASSERT_MESSAGE("Function BindIndexBuffers() must only be called inside of a render pass instance.", RenderPassState != TRenderPassState::EEnded);
	TVulkanRHI::VulkanPFNFunctions.CmdBindIndexBuffer(CommandBuffer, IndexBuffer->GetBuffer(), Offset, IndexType);
}

void TVulkanCommandBuffer::DrawIndexed(TUInt32 IndexCount, TUInt32 InstanceCount, TUInt32 FirstIndex, TInt32 VertexOffset, TUInt32 FirstInstance)
{
	TRNT_ASSERT_MESSAGE("Function BindIndexBuffers() must only be called inside of a render pass instance.", RenderPassState != TRenderPassState::EEnded);
	TVulkanRHI::VulkanPFNFunctions.CmdDrawIndexed(CommandBuffer, IndexCount, InstanceCount, FirstIndex, VertexOffset, FirstInstance);
}

TVulkanCommandBuffer* TVulkanCommandBuffer::BeginSingleTimeCommands(TVulkanDevice* Device, TVulkanCommandPool* CommandPool)
{
	TVulkanCommandBuffer* CommandBuffer = new TVulkanCommandBuffer(Device, CommandPool, false);
	TRNT_ASSERT(CommandBuffer && CommandBuffer->Allocate());

	CommandBuffer->Begin();

	return CommandBuffer;
}

void TVulkanCommandBuffer::EndSingleTimeCommands(TVulkanDevice* Device, TVulkanCommandBuffer* CommandBuffer)
{
	if (CommandBuffer)
	{
		CommandBuffer->End();
		CommandBuffer->SubmitCommandBuffer(Device->GetVulkanQueueByFlag(TVulkanQueueFlags::EGraphicsQueue), VK_PIPELINE_STAGE_NONE_KHR, VK_NULL_HANDLE, VK_NULL_HANDLE, VK_NULL_HANDLE);
		CommandBuffer->Free();

		delete CommandBuffer;
		CommandBuffer = nullptr;
	}
}

void TVulkanCommandBuffer::CopyBuffer(TVulkanBuffer* SrcBuffer, TVulkanBuffer* DestBuffer, VkDeviceSize Size)
{
	VkBufferCopy CopyRegion{};
	CopyRegion.size = Size;
	TVulkanRHI::VulkanPFNFunctions.CmdCopyBuffer(CommandBuffer, SrcBuffer->GetBuffer(), DestBuffer->GetBuffer(), 1, &CopyRegion);
}

#endif