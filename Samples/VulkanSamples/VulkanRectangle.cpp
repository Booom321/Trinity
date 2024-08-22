#include "VulkanRectangle.h"

TVulkanRectangleApp::TVulkanRectangleApp() : TApplication("Vulkan Rectangle")
{
}

TVulkanRectangleApp::~TVulkanRectangleApp()
{
}

void TVulkanRectangleApp::OnInitialize()
{
#ifdef TRNT_USE_GLFW
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#endif
	TLog::AddMessageHandlerCallback(TLog::DefaultStdoutMessageHandler);
	TInputManager::CreateInstance();

	TWindowProperties WindowProps{};
	WindowProps.Title = TApplication::ApplicationName;
	Window = new TWindow(WindowProps, true);

	VulkanRHI = new TVulkanRHI();
	VulkanRHI->Initialize();

	SwapChain = new TVulkanSwapChain(VulkanRHI, Window, Window->GetWidth(), Window->GetHeight());
	SwapChain->Initialize();

	Shader = new TVulkanShader(VulkanRHI->GetVulkanDevice(), "Vulkan Rectangle");
	Shader->LoadFromFile(TShaderStage::EVertex, TRNT_ROOT_DIRECTORY "/Samples/Shaders/VulkanRectangle.tvsh");
	Shader->LoadFromFile(TShaderStage::EFragment, TRNT_ROOT_DIRECTORY "/Samples/Shaders/VulkanRectangle.tfsh");

	RenderPass = new TVulkanRenderPass(VulkanRHI->GetVulkanDevice());
	RenderPass->Initialize(SwapChain->GetSwapChainSurfaceFormat().format, SwapChain->GetSwapChainDepthFormat());

	GraphicsPipeline = new TVulkanGraphicsPipeline(VulkanRHI->GetVulkanDevice());
	TVulkanGraphicsPipeline::TConfigInfo ConfigInfo = {};
	ConfigInfo.Shader = Shader;
	ConfigInfo.RenderPass = RenderPass;
	ConfigInfo.PrimitiveTopology = TPrimitiveTopology::ETriangleList;
	ConfigInfo.SampleCount = VK_SAMPLE_COUNT_1_BIT;
	ConfigInfo.PolygonMode = TPolygonMode::EFill;
	ConfigInfo.CullMode = TCullMode::ENone;
	ConfigInfo.BlendEnable = true;
	ConfigInfo.DepthWriteEnable = true;
	ConfigInfo.DepthTestEnable = true;
	ConfigInfo.FrontFace = TFrontFace::EClockwise;
	ConfigInfo.MinSampleShading = 1.0f;
	ConfigInfo.DepthBiasConstantFactor = 1.0f;
	ConfigInfo.DepthBiasSlopeFactor = 1.0f;
	ConfigInfo.MinSampleShading = 1.0f;
	ConfigInfo.DepthBiasEnabled = true;
	GraphicsPipeline->Initialize(ConfigInfo);
	CmdPool = new TVulkanCommandPool(
		VulkanRHI->GetVulkanDevice(),
		VulkanRHI->GetVulkanDevice()->GetVulkanQueueByFlag(TVulkanQueueFlags::EGraphicsQueue).QueueFamilyIndex);

	VertexBuffer = new TVulkanBuffer(
		VulkanRHI->GetVulkanDevice(),
		sizeof(Vertices[0]),
		static_cast<TUInt32>(Vertices.GetElementCount()),
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
	);
	VertexBuffer->Initialize();

	IndexBuffer = new TVulkanBuffer(
		VulkanRHI->GetVulkanDevice(),
		sizeof(Indices[0]),
		static_cast<TUInt32>(Indices.GetElementCount()),
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
	);
	IndexBuffer->Initialize();

	TVulkanBuffer* StagingBuffer = new TVulkanBuffer(
		VulkanRHI->GetVulkanDevice(),
		sizeof(Vertices[0]),
		static_cast<TUInt32>(Vertices.GetElementCount()),
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
	);
	StagingBuffer->Initialize();
	StagingBuffer->MapMemory(StagingBuffer->GetBufferSize());
	StagingBuffer->WriteDataToBuffer(Vertices.GetData());
	StagingBuffer->UnmapMemory();

	TVulkanCommandBuffer* CmdBuffer = TVulkanCommandBuffer::BeginSingleTimeCommands(VulkanRHI->GetVulkanDevice(), CmdPool);
	CmdBuffer->CopyBuffer(StagingBuffer, VertexBuffer, VertexBuffer->GetBufferSize());

	StagingBuffer->Reinitialize(
		sizeof(Indices[0]),
		static_cast<TUInt32>(Indices.GetElementCount()),
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	StagingBuffer->MapMemory(StagingBuffer->GetBufferSize());
	StagingBuffer->WriteDataToBuffer(Indices.GetData());
	StagingBuffer->UnmapMemory();

	CmdBuffer->CopyBuffer(StagingBuffer, IndexBuffer, IndexBuffer->GetBufferSize());

	TVulkanCommandBuffer::EndSingleTimeCommands(VulkanRHI->GetVulkanDevice(), CmdBuffer);

	StagingBuffer->Destroy();
	delete StagingBuffer;
	StagingBuffer = nullptr;

	const TInt64 ImageCount = SwapChain->GetSwapChainImageCount();
	CmdBuffers.Reserve(ImageCount);
	for (TInt64 ImgIndex = 0; ImgIndex < ImageCount; ++ImgIndex)
	{
		TVulkanCommandBuffer& Ref = CmdBuffers.EmplaceBackGetRef(VulkanRHI->GetVulkanDevice(), CmdPool, false);
		Ref.Allocate();
	}
}
void TVulkanRectangleApp::OnUpdate(const TDouble DeltaTime)
{
	if (Window->IsClosed())
	{
		QuitRequested = true;
	}

	Window->ProcessInput();
	OnDrawFrame(DeltaTime);
}

void TVulkanRectangleApp::OnDrawFrame(const TDouble DeltaTime)
{
	TUInt32 ImageIndex = 0;
	VkDeviceSize Offsets[1] = { 0 };
	const TInt32 CurrentImgIndex = SwapChain->GetCurrentImageIndex();
	VkResult Result = SwapChain->AcquireNextImage(&ImageIndex);

	if (Result == VK_ERROR_OUT_OF_DATE_KHR) { return; }

	CmdBuffers[CurrentImgIndex].Reset();
	CmdBuffers[CurrentImgIndex].Begin();
	CmdBuffers[CurrentImgIndex].BeginRenderPass(
		RenderPass,
		SwapChain->GetSwapChainFrameBufferAt(ImageIndex),
		SwapChain->GetWidth(),
		SwapChain->GetHeight());

	CmdBuffers[CurrentImgIndex].BindPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicsPipeline);
	CmdBuffers[CurrentImgIndex].SetViewport(SwapChain->GetWidth(), SwapChain->GetHeight());
	CmdBuffers[CurrentImgIndex].BindVertexBuffer(0, VertexBuffer, Offsets);
	CmdBuffers[CurrentImgIndex].BindIndexBuffers(IndexBuffer, 0, VK_INDEX_TYPE_UINT16);
	CmdBuffers[CurrentImgIndex].DrawIndexed(static_cast<TUInt32>(Indices.GetElementCount()), 1, 0, 0, 0);
	CmdBuffers[CurrentImgIndex].EndRenderPass();
	CmdBuffers[CurrentImgIndex].End();

	SwapChain->SubmitCommandBuffer(CmdBuffers[CurrentImgIndex]);
	SwapChain->Present();
}

void TVulkanRectangleApp::OnQuit()
{
	VulkanRHI->GetVulkanDevice()->WaitIdle();

	if (CmdPool)
	{
		delete CmdPool;
		CmdPool = nullptr;
	}

	if (GraphicsPipeline)
	{
		GraphicsPipeline->Destroy();
		delete GraphicsPipeline;
		GraphicsPipeline = nullptr;
	}

	if (RenderPass)
	{
		RenderPass->Destroy();
		delete RenderPass;
		RenderPass = nullptr;
	}

	if (VertexBuffer)
	{
		VertexBuffer->Destroy();
		delete VertexBuffer;
		VertexBuffer = nullptr;
	}

	if (IndexBuffer)
	{
		IndexBuffer->Destroy();
		delete IndexBuffer;
		IndexBuffer = nullptr;
	}

	if (Shader)
	{
		Shader->Destroy();
		delete Shader;
		Shader = nullptr;
	}

	if (SwapChain)
	{
		SwapChain->Destroy();
		delete SwapChain;
		SwapChain = nullptr;
	}

	if (VulkanRHI)
	{
		VulkanRHI->Shutdown();
		delete VulkanRHI;
		VulkanRHI = nullptr;
	}

	if (Window)
	{
		delete Window;
		Window = nullptr;
	}

	TInputManager::DeleteInstance();
	TLog::ClearAllMessageHandlerCallbacks();
}
