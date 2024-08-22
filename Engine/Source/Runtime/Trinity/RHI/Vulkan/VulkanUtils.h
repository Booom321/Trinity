#pragma once

#if defined(TRNT_SUPPORT_VULKAN_RHI)

#include "IncludeVulkanHeader.h"

#include "Trinity/RHI/RHIDefinitions.h"

class TRNT_API TVulkanUtils
{
public:
	static const TChar* ConvertVkResultToCString(VkResult Result);
	static const TChar* ConvertPhysicalDeviceTypeToCString(VkPhysicalDeviceType PhysicalDeviceType);
	static const TChar* ConvertVendorIDToVendorName(TUInt32 VendorID);
	static const TChar* ConvertShaderStageToCString(TShaderStage ShaderStage);
	static TString ConvertQueueFlagsToString(const VkQueueFlags QueueFlags);
	static VkShaderStageFlagBits ConvertTShaderStageToVkStage(TShaderStage ShaderStage);
	static VkPolygonMode ConvertTPolygonModeToVkPolygonMode(TPolygonMode PolygonMode);
	static VkCullModeFlagBits ConvertTCullModeToVkCullMode(TCullMode CullMode);
	static VkPrimitiveTopology ConvertTPrimitiveTopologyToVkPrimitiveTopology(TPrimitiveTopology Topology);
	static VkFrontFace ConvertTFrontFaceToVkFrontFace(TFrontFace FrontFace);
};

#ifndef TRNT_CHECK_VULKAN_RESULT
#	define TRNT_CHECK_VULKAN_RESULT(Function) \
	{\
		VkResult Result = (Function);\
		if (Result != VK_SUCCESS)\
		{\
			TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>("`{}` returned: {} [File: \"{}\" | Line: {}]", #Function, TVulkanUtils::ConvertVkResultToCString(Result), __FILE__, __LINE__);\
		}\
	}
#endif

#endif