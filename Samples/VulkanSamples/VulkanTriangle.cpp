#include "VulkanTriangle.h"

TVulkanTriangleApp::TVulkanTriangleApp() : TApplication("Vulkan Triangle")
{
}

TVulkanTriangleApp::~TVulkanTriangleApp()
{
}

void TVulkanTriangleApp::OnInitialize()
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

	Shader = new TVulkanShader(VulkanRHI->GetVulkanDevice(), "Vulkan Triangle");
	Shader->LoadFromFile(TShaderStage::EVertex, TRNT_ROOT_DIRECTORY "/Samples/Shaders/VulkanTriangle.tvsh");
	Shader->LoadFromFile(TShaderStage::EFragment, TRNT_ROOT_DIRECTORY "/Samples/Shaders/VulkanTriangle.tfsh");

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

	const TInt64 ImageCount = SwapChain->GetSwapChainImageCount();
	CmdBuffers.Reserve(ImageCount);

	for (TInt64 ImgIndex = 0; ImgIndex < ImageCount; ++ImgIndex)
	{
		TVulkanCommandBuffer& Ref = CmdBuffers.EmplaceBackGetRef(VulkanRHI->GetVulkanDevice(), CmdPool, false);
		Ref.Allocate();
	}
}

void TVulkanTriangleApp::OnUpdate(const TDouble DeltaTime)
{
	if (Window->IsClosed())
	{
		QuitRequested = true;
	}

	Window->ProcessInput();
	OnDrawFrame(DeltaTime);
}

void TVulkanTriangleApp::OnDrawFrame(const TDouble DeltaTime)
{
	TUInt32 ImageIndex = 0;
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
	CmdBuffers[CurrentImgIndex].Draw(3, 1, 0, 0);
	CmdBuffers[CurrentImgIndex].EndRenderPass();
	CmdBuffers[CurrentImgIndex].End();

	SwapChain->SubmitCommandBuffer(CmdBuffers[CurrentImgIndex]);
	SwapChain->Present();
}

void TVulkanTriangleApp::OnQuit()
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
