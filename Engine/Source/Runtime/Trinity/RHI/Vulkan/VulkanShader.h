#pragma once

#if defined(TRNT_SUPPORT_VULKAN_RHI)

#include "Trinity/Core/Containers/HashMap.h"

#include "VulkanRHI.h"
#include "VulkanDescriptors.h"

class TRNT_API TVulkanShader
{
public:
	TVulkanShader(TVulkanDevice* VulkanDevice, const TChar* Name);
	~TVulkanShader();

public:
	TRNT_FORCE_INLINE const TString& GetName() const { return Name; }
	TRNT_FORCE_INLINE void SetName(const TChar* Name);

	TBool LoadFromFile(TShaderStage ShaderStage, const TString& FilePath);
	TBool LoadFromSourceCode(TShaderStage ShaderStage, const TString& SourceCode);
	TBool LoadFromSPIRVCode(TShaderStage ShaderStage, const TDynamicArray<TUInt32>& SPIRVCode);

	void Destroy(TBool ClearSetLayouts = true);
	void ClearDescriptorSetLayouts();

public:
	TRNT_FORCE_INLINE const TDynamicArray<VkVertexInputAttributeDescription>& GetVertexInputAttributeDescriptions() const { return VertexInpAttributeDescriptions; }
	TRNT_FORCE_INLINE const TDynamicArray<TVulkanDescriptorSetLayout>& GetDescriptorSetLayouts() const { return DescriptorSetLayouts; }
	TRNT_FORCE_INLINE const TDynamicArray<VkPushConstantRange>& GetPushConstantRanges() const { return PushConstantRanges; }

	TRNT_FORCE_INLINE VkShaderModule GetShaderModuleByStage(TShaderStage ShaderStage) const { return ShaderModules[static_cast<TInt32>(ShaderStage)]; }
	TRNT_FORCE_INLINE TUInt32 GetVertexInputStride() const { return VertexInputStride; }
private:
	TBool BuildShaderInternal(TShaderStage ShaderStage, const TDynamicArray<TUInt32>& SPIRVData);
	TBool CreateShaderModule(TShaderStage ShaderStage, const TDynamicArray<TUInt32>& SPIRVData);
	void ReflectShader(TShaderStage ShaderStage, const TDynamicArray<TUInt32>& SPIRVData);
	void BuildDescriptorSetLayouts(const THashMap<TUInt32, TDynamicArray<VkDescriptorSetLayoutBinding>>& SetLayoutBindingMap);

	TString Name;
	TVulkanDevice* VulkanDevice;
	TUInt32 VertexInputStride = 0;
	VkShaderModule ShaderModules[static_cast<TInt32>(TShaderStage::EMax)];
	TDynamicArray<VkVertexInputAttributeDescription> VertexInpAttributeDescriptions;
	TDynamicArray<TVulkanDescriptorSetLayout> DescriptorSetLayouts;
	TDynamicArray<VkPushConstantRange> PushConstantRanges;
};

#endif