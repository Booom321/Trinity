#pragma once

#if defined(TRNT_USE_VULKAN_RHI)

#include "IncludeVulkanHeader.h"

class TRNT_API TVulkanUtils
{
public:
	static const TChar* ConvertVkResultToCString(VkResult Result);
	static const TChar* ConvertPhysicalDeviceTypeToCString(VkPhysicalDeviceType PhysicalDeviceType);
	static const TChar* ConvertVendorIDToVendorName(TUInt32 VendorID);
	static TString ConvertQueueFlagsToString(const VkQueueFlags QueueFlags);
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