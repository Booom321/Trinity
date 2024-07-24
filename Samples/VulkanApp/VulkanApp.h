#pragma once

#ifdef TRNT_SUPPORT_VULKAN_RHI

#include <Trinity/Application/Application.h>
#include <Trinity/Window/Window.h>
#include <Trinity/Input/InputManager.h>

#include <Trinity/RHI/Vulkan/VulkanRHI.h>
#include <Trinity/RHI/Vulkan/VulkanShader.h>
#include <Trinity/RHI/Vulkan/VulkanPipeline.h>
#include <Trinity/RHI/Vulkan/VulkanSwapChain.h>
#include <Trinity/RHI/Vulkan/VulkanCommandPool.h>
#include <Trinity/RHI/Vulkan/VulkanCommandBuffer.h>

class TVulkanApp : public TApplication
{
public:
	TVulkanApp();
	
	~TVulkanApp();

public:
	void OnInitialize() override;

	void OnUpdate(const TDouble DeltaTime) override;

	void OnDrawFrame(const TDouble DeltaTime);

	void OnQuit() override;

private:
	TWindow* Window = nullptr;
	TVulkanRHI* VulkanRhi = nullptr;
	TVulkanSwapChain* SwapChain = nullptr;
	TVulkanShader* Shader = nullptr;
	TVulkanGraphicsPipeline* GraphicsPipeline = nullptr;
	TVulkanRenderPass* RenderPass = nullptr;
	TVulkanCommandPool* CmdPool = nullptr;
	TDynamicArray<TVulkanCommandBuffer> CmdBuffers;
};

#endif