#include "TrinityPCH.h"

#if defined(TRNT_SUPPORT_VULKAN_RHI)

#include "VulkanPipeline.h"
#include "VulkanUtils.h"

TVulkanPipelineBase::TVulkanPipelineBase(TVulkanDevice* VulkanDevice)
	: VulkanDevice(VulkanDevice), PipelineHandle(VK_NULL_HANDLE)
{ }

///////////////////////////////////////////////////////////////////////////////////////////////////////
///// TVulkanComputePipeline //////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

TVulkanComputePipeline::TVulkanComputePipeline(TVulkanDevice* VulkanDevice)
	: TVulkanPipelineBase(VulkanDevice)
{
}

TVulkanComputePipeline::~TVulkanComputePipeline()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///// TVulkanGraphicsPipeline /////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
TVulkanGraphicsPipeline::TVulkanGraphicsPipeline(TVulkanDevice* VulkanDevice)
	: TVulkanPipelineBase(VulkanDevice), PipelineLayout(VK_NULL_HANDLE)
{
}

TVulkanGraphicsPipeline::~TVulkanGraphicsPipeline()
{

}

TBool TVulkanGraphicsPipeline::Initialize(const TConfigInfo& PipelineConfigInfo)
{
	if (!CreatePipelineLayout(PipelineConfigInfo.Shader))
	{
		TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>("Failed to create pipeline layout!");
		return false;
	}
	else
	{
		TLog::Success<TRNT_GET_LOG_INFO(VulkanRHI)>("Created pipeline layout successfully!");
	}

	TDynamicArray<VkPipelineShaderStageCreateInfo> ShaderStageCreateInfos{};
	ShaderStageCreateInfos.Reserve(static_cast<TInt64>(TShaderStage::EMax));
	for (TInt32 StageIndex = 0, StageCount = static_cast<TInt32>(TShaderStage::EMax); StageIndex < StageCount; ++StageIndex)
	{
		TShaderStage ShaderStage = static_cast<TShaderStage>(StageIndex);
		VkShaderModule CurrentShaderModule = PipelineConfigInfo.Shader->GetShaderModuleByStage(ShaderStage);
		if (CurrentShaderModule == VK_NULL_HANDLE)
		{
			continue;
		}

		VkPipelineShaderStageCreateInfo& CurrentShaderStageCreateInfo = ShaderStageCreateInfos.EmplaceBackGetRef();
		CurrentShaderStageCreateInfo.sType	= VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		CurrentShaderStageCreateInfo.stage	= TVulkanUtils::ConvertTShaderStageToVkStage(ShaderStage);
		CurrentShaderStageCreateInfo.module = CurrentShaderModule;
		CurrentShaderStageCreateInfo.pName	= "main";
		CurrentShaderStageCreateInfo.pNext	= nullptr;
	}

	VkPipelineInputAssemblyStateCreateInfo InputAssemblyStateCreateInfo;
	TMemory::Memset(&InputAssemblyStateCreateInfo, 0, sizeof(InputAssemblyStateCreateInfo));
	InputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	InputAssemblyStateCreateInfo.pNext = nullptr;
	InputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;
	InputAssemblyStateCreateInfo.topology = TVulkanUtils::ConvertTPrimitiveTopologyToVkPrimitiveTopology(PipelineConfigInfo.PrimitiveTopology);

	const TDynamicArray<VkVertexInputAttributeDescription>& VertexInpAttributeDescriptions = PipelineConfigInfo.Shader->GetVertexInputAttributeDescriptions();
	TUInt32 VertexInputStride = PipelineConfigInfo.Shader->GetVertexInputStride();

	VkVertexInputBindingDescription VertexInputBindingDescription;
	TMemory::Memset(&VertexInputBindingDescription, 0, sizeof(VertexInputBindingDescription));
	VertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	VertexInputBindingDescription.stride = VertexInputStride;
	VertexInputBindingDescription.binding = 0;

	VkPipelineVertexInputStateCreateInfo VertexInpStateCreateInfo;
	TMemory::Memset(&VertexInpStateCreateInfo, 0, sizeof(VertexInpStateCreateInfo));
	VertexInpStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	VertexInpStateCreateInfo.vertexBindingDescriptionCount = (VertexInputStride > 0) ? 1 : 0;
	VertexInpStateCreateInfo.pVertexBindingDescriptions = (VertexInputStride > 0) ? &VertexInputBindingDescription : nullptr;
	VertexInpStateCreateInfo.vertexAttributeDescriptionCount = (VertexInputStride > 0) ? static_cast<TUInt32>(VertexInpAttributeDescriptions.GetElementCount()) : 0;
	VertexInpStateCreateInfo.pVertexAttributeDescriptions = (VertexInputStride > 0) ? VertexInpAttributeDescriptions.GetData() : nullptr;
	VertexInpStateCreateInfo.pNext = nullptr;

	VkPipelineViewportStateCreateInfo ViewportStateCreateInfo;
	TMemory::Memset(&ViewportStateCreateInfo, 0, sizeof(ViewportStateCreateInfo));
	ViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	ViewportStateCreateInfo.viewportCount = 1;
	ViewportStateCreateInfo.scissorCount = 1;
	ViewportStateCreateInfo.pNext = nullptr;
	ViewportStateCreateInfo.pViewports = nullptr;
	ViewportStateCreateInfo.pScissors= nullptr;

	TDynamicArray<VkDynamicState> DynamicStates{ VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR, VK_DYNAMIC_STATE_STENCIL_REFERENCE , VK_DYNAMIC_STATE_DEPTH_BOUNDS };
	VkPipelineDynamicStateCreateInfo DynamicStateCreateInfo;
	TMemory::Memset(&DynamicStateCreateInfo, 0, sizeof(DynamicStateCreateInfo));
	DynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	DynamicStateCreateInfo.pNext = nullptr;
	DynamicStateCreateInfo.dynamicStateCount = static_cast<TUInt32>(DynamicStates.GetElementCount());
	DynamicStateCreateInfo.pDynamicStates = DynamicStates.GetData();

	VkPipelineDepthStencilStateCreateInfo DepthStencilStateCreateInfo;
	TMemory::Memset(&DepthStencilStateCreateInfo, 0, sizeof(DepthStencilStateCreateInfo));
	DepthStencilStateCreateInfo.sType					= VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	DepthStencilStateCreateInfo.pNext					= nullptr;
	DepthStencilStateCreateInfo.depthTestEnable			= PipelineConfigInfo.DepthTestEnable ? VK_TRUE : VK_FALSE;
	DepthStencilStateCreateInfo.depthWriteEnable		= PipelineConfigInfo.DepthWriteEnable ? VK_TRUE : VK_FALSE;
	DepthStencilStateCreateInfo.depthCompareOp			= VK_COMPARE_OP_LESS_OR_EQUAL;
	DepthStencilStateCreateInfo.depthBoundsTestEnable	= VK_FALSE;
	DepthStencilStateCreateInfo.stencilTestEnable		= VK_FALSE;
	DepthStencilStateCreateInfo.back.failOp				= VK_STENCIL_OP_KEEP;
	DepthStencilStateCreateInfo.back.passOp				= VK_STENCIL_OP_KEEP;
	DepthStencilStateCreateInfo.back.compareOp			= VK_COMPARE_OP_ALWAYS;
	DepthStencilStateCreateInfo.back.compareMask		= 0;
	DepthStencilStateCreateInfo.back.reference			= 0;
	DepthStencilStateCreateInfo.back.depthFailOp		= VK_STENCIL_OP_KEEP;
	DepthStencilStateCreateInfo.back.writeMask			= 0;
	DepthStencilStateCreateInfo.minDepthBounds			= 0.0f;
	DepthStencilStateCreateInfo.maxDepthBounds			= 1.0f;
	DepthStencilStateCreateInfo.front					= DepthStencilStateCreateInfo.back;

	VkPipelineRasterizationStateCreateInfo RasterizationStateCreateInfo;
	TMemory::Memset(&RasterizationStateCreateInfo, 0, sizeof(RasterizationStateCreateInfo));
	RasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	RasterizationStateCreateInfo.frontFace = TVulkanUtils::ConvertTFrontFaceToVkFrontFace(PipelineConfigInfo.FrontFace);
	RasterizationStateCreateInfo.depthClampEnable = VK_TRUE;
	RasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
	RasterizationStateCreateInfo.depthBiasEnable = PipelineConfigInfo.DepthBiasEnabled ? VK_TRUE : VK_FALSE;
	RasterizationStateCreateInfo.depthBiasConstantFactor = PipelineConfigInfo.DepthBiasConstantFactor;
	RasterizationStateCreateInfo.depthBiasClamp = 0;
	RasterizationStateCreateInfo.depthBiasSlopeFactor = PipelineConfigInfo.DepthBiasSlopeFactor;
	RasterizationStateCreateInfo.cullMode = TVulkanUtils::ConvertTCullModeToVkCullMode(PipelineConfigInfo.CullMode);
	RasterizationStateCreateInfo.polygonMode = TVulkanUtils::ConvertTPolygonModeToVkPolygonMode(PipelineConfigInfo.PolygonMode);

	VkPipelineMultisampleStateCreateInfo MultisampleStateCreateInfo;
	TMemory::Memset(&MultisampleStateCreateInfo, 0, sizeof(MultisampleStateCreateInfo));
	MultisampleStateCreateInfo.sType					= VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	MultisampleStateCreateInfo.pNext					= nullptr;
	MultisampleStateCreateInfo.pSampleMask				= nullptr;
	MultisampleStateCreateInfo.rasterizationSamples		= PipelineConfigInfo.SampleCount;
	MultisampleStateCreateInfo.sampleShadingEnable		= VK_FALSE;
	MultisampleStateCreateInfo.alphaToCoverageEnable	= VK_FALSE;
	MultisampleStateCreateInfo.alphaToOneEnable			= VK_FALSE;
	MultisampleStateCreateInfo.minSampleShading			= PipelineConfigInfo.MinSampleShading;

	VkPipelineColorBlendAttachmentState ColorBlendAttachmentState;
	TMemory::Memset(&ColorBlendAttachmentState, 0, sizeof(ColorBlendAttachmentState));
	ColorBlendAttachmentState.colorWriteMask		= VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	ColorBlendAttachmentState.blendEnable			= PipelineConfigInfo.BlendEnable ? VK_TRUE : VK_FALSE;
	ColorBlendAttachmentState.srcColorBlendFactor	= VK_BLEND_FACTOR_ONE;   
	ColorBlendAttachmentState.dstColorBlendFactor	= VK_BLEND_FACTOR_ZERO;  
	ColorBlendAttachmentState.colorBlendOp			= VK_BLEND_OP_ADD;       
	ColorBlendAttachmentState.srcAlphaBlendFactor	= VK_BLEND_FACTOR_ONE;   
	ColorBlendAttachmentState.dstAlphaBlendFactor	= VK_BLEND_FACTOR_ZERO;  
	ColorBlendAttachmentState.alphaBlendOp			= VK_BLEND_OP_ADD;       

	VkPipelineColorBlendStateCreateInfo ColorBlendStateCreateInfo;
	TMemory::Memset(&ColorBlendStateCreateInfo, 0, sizeof(ColorBlendStateCreateInfo));
	ColorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	ColorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
	ColorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_COPY;  
	ColorBlendStateCreateInfo.attachmentCount = 1;
	ColorBlendStateCreateInfo.pAttachments = &ColorBlendAttachmentState;
	ColorBlendStateCreateInfo.blendConstants[0] = 1.0f;  
	ColorBlendStateCreateInfo.blendConstants[1] = 1.0f;  
	ColorBlendStateCreateInfo.blendConstants[2] = 1.0f;  
	ColorBlendStateCreateInfo.blendConstants[3] = 1.0f;  

	VkGraphicsPipelineCreateInfo GraphicsPipelineCreateInfo;
	TMemory::Memset(&GraphicsPipelineCreateInfo, 0, sizeof(GraphicsPipelineCreateInfo));
	GraphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	GraphicsPipelineCreateInfo.stageCount = static_cast<TUInt32>(ShaderStageCreateInfos.GetElementCount());
	GraphicsPipelineCreateInfo.pStages = ShaderStageCreateInfos.GetData();
	GraphicsPipelineCreateInfo.pVertexInputState = &VertexInpStateCreateInfo;
	GraphicsPipelineCreateInfo.pInputAssemblyState = &InputAssemblyStateCreateInfo;
	GraphicsPipelineCreateInfo.pViewportState = &ViewportStateCreateInfo;
	GraphicsPipelineCreateInfo.pRasterizationState = &RasterizationStateCreateInfo;
	GraphicsPipelineCreateInfo.pMultisampleState = &MultisampleStateCreateInfo;
	GraphicsPipelineCreateInfo.pDepthStencilState = &DepthStencilStateCreateInfo;
	GraphicsPipelineCreateInfo.pColorBlendState = &ColorBlendStateCreateInfo;
	GraphicsPipelineCreateInfo.pDynamicState = &DynamicStateCreateInfo;
	GraphicsPipelineCreateInfo.layout = PipelineLayout;
	GraphicsPipelineCreateInfo.renderPass = PipelineConfigInfo.RenderPass->GetVulkanRenderPass();
	GraphicsPipelineCreateInfo.subpass = 0;
	GraphicsPipelineCreateInfo.pTessellationState = nullptr;
	GraphicsPipelineCreateInfo.basePipelineHandle = nullptr;
	GraphicsPipelineCreateInfo.basePipelineIndex = -1;

	if (TVulkanRHI::VulkanPFNFunctions.CreateGraphicsPipelines(VulkanDevice->GetDevice(), nullptr, 1, &GraphicsPipelineCreateInfo, nullptr, &PipelineHandle) != VK_SUCCESS)
	{
		TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>("Failed to create GFX pipeline!");
		return false;
	}
	else
	{
		TLog::Success<TRNT_GET_LOG_INFO(VulkanRHI)>("Created GFX pipeline successful!");
	}

	return true;
}

TBool TVulkanGraphicsPipeline::CreatePipelineLayout(const TVulkanShader* VulkanShader)
{
	const TDynamicArray<VkPushConstantRange>& PushConstantRanges = VulkanShader->GetPushConstantRanges();
	const TDynamicArray<TVulkanDescriptorSetLayout>& DescriptorSetLayouts = VulkanShader->GetDescriptorSetLayouts();
	const TInt64 SetLayoutCount = DescriptorSetLayouts.GetElementCount();

	TDynamicArray<VkDescriptorSetLayout> FinalSetLayouts{};
	FinalSetLayouts.Resize(FinalSetLayouts.GetElementCount());

	for (TInt64 Index = 0; Index < SetLayoutCount; ++Index)
	{
		FinalSetLayouts[Index] = DescriptorSetLayouts[Index].GetDescriptorSetLayout();
	}

	VkPipelineLayoutCreateInfo PipelineLayoutCreateInfo;
	TMemory::Memset(&PipelineLayoutCreateInfo, 0, sizeof(PipelineLayoutCreateInfo));

	PipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	PipelineLayoutCreateInfo.pushConstantRangeCount = static_cast<TUInt32>(PushConstantRanges.GetElementCount());
	PipelineLayoutCreateInfo.pPushConstantRanges = PushConstantRanges.GetData();
	PipelineLayoutCreateInfo.setLayoutCount = static_cast<TUInt32>(SetLayoutCount);
	PipelineLayoutCreateInfo.pSetLayouts = FinalSetLayouts.GetData();
	PipelineLayoutCreateInfo.pNext = nullptr;

	if (TVulkanRHI::VulkanPFNFunctions.CreatePipelineLayout(VulkanDevice->GetDevice(), &PipelineLayoutCreateInfo, nullptr, &PipelineLayout) != VK_SUCCESS)
	{
		return false;
	}

	// Just to be safe! :v
	for (TInt64 Index = 0; Index < SetLayoutCount; ++Index)
	{
		FinalSetLayouts[Index] = VK_NULL_HANDLE;
	}

	return true;
}

void TVulkanGraphicsPipeline::Destroy()
{
	const TVulkanPFNFunctions VulkanPFNFuncs = TVulkanRHI::VulkanPFNFunctions;
	VkDevice Device = VulkanDevice->GetDevice();

	if (PipelineHandle != VK_NULL_HANDLE)
	{
		VulkanPFNFuncs.DestroyPipeline(Device, PipelineHandle, nullptr);
		PipelineHandle = VK_NULL_HANDLE;
	}

	if (PipelineLayout != VK_NULL_HANDLE)
	{
		VulkanPFNFuncs.DestroyPipelineLayout(Device, PipelineLayout, nullptr);
		PipelineLayout = VK_NULL_HANDLE;
	}
}

#endif