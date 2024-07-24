#pragma once

#if defined(TRNT_SUPPORT_VULKAN_RHI)

#include "IncludeVulkanHeader.h"
#include "VulkanDevice.h"
#include "VulkanShader.h"
#include "VulkanRenderPass.h"

class TRNT_API TVulkanPipelineBase
{
public:
	TVulkanPipelineBase(TVulkanDevice* VulkanDevice);
	virtual ~TVulkanPipelineBase() = default;

public:
	TRNT_INLINE VkPipeline GetVulkanPipeline() const
	{
		return PipelineHandle;
	}

protected:
	TVulkanDevice* VulkanDevice;
	VkPipeline PipelineHandle;
};

class TRNT_API TVulkanComputePipeline : public TVulkanPipelineBase
{
public:
	TVulkanComputePipeline(TVulkanDevice* VulkanDevice);
	virtual ~TVulkanComputePipeline();

private:
};

class TRNT_API TVulkanGraphicsPipeline : public TVulkanPipelineBase
{
public:
	class TConfigInfo
	{
	public:
		TVulkanShader* Shader;
		TVulkanRenderPass* RenderPass;
		TPrimitiveTopology PrimitiveTopology;
		TPolygonMode PolygonMode;
		TFrontFace FrontFace;
		TCullMode CullMode;
		VkSampleCountFlagBits SampleCount;
		TFloat DepthBiasConstantFactor;
		TFloat DepthBiasSlopeFactor;
		TFloat MinSampleShading;
		TBool DepthBiasEnabled;
		TBool DepthWriteEnable;
		TBool DepthTestEnable;
		TBool BlendEnable;
	};

	TVulkanGraphicsPipeline(TVulkanDevice* VulkanDevice);
	virtual ~TVulkanGraphicsPipeline();

	TBool Initialize(const TConfigInfo& PipelineConfigInfo);
	void Destroy();

private:
	TBool CreatePipelineLayout(const TVulkanShader* VulkanShader);

	VkPipelineLayout PipelineLayout;
};

#endif 