#include "TrinityPCH.h"

#if defined(TRNT_SUPPORT_VULKAN_RHI)

#include "VulkanDescriptors.h"
#include "VulkanDevice.h"

/////////////////////////////////////////////////////////
// TVulkanDescriptorSetLayout ///////////////////////////
/////////////////////////////////////////////////////////
TVulkanDescriptorSetLayout::TVulkanDescriptorSetLayout(TVulkanDevice* VulkanDevice)
	: VulkanDevice(VulkanDevice), DescriptorSetLayout(VK_NULL_HANDLE)
{
}

TVulkanDescriptorSetLayout::~TVulkanDescriptorSetLayout()
{
}

void TVulkanDescriptorSetLayout::AddLayoutBinding(TUInt32 Binding, VkDescriptorType DescriptorType, TUInt32 DescriptorCount, VkShaderStageFlags StageFlags)
{
	if (DescriptorSetLayoutBindings.Count(Binding) > 0)
	{
		TLog::Warning<TRNT_GET_LOG_INFO(VulkanRHI)>("Skip binding {} because binding already in use.", Binding);
		return;
	}

	VkDescriptorSetLayoutBinding SetLayoutBinding{};
	SetLayoutBinding.binding = Binding;
	SetLayoutBinding.descriptorType = DescriptorType;
	SetLayoutBinding.descriptorCount = DescriptorCount;
	SetLayoutBinding.stageFlags = StageFlags;
	DescriptorSetLayoutBindings[Binding] = SetLayoutBinding;
}

TBool TVulkanDescriptorSetLayout::Build()
{
	if (IsBuilt)
	{
		TLog::Warning<TRNT_GET_LOG_INFO(VulkanRHI)>("Descriptor set layout has already been built.");
		return IsBuilt;
	}

	TDynamicArray<VkDescriptorSetLayoutBinding> FinalSetLayoutBindings{};
	for (const THashMapElement<TUInt32, VkDescriptorSetLayoutBinding>& Element : DescriptorSetLayoutBindings)
	{
		FinalSetLayoutBindings.Add(Element.Value);
	}

	VkDescriptorSetLayoutCreateInfo SetLayoutCreateInfo;
	TMemory::Memset(&SetLayoutCreateInfo, 0, sizeof(SetLayoutCreateInfo));
	SetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	SetLayoutCreateInfo.bindingCount = static_cast<TUInt32>(FinalSetLayoutBindings.GetElementCount());
	SetLayoutCreateInfo.pBindings = FinalSetLayoutBindings.GetData();
	SetLayoutCreateInfo.pNext = nullptr;

	if (TVulkanRHI::VulkanPFNFunctions.CreateDescriptorSetLayout(VulkanDevice->GetDevice(), &SetLayoutCreateInfo, nullptr, &DescriptorSetLayout) != VK_SUCCESS)
	{
		IsBuilt = false;
	}
	else
	{
		IsBuilt = true;
	}

	return IsBuilt;
}

void TVulkanDescriptorSetLayout::Destroy()
{
	if (DescriptorSetLayout != VK_NULL_HANDLE)
	{
		TVulkanRHI::VulkanPFNFunctions.DestroyDescriptorSetLayout(VulkanDevice->GetDevice(), DescriptorSetLayout, nullptr);
		DescriptorSetLayout = VK_NULL_HANDLE;
	}
}

#endif