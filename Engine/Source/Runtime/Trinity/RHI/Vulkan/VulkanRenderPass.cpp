#include "TrinityPCH.h"

#if defined(TRNT_SUPPORT_VULKAN_RHI)

#include "VulkanRenderPass.h"
#include "VulkanDevice.h"
#include "Trinity/Core/Containers/StaticArray.h"

TVulkanRenderPass::TVulkanRenderPass(TVulkanDevice* VulkanDevice)
	: VulkanDevice(VulkanDevice), RenderPass(VK_NULL_HANDLE)
{

}

TVulkanRenderPass::~TVulkanRenderPass()
{

}

TBool TVulkanRenderPass::Initialize(VkFormat SwapChainSurfaceFormat, VkFormat DepthFormat)
{
	TStaticArray<VkAttachmentDescription, 2> AttachmentDescriptions{};

	VkAttachmentDescription& ColorAttachment = AttachmentDescriptions[0];
	ColorAttachment.format			= SwapChainSurfaceFormat;
	ColorAttachment.samples			= VK_SAMPLE_COUNT_1_BIT;
	ColorAttachment.loadOp			= VK_ATTACHMENT_LOAD_OP_CLEAR;
	ColorAttachment.storeOp			= VK_ATTACHMENT_STORE_OP_STORE;
	ColorAttachment.stencilStoreOp	= VK_ATTACHMENT_STORE_OP_DONT_CARE;
	ColorAttachment.stencilLoadOp	= VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	ColorAttachment.initialLayout	= VK_IMAGE_LAYOUT_UNDEFINED;
	ColorAttachment.finalLayout		= VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentDescription& DepthAttachment = AttachmentDescriptions[1];
	DepthAttachment.format			= DepthFormat;
	DepthAttachment.samples			= VK_SAMPLE_COUNT_1_BIT;
	DepthAttachment.loadOp			= VK_ATTACHMENT_LOAD_OP_CLEAR;
	DepthAttachment.storeOp			= VK_ATTACHMENT_STORE_OP_DONT_CARE;
	DepthAttachment.stencilLoadOp	= VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	DepthAttachment.stencilStoreOp	= VK_ATTACHMENT_STORE_OP_DONT_CARE;
	DepthAttachment.initialLayout	= VK_IMAGE_LAYOUT_UNDEFINED;
	DepthAttachment.finalLayout		= VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference ColorAttachmentRef{};
	ColorAttachmentRef.attachment = 0;
	ColorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference DepthAttachmentRef{};
	DepthAttachmentRef.attachment = 1;
	DepthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription SubpassDescription{};
	SubpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	SubpassDescription.colorAttachmentCount = 1;
	SubpassDescription.pColorAttachments = &ColorAttachmentRef;
	SubpassDescription.pDepthStencilAttachment = &DepthAttachmentRef;

	VkSubpassDependency SubpassDependency{};
	SubpassDependency.dstSubpass	= 0;
	SubpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
	SubpassDependency.dstStageMask	= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	SubpassDependency.srcSubpass	= VK_SUBPASS_EXTERNAL;
	SubpassDependency.srcAccessMask = 0;
	SubpassDependency.srcStageMask	= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;

	VkRenderPassCreateInfo RenderPassCreateInfo;
	TMemory::Memset(&RenderPassCreateInfo, 0, sizeof(RenderPassCreateInfo));
	RenderPassCreateInfo.sType				= VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	RenderPassCreateInfo.pNext				= nullptr;
	RenderPassCreateInfo.attachmentCount	= static_cast<TUInt32>(AttachmentDescriptions.GetElementCount());
	RenderPassCreateInfo.pAttachments		= AttachmentDescriptions.GetData();
	RenderPassCreateInfo.subpassCount		= 1;
	RenderPassCreateInfo.pSubpasses			= &SubpassDescription;
	RenderPassCreateInfo.dependencyCount	= 1;
	RenderPassCreateInfo.pDependencies		= &SubpassDependency;

	if (TVulkanRHI::VulkanPFNFunctions.CreateRenderPass(VulkanDevice->GetDevice(), &RenderPassCreateInfo, nullptr, &RenderPass) != VK_SUCCESS)
	{
		TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>(
			"Failed to create VkRenderPass [SwapChainSurfaceFormat={} | DepthFormat = {}]", static_cast<TUInt32>(SwapChainSurfaceFormat), static_cast<TUInt32>(DepthFormat));
		return false;
	}

	return true;
}

void TVulkanRenderPass::Destroy()
{
	if (RenderPass != VK_NULL_HANDLE)
	{
		TVulkanRHI::VulkanPFNFunctions.DestroyRenderPass(VulkanDevice->GetDevice(), RenderPass, nullptr);
		RenderPass = VK_NULL_HANDLE;
	}
}

#endif 
