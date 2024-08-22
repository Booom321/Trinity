#pragma once

#ifdef TRNT_SUPPORT_VULKAN_RHI

#include <Trinity/Application/Application.h>
#include <Trinity/Window/Window.h>
#include <Trinity/Input/InputManager.h>

#include <Trinity/RHI/Vulkan/VulkanRHI.h>
#include <Trinity/RHI/Vulkan/VulkanShader.h>
#include <Trinity/RHI/Vulkan/VulkanBuffer.h>
#include <Trinity/RHI/Vulkan/VulkanPipeline.h>
#include <Trinity/RHI/Vulkan/VulkanSwapChain.h>
#include <Trinity/RHI/Vulkan/VulkanCommandPool.h>
#include <Trinity/RHI/Vulkan/VulkanCommandBuffer.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class TVertex
{
public:
	glm::vec2 Position;
	glm::vec3 Color;
};

class TVulkanRectangleApp : public TApplication
{
public:
	TVulkanRectangleApp();

	~TVulkanRectangleApp();

public:
	void OnInitialize() override;

	void OnUpdate(const TDouble DeltaTime) override;

	void OnDrawFrame(const TDouble DeltaTime);

	void OnQuit() override;

private:
	TWindow* Window = nullptr;
	TVulkanRHI* VulkanRHI = nullptr;
	TVulkanSwapChain* SwapChain = nullptr;
	TVulkanShader* Shader = nullptr;
	TVulkanGraphicsPipeline* GraphicsPipeline = nullptr;
	TVulkanRenderPass* RenderPass = nullptr;
	TVulkanCommandPool* CmdPool = nullptr;
	TVulkanBuffer* VertexBuffer = nullptr;
	TVulkanBuffer* IndexBuffer = nullptr;
	TDynamicArray<TVulkanCommandBuffer> CmdBuffers;

	const TDynamicArray<TVertex> Vertices = {
		{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
		{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
		{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
	};

	const TDynamicArray<TUInt16> Indices = { 0, 1, 2, 2, 3, 0 };
};

#endif