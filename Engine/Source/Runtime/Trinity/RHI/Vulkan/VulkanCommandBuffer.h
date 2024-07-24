#pragma once

#if defined(TRNT_SUPPORT_VULKAN_RHI)

#include "IncludeVulkanHeader.h"

#include "VulkanDevice.h"
#include "VulkanCommandPool.h"

class TVulkanRenderPass;

class TRNT_API TVulkanCommandBuffer 
{
public:
	enum class TState : unsigned char
	{
		Idle,
		Allocated,
		Began,
		Ended,
		Submitted,
		Freed,
		RequestReset
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
	TRNT_FORCE_INLINE VkCommandBuffer GetCommandBufferHandle() const { return CommandBuffer; }
	TRNT_FORCE_INLINE TBool IsSecondary() const { return Secondary; }
	TRNT_FORCE_INLINE TState GetCommandBufferState() const { return CommandBufferState; }

private:
	VkCommandBuffer CommandBuffer;

	TVulkanDevice* VulkanDevice;
	TVulkanCommandPool* VulkanCmdPool;
	TBool Secondary;
	TState CommandBufferState;
};

#endif