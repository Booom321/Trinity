#pragma once

#if defined(TRNT_SUPPORT_VULKAN_RHI)

#include "IncludeVulkanHeader.h"

#include "VulkanDevice.h"
#include "VulkanCommandPool.h"

class TVulkanBuffer;
class TVulkanRenderPass;
class TVulkanPipelineBase;

class TRNT_API TVulkanCommandBuffer 
{
public:
	enum class TRenderPassState : unsigned char
	{
		EIdle,
		EBegan,
		EEnded,
	};

	enum class TState : unsigned char
	{
		EIdle,
		EAllocated,
		EBegan,
		EEnded,
		ESubmitted,
		EFreed,
		ERequestReset
	};

	TVulkanCommandBuffer(TVulkanDevice* VulkanDevice, TVulkanCommandPool* VulkanCmdPool, TBool Secondary);
	~TVulkanCommandBuffer();

	TBool Allocate();

	void Free();
	void Begin();
	void BeginSecondary(TVulkanRenderPass* VulkanRenderPass, VkFramebuffer* Framebuffer);
	void End();

	void BeginRenderPass(TVulkanRenderPass* VulkanRenderPass, VkFramebuffer* Framebuffer, TUInt32 Width, TUInt32 Height);
	void EndRenderPass();

	void Reset();
	void SubmitCommandBuffer(TVulkanQueue Queue, VkPipelineStageFlags PipelineStage, VkSemaphore WaitSemaphore, VkSemaphore SignalSemaphore, VkFence Fence);
	void SetViewport(TUInt32 Width, TUInt32 Height, TFloat MinDepth = 0.0f, TFloat MaxDepth = 1.0f);

public:
	void BindPipeline(VkPipelineBindPoint PipelineBindPoint, TVulkanPipelineBase* Pipeline);
	void Draw(TUInt32 VertexCount, TUInt32 InstanceCount, TUInt32 FirstVertex, TUInt32 FirstInstance);
	void BindVertexBuffer(TUInt32 FirstBinding, TVulkanBuffer* VertexBuffers, const VkDeviceSize* Offsets);
	void BindVertexBuffers(
		TUInt32 FirstBinding, TUInt32 BindingCount, const TDynamicArray<TVulkanBuffer>& VertexBuffers, const VkDeviceSize* Offsets);
	void DrawIndexed(TUInt32 IndexCount, TUInt32 InstanceCount, TUInt32 FirstIndex, TInt32 VertexOffset, TUInt32 FirstInstance);
	void BindIndexBuffers(TVulkanBuffer* IndexBuffer, VkDeviceSize Offset, VkIndexType IndexType);
	void CopyBuffer(TVulkanBuffer* SrcBuffer, TVulkanBuffer* DestBuffer, VkDeviceSize Size);

	static TVulkanCommandBuffer* BeginSingleTimeCommands(TVulkanDevice* Device, TVulkanCommandPool* CommandPool);
	static void EndSingleTimeCommands(TVulkanDevice* Device, TVulkanCommandBuffer* CommandBuffer);

	TRNT_FORCE_INLINE TBool IsSecondary() const { return Secondary; }
	TRNT_FORCE_INLINE TState GetCommandBufferState() const { return CommandBufferState; }
	TRNT_FORCE_INLINE TRenderPassState GetRenderPassState() const { return RenderPassState; }
	TRNT_FORCE_INLINE VkCommandBuffer GetCommandBufferHandle() const { return CommandBuffer; }

private:
	VkCommandBuffer CommandBuffer;

	TVulkanDevice* VulkanDevice;
	TVulkanCommandPool* VulkanCmdPool;

	TBool Secondary;
	TState CommandBufferState;
	TRenderPassState RenderPassState;
};

#endif