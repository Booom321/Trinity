#include "TrinityPCH.h"

#if defined(TRNT_SUPPORT_VULKAN_RHI)

#include "VulkanRHI.h"
#include "VulkanUtils.h"
#include "VulkanDevice.h"
#include "VulkanPlatform.h"

#include "Trinity/Engine/EngineGlobals.h"
#include "Trinity/Application/Application.h"
#include "Trinity/Core/Utilities/CommandLine.h"

#pragma warning( push )
#pragma warning( disable: 26495 )

#define TRNT_VK_LAYER_KHRONOS_VALIDATION "VK_LAYER_KHRONOS_validation"
#define TRNT_VK_LAYER_LUNARG_STANDARD_VALIDATION "VK_LAYER_LUNARG_standard_validation"
#define TRNT_VK_LAYER_LUNARG_CORE_VALIDATION "VK_LAYER_LUNARG_core_validation"

static const VkValidationFeatureEnableEXT ValidationFeaturesEnable[] = { VK_VALIDATION_FEATURE_ENABLE_SYNCHRONIZATION_VALIDATION_EXT };

//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------- TVulkanPFNFunctions -------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//

TBool TVulkanPFNFunctions::LoadVulkanInstanceFunctions()
{
#define TRNT_LOAD_VULKAN_INSTANCE_FUNCTION(ProcName, PFNFunc) \
	PFNFunc = reinterpret_cast<PFN_##ProcName>(TVulkanPlatform::VulkanGetInstanceProcAddr(nullptr, TRNT_STRINGIFY(ProcName)));\
	if (!PFNFunc) \
	{\
		return false; \
	}

	TRNT_LOAD_VULKAN_INSTANCE_FUNCTION(vkEnumerateInstanceExtensionProperties, EnumerateInstanceExtensionProperties);
	TRNT_LOAD_VULKAN_INSTANCE_FUNCTION(vkEnumerateInstanceLayerProperties, EnumerateInstanceLayerProperties);
	TRNT_LOAD_VULKAN_INSTANCE_FUNCTION(vkCreateInstance, CreateInstance);

#undef TRNT_LOAD_VULKAN_INSTANCE_FUNCTION

	return true;
}

TBool TVulkanPFNFunctions::LoadVulkanPFNFunctions(VkInstance Instance)
{
#define TRNT_LOAD_VULKAN_PFN_FUNCTION(ProcName, PFNFunc) \
	PFNFunc = reinterpret_cast<PFN_##ProcName>(TVulkanPlatform::VulkanGetInstanceProcAddr(Instance, TRNT_STRINGIFY(ProcName)));\
	if (!PFNFunc) \
	{\
		return false; \
	}

#ifdef TRNT_PLATFORM_WIN64
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkCreateWin32SurfaceKHR, CreateWin32SurfaceKHR);
#endif 
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkDestroyInstance, DestroyInstance);

	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkGetPhysicalDeviceProperties, GetPhysicalDeviceProperties);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkGetPhysicalDeviceFeatures, GetPhysicalDeviceFeatures);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkEnumeratePhysicalDevices, EnumeratePhysicalDevices);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkEnumerateDeviceExtensionProperties, EnumerateDeviceExtensionProperties);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkGetPhysicalDeviceQueueFamilyProperties2, GetPhysicalDeviceQueueFamilyProperties2);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkGetPhysicalDeviceMemoryProperties, GetPhysicalDeviceMemoryProperties);
	
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkCreateDevice, CreateDevice);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkGetDeviceQueue, GetDeviceQueue);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkDeviceWaitIdle, DeviceWaitIdle);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkDestroyDevice, DestroyDevice);

	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkGetPhysicalDeviceSurfaceFormatsKHR, GetPhysicalDeviceSurfaceFormatsKHR);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkGetPhysicalDeviceSurfaceCapabilitiesKHR, GetPhysicalDeviceSurfaceCapabilitiesKHR);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkGetPhysicalDeviceSurfacePresentModesKHR, GetPhysicalDeviceSurfacePresentModesKHR);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkGetPhysicalDeviceSurfaceSupportKHR, GetPhysicalDeviceSurfaceSupportKHR);

	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkCreateSwapchainKHR, CreateSwapchainKHR);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkDestroySurfaceKHR, DestroySurfaceKHR);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkDestroySwapchainKHR, DestroySwapchainKHR);

	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkGetSwapchainImagesKHR, GetSwapchainImagesKHR);

	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkCreateImageView, CreateImageView);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkDestroyImageView, DestroyImageView);

	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkCreateShaderModule, CreateShaderModule);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkDestroyShaderModule, DestroyShaderModule);

	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkCreateRenderPass, CreateRenderPass);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkDestroyRenderPass, DestroyRenderPass);

	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkCreatePipelineLayout, CreatePipelineLayout);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkDestroyPipelineLayout, DestroyPipelineLayout);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkCreateGraphicsPipelines, CreateGraphicsPipelines);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkDestroyPipeline, DestroyPipeline);

	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkCreateDescriptorSetLayout, CreateDescriptorSetLayout);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkDestroyDescriptorSetLayout, DestroyDescriptorSetLayout);

	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkGetPhysicalDeviceFormatProperties, GetPhysicalDeviceFormatProperties);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkCreateFramebuffer, CreateFramebuffer);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkDestroyFramebuffer, DestroyFramebuffer);

	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkCreateImage, CreateImage);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkDestroyImage, DestroyImage);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkAllocateMemory, AllocateMemory);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkFreeMemory, FreeMemory);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkBindImageMemory, BindImageMemory);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkGetImageMemoryRequirements, GetImageMemoryRequirements);

	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkCreateCommandPool, CreateCommandPool);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkDestroyCommandPool, DestroyCommandPool);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkAllocateCommandBuffers, AllocateCommandBuffers);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkFreeCommandBuffers, FreeCommandBuffers);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkBeginCommandBuffer, BeginCommandBuffer);
	
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkEndCommandBuffer, EndCommandBuffer);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkResetCommandBuffer, ResetCommandBuffer);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkCmdBeginRenderPass, CmdBeginRenderPass);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkCmdBeginRenderPass2KHR, CmdBeginRenderPass2KHR);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkCmdEndRenderPass, CmdEndRenderPass);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkCmdEndRenderPass, CmdEndRenderPass);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkCmdBindPipeline, CmdBindPipeline);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkCmdDraw, CmdDraw);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkCmdSetViewport, CmdSetViewport);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkCmdSetScissor, CmdSetScissor);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkCmdBindVertexBuffers, CmdBindVertexBuffers);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkCmdBindIndexBuffer, CmdBindIndexBuffer);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkCmdCopyBuffer, CmdCopyBuffer);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkCmdDrawIndexed, CmdDrawIndexed);

	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkCreateSemaphore, CreateSemaphore);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkCreateFence, CreateFence);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkDestroySemaphore, DestroySemaphore);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkDestroyFence, DestroyFence);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkWaitForFences, WaitForFences);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkResetFences, ResetFences);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkAcquireNextImageKHR, AcquireNextImageKHR);

	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkQueueSubmit, QueueSubmit);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkQueuePresentKHR, QueuePresentKHR);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkQueueWaitIdle, QueueWaitIdle);

	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkCreateBuffer, CreateBuffer);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkDestroyBuffer, DestroyBuffer);

	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkBindBufferMemory, BindBufferMemory);

	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkMapMemory, MapMemory);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkUnmapMemory, UnmapMemory);

	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkInvalidateMappedMemoryRanges, InvalidateMappedMemoryRanges);
	TRNT_LOAD_VULKAN_PFN_FUNCTION(vkFlushMappedMemoryRanges, FlushMappedMemoryRanges);
#undef TRNT_LOAD_VULKAN_PFN_FUNCTION
	
	return true;
}

void TVulkanRHIFeatures::Reset(TVulkanRHIFeatures& Features)
{
	Features = TVulkanRHIFeatures{};
	return;
}

TVulkanPhysicalDevice::TVulkanPhysicalDevice(VkPhysicalDevice PhysicalDevice)
	: Handle(PhysicalDevice)
{
	if (Handle != VK_NULL_HANDLE)
	{
		TVulkanRHI::VulkanPFNFunctions.GetPhysicalDeviceFeatures(Handle, &PhysicalDeviceFeatures);
		TVulkanRHI::VulkanPFNFunctions.GetPhysicalDeviceProperties(Handle, &PhysicalDeviceProperties);
	}	
}

TVulkanPhysicalDevice::~TVulkanPhysicalDevice() = default;

//------------------------------------------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------- TVulkanRHI -----------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------------------------------------------//

TVulkanPFNFunctions TVulkanRHI::VulkanPFNFunctions = TVulkanPFNFunctions{};

#if defined(TRNT_DEBUG)

#if defined(TRNT_USE_VULKAN_DEBUG_UTILS)

VKAPI_ATTR VkBool32 VKAPI_PTR TVulkanRHI::DebugUtilsCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT MessageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT MessageType,
	const VkDebugUtilsMessengerCallbackDataEXT* CallbackData,
	void* UserData)
{
	const TChar* MsgType = "";

	if (MessageType & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT)
	{
		if (MessageType & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
		{
			MsgType = "General | Validation";
		}
		else
		{
			MsgType = "General";
		}
	}
	else if (MessageType & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
	{
		if (MessageType & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
		{
			MsgType = "Performance | Validation";
		}
		else
		{
			MsgType = "Validation";
		}
	}
	else if (MessageType & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
	{
		MsgType = "Performance";
	}

	if (MessageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
	{
		TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>("MsgType: {} | Message: {}", MsgType, CallbackData->pMessage);
	}
	else if (MessageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
	{
		TLog::Warning<TRNT_GET_LOG_INFO(VulkanRHI)>("MsgType: {} | Message: {}", MsgType, CallbackData->pMessage);
	}
	else if (MessageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
	{
		TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>("MsgType: {} | Message: {}", MsgType, CallbackData->pMessage);
	}
	else if (MessageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
	{
		TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>("MsgType: {} | Message: {}", MsgType, CallbackData->pMessage);
	}

	return VK_FALSE;
}

VkResult TVulkanRHI::VKCreateDebugUtilsMessengerEXT(
	VkInstance Instance, const VkDebugUtilsMessengerCreateInfoEXT* DebugUtilsMessengerCreateInfo, const VkAllocationCallbacks* AllocationCallbacks, VkDebugUtilsMessengerEXT* DebugUtilsMessenger)
{
	PFN_vkCreateDebugUtilsMessengerEXT CreateDebugUtilsMessenger =
				reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(TVulkanPlatform::VulkanGetInstanceProcAddr(Instance, "vkCreateDebugUtilsMessengerEXT"));

	if (CreateDebugUtilsMessenger)
	{
		return CreateDebugUtilsMessenger(Instance, DebugUtilsMessengerCreateInfo, AllocationCallbacks, DebugUtilsMessenger);
	}

	return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void TVulkanRHI::VKDestroyDebugUtilsMessengerEXT(VkInstance Instance, VkDebugUtilsMessengerEXT DebugUtilsMessenger, const VkAllocationCallbacks* AllocationCallbacks)
{
	PFN_vkDestroyDebugUtilsMessengerEXT DestroyDebugUtilsMessenger =
				reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(TVulkanPlatform::VulkanGetInstanceProcAddr(Instance, "vkDestroyDebugUtilsMessengerEXT"));

	if (DestroyDebugUtilsMessenger)
	{
		return DestroyDebugUtilsMessenger(Instance, DebugUtilsMessenger, AllocationCallbacks);
	}
}

#elif defined(TRNT_USE_VULKAN_DEBUG_REPORT)

VkBool32 VKAPI_PTR TVulkanRHI::DebugReportFunction(
	VkDebugReportFlagsEXT ReportFlags,
	VkDebugReportObjectTypeEXT ObjectType,
	TUInt64 SourceObject,
	TSize_T Location,
	TInt32 MessageCode,
	const TChar* LayerPrefix,
	const TChar* Message,
	void* UserData)
{
	if (!ReportFlags)
	{
		return VK_FALSE;
	}

	if (ReportFlags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
	{
		TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>("Error: [{} | Code: {}]: {}", LayerPrefix, MessageCode, Message);
	}

	if (ReportFlags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
	{
		TLog::Warning<TRNT_GET_LOG_INFO(VulkanRHI)>("Warning: [{} | Code: {}]: {}", LayerPrefix, MessageCode, Message);
	}

	if (ReportFlags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
	{
		TLog::Warning<TRNT_GET_LOG_INFO(VulkanRHI)>("Performance: [{} | Code: {}]: {}", LayerPrefix, MessageCode, Message);
	}

	if (ReportFlags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT)
	{
		TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>("Info: [{} | Code: {}]: {}", LayerPrefix, MessageCode, Message);
	}

	if (ReportFlags & VK_DEBUG_REPORT_DEBUG_BIT_EXT)
	{
		TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>("Debug: [{} | Code: {}]: {}", LayerPrefix, MessageCode, Message);
	}

	return VK_FALSE;
}

VkResult TVulkanRHI::VKCreateDebugReportCallbackEXT(
	VkInstance Instance, const VkDebugReportCallbackCreateInfoEXT* DebugReportCallbackCreateInfo, const VkAllocationCallbacks* AllocationCallbacks, VkDebugReportCallbackEXT* DebugReportCallback)
{
	PFN_vkCreateDebugReportCallbackEXT CreateDebugReportCallbackEXT = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(TVulkanPlatform::VulkanGetInstanceProcAddr(Instance, "vkCreateDebugReportCallbackEXT"));

	if (CreateDebugReportCallbackEXT)
	{
		return CreateDebugReportCallbackEXT(Instance, DebugReportCallbackCreateInfo, AllocationCallbacks, DebugReportCallback);
	}

	return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void TVulkanRHI::VKDestroyDebugReportCallbackEXT(VkInstance Instance, VkDebugReportCallbackEXT DebugReportCallback, const VkAllocationCallbacks* AllocationCallbacks)
{
	PFN_vkDestroyDebugReportCallbackEXT DestroyDebugReportCallbackEXT =
		reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(TVulkanPlatform::VulkanGetInstanceProcAddr(Instance, "vkDestroyDebugReportCallbackEXT"));

	if (DestroyDebugReportCallbackEXT)
	{
		return DestroyDebugReportCallbackEXT(Instance, DebugReportCallback, AllocationCallbacks);
	}
}

#endif

#endif

TVulkanRHI::TVulkanRHI()
	:	Instance(VK_NULL_HANDLE), VulkanRHIFeatures(), VulkanDevice(nullptr)
{
	/*TCommandLineParser* CmdLineParserInstance = TCommandLineParser::GetInstance();

	if (!CmdLineParserInstance->GetBooleanOption(VulkanRHIFeatures.EnableValidationLayer, "-enablevalidationlayer"))
	{
		VulkanRHIFeatures.EnableValidationLayer = false;
	}*/

	VulkanRHIFeatures.EnableValidationLayer = true;
}


TVulkanRHI::~TVulkanRHI()
{

}


TBool TVulkanRHI::Initialize()
{
	const TBool IsVulkanLibraryLoaded = TVulkanPlatform::LoadVulkanLibrary();

	if (!IsVulkanLibraryLoaded)
	{
		TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>("Failed to load Vulkan library!");
		return false;
	}

	if (!VulkanPFNFunctions.LoadVulkanInstanceFunctions())
	{
		TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>("Failed to load Vulkan instance functions!");
		return false;
	}

	if (!CreateVulkanInstance())
	{
		TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>("Failed to create Vulkan instance!");
		return false;
	}

#ifdef TRNT_DEBUG
	SetupDebugLayerCallback();
#endif

	TVulkanPhysicalDevice SelectedPhysicalDevice(ChoosePhysicalDevice());

	if (SelectedPhysicalDevice.Handle == VK_NULL_HANDLE)
	{
		TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>("Failed to choose suitable GPU to create Vulkan device!");
		return false;
	}

	TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>("**************** Selected GPU properties ****************");
	TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>("> GPU name: {}", SelectedPhysicalDevice.PhysicalDeviceProperties.deviceName);
	TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>("> GPU ID: {}", SelectedPhysicalDevice.PhysicalDeviceProperties.deviceID);
	TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>("> GPU Type: {}", TVulkanUtils::ConvertPhysicalDeviceTypeToCString(SelectedPhysicalDevice.PhysicalDeviceProperties.deviceType));
	TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>("> Vendor: {}", TVulkanUtils::ConvertVendorIDToVendorName(SelectedPhysicalDevice.PhysicalDeviceProperties.vendorID));
	TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>("> Vendor ID:  0x{:x}", SelectedPhysicalDevice.PhysicalDeviceProperties.vendorID);
	TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>(
		"> API version: {}.{}.{}", 
		VK_VERSION_MAJOR(SelectedPhysicalDevice.PhysicalDeviceProperties.apiVersion),
		VK_VERSION_MINOR(SelectedPhysicalDevice.PhysicalDeviceProperties.apiVersion), VK_VERSION_PATCH(SelectedPhysicalDevice.PhysicalDeviceProperties.apiVersion));
	TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>(
		"> Driver version: {}.{}.{}", 
		VK_VERSION_MAJOR(SelectedPhysicalDevice.PhysicalDeviceProperties.driverVersion), 
		VK_VERSION_MINOR(SelectedPhysicalDevice.PhysicalDeviceProperties.driverVersion), VK_VERSION_PATCH(SelectedPhysicalDevice.PhysicalDeviceProperties.driverVersion));

	TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>("> Max descriptor sets bound: {}", SelectedPhysicalDevice.PhysicalDeviceProperties.limits.maxBoundDescriptorSets);

	if (!CreateVulkanDevice(SelectedPhysicalDevice))
	{
		return false;
	}

	return true;
}

void TVulkanRHI::Shutdown()
{
	if (VulkanDevice)
	{
		VulkanDevice->Destroy();

		delete VulkanDevice;
		VulkanDevice = nullptr;
	}

	if (Instance)
	{
#ifdef TRNT_DEBUG
#	ifdef TRNT_USE_VULKAN_DEBUG_UTILS
		VKDestroyDebugUtilsMessengerEXT(Instance, DebugUtilsMessenger, nullptr);
#	elif defined(TRNT_USE_VULKAN_DEBUG_REPORT)
		VKDestroyDebugReportCallbackEXT(Instance, DebugReportCallback, nullptr);
#	endif
#endif
		VulkanPFNFunctions.DestroyInstance(Instance, nullptr);
		Instance = VK_NULL_HANDLE;
	}

	TVulkanPlatform::FreeVulkanLibrary();

	TVulkanRHIFeatures::Reset(VulkanRHIFeatures);
}

const TChar* TVulkanRHI::GetName()
{
	return TRNT_VULKAN_RHI_NAME;
}

TVersion TVulkanRHI::GetVersion() const
{
	return VulkanInstanceVersion;
}

TGraphicsAPI TVulkanRHI::GetGraphicsAPI()
{
	return TGraphicsAPI::EVulkan;
}

TUInt32 TVulkanRHI::EnumerateInstanceExtensionProperties(const TChar* LayerName, TDynamicArray<VkExtensionProperties>& AvailableExtensionProps)
{
	TUInt32 ExtensionCount = 0;

	TRNT_CHECK_VULKAN_RESULT(VulkanPFNFunctions.EnumerateInstanceExtensionProperties(LayerName, &ExtensionCount, nullptr));

	if (ExtensionCount > 0)
	{
		AvailableExtensionProps.Resize(ExtensionCount);
		TRNT_CHECK_VULKAN_RESULT(VulkanPFNFunctions.EnumerateInstanceExtensionProperties(LayerName, &ExtensionCount, AvailableExtensionProps.GetData()));
	}

	return ExtensionCount;
}

TUInt32 TVulkanRHI::EnumerateInstanceLayerProperties(TDynamicArray<VkLayerProperties>& AvailableLayerProps)
{
	TUInt32 LayerCount = 0;

	TRNT_CHECK_VULKAN_RESULT(VulkanPFNFunctions.EnumerateInstanceLayerProperties(&LayerCount, nullptr));

	if (LayerCount > 0)
	{
		AvailableLayerProps.Resize(LayerCount);
		TRNT_CHECK_VULKAN_RESULT(VulkanPFNFunctions.EnumerateInstanceLayerProperties(&LayerCount, AvailableLayerProps.GetData()));
	}

	return LayerCount;
}

static void AddInstanceExtensionNameIfNotExists(const TChar* ExtensionNameToAdd, TDynamicArray<const TChar*>& ExtensionNames)
{
	for (TInt64 Index = 0, ExtensionNameCount = ExtensionNames.GetElementCount(); Index < ExtensionNameCount; ++Index)
	{
		if (::strcmp(ExtensionNameToAdd, ExtensionNames[Index]) == 0)
		{
			return;
		}
	}

	ExtensionNames.EmplaceBack(ExtensionNameToAdd);
}

void TVulkanRHI::SetupInstanceLayersAndExtensions()
{
	TDynamicArray<TVulkanLayerAndExtension> GlobalLayersAndExtensions;
	TDynamicArray<VkExtensionProperties> GlobalInstanceExtensions;

	{
		TDynamicArray<VkLayerProperties> InstanceLayerProperties;
		TUInt32 InstanceLayerPropertiesCount = EnumerateInstanceLayerProperties(InstanceLayerProperties);
		GlobalLayersAndExtensions.Resize(static_cast<TInt64>(InstanceLayerPropertiesCount));

		TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>("========= Found {} available instance layer(s) =========", InstanceLayerPropertiesCount);
		for (TInt64 Index = 0; Index < InstanceLayerPropertiesCount; ++Index)
		{
			VkLayerProperties& CurrentLayerProp = InstanceLayerProperties[Index];
			TVulkanLayerAndExtension& CurrentLayerAndExtension = GlobalLayersAndExtensions[Index];

			CurrentLayerAndExtension.LayerProperties = CurrentLayerProp;
			TUInt32 LayerExtensionCount = EnumerateInstanceExtensionProperties(CurrentLayerProp.layerName, CurrentLayerAndExtension.ExtensionProperties);

			TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>("{}. \"{}\" [{} extension(s)]", Index + 1, CurrentLayerProp.layerName, LayerExtensionCount);
		}
	}

	{
		TUInt32 InstanceExtensionCount = EnumerateInstanceExtensionProperties(nullptr, GlobalInstanceExtensions);
		TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>("========= Found {} instance extension(s) =========", InstanceExtensionCount);
		for (TInt64 Index = 0; Index < static_cast<TInt64>(InstanceExtensionCount); ++Index)
		{
			TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>("{}. \"{}\".", Index + 1, GlobalInstanceExtensions[Index].extensionName);
		}
	}

	const auto ContainsExtension = [&GlobalInstanceExtensions](const TChar* ExtensionName) -> TBool 
	{
		return GlobalInstanceExtensions.FindElementIf([ExtensionName](const VkExtensionProperties& Extension) -> TBool 
		{
			return ::strcmp(Extension.extensionName, ExtensionName) == 0; 
		}) != TDynamicArray<VkExtensionProperties>::Npos;
	};

#if defined(TRNT_DEBUG)
#if defined(TRNT_USE_VULKAN_DEBUG_UTILS) 
	if (VulkanRHIFeatures.EnableValidationLayer && ContainsExtension(VK_EXT_DEBUG_UTILS_EXTENSION_NAME))
	{
		AddInstanceExtensionNameIfNotExists(VK_EXT_DEBUG_UTILS_EXTENSION_NAME, InstanceExtensions);
		VulkanRHIFeatures.SupportsDebugUtils = true;
	}
#else
	if (EnableValidationLayer && ContainsExtension(VK_EXT_DEBUG_REPORT_EXTENSION_NAME))
	{
		AddInstanceExtensionNameIfNotExists(VK_EXT_DEBUG_REPORT_EXTENSION_NAME, InstanceExtensions);
		SupportsDebugReport = true;
	}
#endif
#endif

	const TBool ContainsKHRSurfaceExtension = ContainsExtension(VK_KHR_SURFACE_EXTENSION_NAME);

	if (ContainsKHRSurfaceExtension)
	{
		AddInstanceExtensionNameIfNotExists(VK_KHR_SURFACE_EXTENSION_NAME, InstanceExtensions);
	}

	if (ContainsKHRSurfaceExtension && ContainsExtension(VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME))
	{
		AddInstanceExtensionNameIfNotExists(VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME, InstanceExtensions);
		VulkanRHIFeatures.SupportsSurfaceCapabilities2 = true;
	}

	if (ContainsKHRSurfaceExtension && ContainsExtension(VK_EXT_SWAPCHAIN_COLOR_SPACE_EXTENSION_NAME))
	{
		AddInstanceExtensionNameIfNotExists(VK_EXT_SWAPCHAIN_COLOR_SPACE_EXTENSION_NAME, InstanceExtensions);
		VulkanRHIFeatures.SupportsSwapchainColorspace = true;
	}

	if (VulkanRHIFeatures.SupportsSurfaceCapabilities2 && ContainsExtension(VK_EXT_SURFACE_MAINTENANCE_1_EXTENSION_NAME))
	{
		AddInstanceExtensionNameIfNotExists(VK_EXT_SURFACE_MAINTENANCE_1_EXTENSION_NAME, InstanceExtensions);
		VulkanRHIFeatures.SupportsSurfaceMaintenance1 = true;
	}

	if (ContainsExtension(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME))
	{
		AddInstanceExtensionNameIfNotExists(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME, InstanceExtensions);
	}

	{
		TDynamicArray<const TChar*> PlatformExtensionNames;
		TVulkanPlatform::GetInstanceExtension(PlatformExtensionNames);

		for (const TChar* ExtensionName : PlatformExtensionNames)
		{
			if (ContainsExtension(ExtensionName))
			{
				AddInstanceExtensionNameIfNotExists(ExtensionName, InstanceExtensions);
			}
		}
	}

	const auto ContainsLayer = [&GlobalLayersAndExtensions](const TChar* LayerName) -> TBool 
	{
		return GlobalLayersAndExtensions.FindElementIf([LayerName](const TVulkanLayerAndExtension& LayerAndExtension) -> TBool
		{
			return ::strcmp(LayerAndExtension.LayerProperties.layerName, LayerName) == 0;
		}) != TDynamicArray<TVulkanLayerAndExtension>::Npos;
	};

#if defined(TRNT_DEBUG)
	TBool HasKHRValidation = false;
	TBool HasLunarGStandardValidation = false;

	if (VulkanRHIFeatures.EnableValidationLayer && ContainsLayer(TRNT_VK_LAYER_KHRONOS_VALIDATION))
	{
		InstanceLayers.EmplaceBack(TRNT_VK_LAYER_KHRONOS_VALIDATION);
		HasKHRValidation = true;
	}
	else
	{
		TLog::Warning<TRNT_GET_LOG_INFO(VulkanRHI)>("Could not find instance validation layer \"{}\"", TRNT_VK_LAYER_KHRONOS_VALIDATION);
	}

	if (!HasKHRValidation)
	{
		if (VulkanRHIFeatures.EnableValidationLayer && ContainsLayer(TRNT_VK_LAYER_LUNARG_STANDARD_VALIDATION))
		{
			InstanceLayers.EmplaceBack(TRNT_VK_LAYER_LUNARG_STANDARD_VALIDATION);
			HasLunarGStandardValidation = true;
		}
		else
		{
			TLog::Warning<TRNT_GET_LOG_INFO(VulkanRHI)>("Could not find instance validation layer \"{}\"", TRNT_VK_LAYER_LUNARG_STANDARD_VALIDATION);
		}
	}
	
	if (!HasKHRValidation && !HasLunarGStandardValidation && VulkanRHIFeatures.EnableValidationLayer)
	{
		static const TChar* ValidationArray[]
		{ 
			"VK_LAYER_GOOGLE_threading",
			"VK_LAYER_LUNARG_parameter_validation",
			"VK_LAYER_LUNARG_object_tracker",
			"VK_LAYER_LUNARG_core_validation",
			"VK_LAYER_GOOGLE_unique_objects"
		};

		TInt32 ValidationCount = TRNT_GET_ARRAY_LENGTH(ValidationArray);
		TBool FoundAnyLayer = false;

		for (TInt32 Index = 0; Index < ValidationCount; ++Index)
		{
			if (ContainsLayer(ValidationArray[Index]))
			{
				FoundAnyLayer = true;
				InstanceLayers.EmplaceBack(ValidationArray[Index]);
			}
			else
			{
				TLog::Warning<TRNT_GET_LOG_INFO(VulkanRHI)>("Could not find instance validation layer \"{}\"", ValidationArray[Index]);
			}
		}

		if (!FoundAnyLayer)
		{
			if (ContainsLayer(TRNT_VK_LAYER_LUNARG_CORE_VALIDATION))
			{
				InstanceLayers.EmplaceBack(TRNT_VK_LAYER_LUNARG_CORE_VALIDATION);
			}
			else
			{
				TLog::Warning<TRNT_GET_LOG_INFO(VulkanRHI)>("Could not find instance validation layer \"{}\"", TRNT_VK_LAYER_LUNARG_CORE_VALIDATION);
			}
		}
	}

	TBool Found = false;

	for (const TVulkanLayerAndExtension& LayerAndExtension : GlobalLayersAndExtensions)
	{
		for (const VkExtensionProperties& ExtensionProp : LayerAndExtension.ExtensionProperties)
		{
			if (::strcmp(VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME, ExtensionProp.extensionName) == 0)
			{
				Found = true;
				break;
			}
		}

		if (Found)
		{
			break;
		}
	}

	if (Found)
	{
		InstanceExtensions.EmplaceBack(VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME);
		VulkanRHIFeatures.HasValidationFeaturesExtension = true;
	}
#endif
}

TBool TVulkanRHI::CreateVulkanInstance()
{
	TUInt32 VKInstanceVersion = VK_API_VERSION_1_0;
	
	PFN_vkEnumerateInstanceVersion vkEnumerateInstanceVersion =	reinterpret_cast<PFN_vkEnumerateInstanceVersion>(TVulkanPlatform::VulkanGetInstanceProcAddr(nullptr, "vkEnumerateInstanceVersion"));

	if (vkEnumerateInstanceVersion)
	{
		TRNT_CHECK_VULKAN_RESULT(vkEnumerateInstanceVersion(&VKInstanceVersion));
	}

	VulkanInstanceVersion.Major = (TInt8)(VK_VERSION_MAJOR(VKInstanceVersion));
	VulkanInstanceVersion.Minor = (TInt8)(VK_VERSION_MINOR(VKInstanceVersion));
	VulkanInstanceVersion.Patch = (TInt8)(VK_VERSION_PATCH(VKInstanceVersion));

	TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>("Vulkan instance version: {}.{}.{}", VulkanInstanceVersion.Major, VulkanInstanceVersion.Minor, VulkanInstanceVersion.Patch);

	SetupInstanceLayersAndExtensions();

	TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>("================ Using instance extensions ================");
	for (const TChar* InstanceExtension : InstanceExtensions)
	{
		TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>("* \"{}\"", InstanceExtension);
	}

	TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>("================ Using instance layers ================");
	for (const TChar* InstanceLayer : InstanceLayers)
	{
		TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>("* \"{}\"", InstanceLayer);
	}

	VkApplicationInfo ApplicationInfo;
	TMemory::Memset(&ApplicationInfo, 0, sizeof(ApplicationInfo));

	ApplicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	ApplicationInfo.apiVersion = VK_MAKE_API_VERSION(0, VulkanInstanceVersion.Major, VulkanInstanceVersion.Minor, 0);

	ApplicationInfo.pEngineName = "Trinity";
	ApplicationInfo.engineVersion = VK_MAKE_VERSION(TEngineGlobals::EngineVersion.Major, TEngineGlobals::EngineVersion.Minor, TEngineGlobals::EngineVersion.Patch);

	TApplication* ApplicationInstancePtr = TApplication::GetApplicationInstance();
	ApplicationInfo.pApplicationName = (ApplicationInstancePtr == nullptr) ? "<Unknown>" : ApplicationInstancePtr->GetApplicationName().GetData();
	ApplicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);

	////////////////////// Create the Vulkan Instance //////////////////////
	VkInstanceCreateInfo InstanceCreateInfo;
	TMemory::Memset(&InstanceCreateInfo, 0, sizeof(InstanceCreateInfo));

	InstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	InstanceCreateInfo.pApplicationInfo = &ApplicationInfo;

	VkValidationFeaturesEXT ValidationFeatures;
	TMemory::Memset(&ValidationFeatures, 0, sizeof(ValidationFeatures));

	if (VulkanRHIFeatures.HasValidationFeaturesExtension)
	{
		ValidationFeatures.sType = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT;
		
		ValidationFeatures.pEnabledValidationFeatures = ValidationFeaturesEnable;
		ValidationFeatures.enabledValidationFeatureCount = TRNT_GET_ARRAY_LENGTH(ValidationFeaturesEnable);

		InstanceCreateInfo.pNext = &ValidationFeatures;
	}

	TInt64 InstanceExtensionCount = InstanceExtensions.GetElementCount();
	TInt64 InstanceLayerCount = InstanceLayers.GetElementCount();

	InstanceCreateInfo.enabledExtensionCount = static_cast<TUInt32>(InstanceExtensionCount);
	InstanceCreateInfo.ppEnabledExtensionNames = (InstanceExtensionCount == 0) ? nullptr : (const TChar* const*)InstanceExtensions.GetData();

	InstanceCreateInfo.enabledLayerCount = static_cast<TUInt32>(InstanceLayerCount);
	InstanceCreateInfo.ppEnabledLayerNames = (InstanceLayerCount == 0) ? nullptr : InstanceLayers.GetData();

#if defined(TRNT_PLATFORM_APPLE) && defined(VK_KHR_portability_enumeration)
	InstanceCreateInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif
	VkResult Result = VulkanPFNFunctions.CreateInstance(&InstanceCreateInfo, nullptr, &Instance);

	if (Result == VK_ERROR_INCOMPATIBLE_DRIVER)
	{
		TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>("Can't find a compatible Vulkan driver!");
		return false;
	}
	
	if (Result == VK_ERROR_EXTENSION_NOT_PRESENT)
	{
		TDynamicArray<VkExtensionProperties> ExtensionProperties;
		TUInt32 PropertyCount = EnumerateInstanceExtensionProperties(nullptr, ExtensionProperties);

		for (const TChar* Extension : InstanceExtensions)
		{
			TBool ExtensionFound = false;

			for (TUInt32 Index = 0; Index < PropertyCount; Index++)
			{
				if (!::strcmp(ExtensionProperties[Index].extensionName, Extension))
				{
					ExtensionFound = true;
					break;
				}
			}

			if (!ExtensionFound)
			{
				TLog::Warning<TRNT_GET_LOG_INFO(VulkanRHI)>("Missing required Vulkan extension: {}", Extension);
			}
		}

		return false;
	}

	if (Result != VK_SUCCESS)
	{
		TLog::Fatal<TRNT_GET_LOG_INFO(VulkanRHI)>("Failed to create Vulkan instance, error: {}", TVulkanUtils::ConvertVkResultToCString(Result));
		return false;
	}

	TLog::Success<TRNT_GET_LOG_INFO(VulkanRHI)>("Load Vulkan instance successfully!");

	if (Instance)
	{
		if (VulkanPFNFunctions.LoadVulkanPFNFunctions(Instance))
		{
			TLog::Success<TRNT_GET_LOG_INFO(VulkanRHI)>("Load Vulkan PFN functions successfully!");
		}
		else
		{
			TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>("Failed to load Vulkan PFN functions!");
		}
	}

	return true;
}

void TVulkanRHI::SetupDebugLayerCallback()
{
	if (!VulkanRHIFeatures.EnableValidationLayer)
	{
		return;
	}

#if defined(TRNT_DEBUG)
#if defined(TRNT_USE_VULKAN_DEBUG_UTILS)
	if (VulkanRHIFeatures.SupportsDebugUtils)
	{
		VkDebugUtilsMessengerCreateInfoEXT DebugUtilsMessengerCreateInfo;
		TMemory::Memset(&DebugUtilsMessengerCreateInfo, 0, sizeof(DebugUtilsMessengerCreateInfo));

		DebugUtilsMessengerCreateInfo.sType				= VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		DebugUtilsMessengerCreateInfo.pfnUserCallback	= DebugUtilsCallback;
		DebugUtilsMessengerCreateInfo.pUserData			= nullptr;
		DebugUtilsMessengerCreateInfo.messageType		= VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		DebugUtilsMessengerCreateInfo.messageSeverity	= /*VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |*/ VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
															VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

		TRNT_CHECK_VULKAN_RESULT(VKCreateDebugUtilsMessengerEXT(Instance, &DebugUtilsMessengerCreateInfo, nullptr, &DebugUtilsMessenger));
	}
#elif TRNT_USE_VULKAN_DEBUG_REPORT
	if (SupportsDebugReport)
	{
		VkDebugReportCallbackCreateInfoEXT DebugReportCallbackCreateInfo;
		TMemory::Memset(&DebugReportCallbackCreateInfo, 0, sizeof(DebugReportCallbackCreateInfo));

		DebugReportCallbackCreateInfo.sType			= VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
		DebugReportCallbackCreateInfo.pfnCallback	= DebugReportFunction;
		DebugReportCallbackCreateInfo.pUserData		= nullptr;
		DebugReportCallbackCreateInfo.pNext			= nullptr;
		DebugReportCallbackCreateInfo.flags			= VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
													  VK_DEBUG_REPORT_DEBUG_BIT_EXT | VK_DEBUG_REPORT_INFORMATION_BIT_EXT;

		VkResult Result = VKCreateDebugReportCallbackEXT(Instance, &DebugReportCallbackCreateInfo, nullptr, &DebugReportCallback);
		if (Result != VK_SUCCESS)
		{
			TLog::Warning<TRNT_GET_LOG_INFO(VulkanRHI)>("Failed to create debug report callback, error: {}", TVulkanUtils::ConvertVkResultToCString(Result));
		}
	}
#endif
#endif
}

VkPhysicalDevice TVulkanRHI::ChoosePhysicalDevice()
{
	TUInt32 PhysicalDeviceCount = 0;
	if (VulkanPFNFunctions.EnumeratePhysicalDevices(Instance, &PhysicalDeviceCount, nullptr) != VK_SUCCESS)
	{
		return VK_NULL_HANDLE;
	}

	if (PhysicalDeviceCount == 0)
	{
		return VK_NULL_HANDLE;
	}

	TDynamicArray<VkPhysicalDevice> PhysicalDevices;
	PhysicalDevices.Resize(static_cast<TInt64>(PhysicalDeviceCount));
	TRNT_CHECK_VULKAN_RESULT(VulkanPFNFunctions.EnumeratePhysicalDevices(Instance, &PhysicalDeviceCount, PhysicalDevices.GetData()));
	
	TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>("================ Found {} GPU(s) ================", PhysicalDeviceCount);
	for (TUInt32 Index = 0; Index < PhysicalDeviceCount; ++Index)
	{
		VkPhysicalDeviceProperties PhysicalDeviceProps;
		VulkanPFNFunctions.GetPhysicalDeviceProperties(PhysicalDevices[Index], &PhysicalDeviceProps);

		TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>("{}. {} (Device ID: {}).", Index + 1, PhysicalDeviceProps.deviceName, PhysicalDeviceProps.deviceID);
		TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>(
			"\tAPI version: {}.{}.{}", VK_VERSION_MAJOR(PhysicalDeviceProps.apiVersion), VK_VERSION_MINOR(PhysicalDeviceProps.apiVersion), VK_VERSION_PATCH(PhysicalDeviceProps.apiVersion));
		TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>(
			"\tDriver version: {}.{}.{}", VK_VERSION_MAJOR(PhysicalDeviceProps.driverVersion), VK_VERSION_MINOR(PhysicalDeviceProps.driverVersion), VK_VERSION_PATCH(PhysicalDeviceProps.driverVersion));
	}

	VkPhysicalDevice FinalPhysicalDevice = VK_NULL_HANDLE;

	TInt64 GPUIndex = -1;
	if (TCommandLine::GetInstance()->GetValueOf("-gpuindex", &GPUIndex))
	{
		if (GPUIndex >= 0 && GPUIndex < PhysicalDeviceCount)
		{
			FinalPhysicalDevice = PhysicalDevices[GPUIndex];
		}
		else
		{
			TLog::Warning<TRNT_GET_LOG_INFO(VulkanRHI)>("GPU index value is out of range, you should select GPU index between 0 and {}", PhysicalDeviceCount - 1);
			TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>("** Vulkan RHI will select the GPU automatically.");
		}
	}

	if (FinalPhysicalDevice == VK_NULL_HANDLE)
	{
		TInt32 MaxScore = 0;
		TInt64 PhysicalDeviceIndex = -1;

		for (TInt64 Index = 0; Index < PhysicalDeviceCount; ++Index)
		{
			TInt32 Score = RatePhysicalDeviceSuitability(PhysicalDevices[Index]);

			if (Score > MaxScore)
			{
				PhysicalDeviceIndex = Index;
				MaxScore = Score;
			}
		}

		if (MaxScore > 0 && PhysicalDeviceIndex != -1)
		{
			FinalPhysicalDevice = PhysicalDevices[PhysicalDeviceIndex];
		}
	}

	return FinalPhysicalDevice;
}

TInt32 TVulkanRHI::RatePhysicalDeviceSuitability(VkPhysicalDevice PhysicalDevice)
{
	TInt32 Score = 0;

	TUInt32 DeviceExtPropertyCount = 0;
	VulkanPFNFunctions.EnumerateDeviceExtensionProperties(PhysicalDevice, nullptr, &DeviceExtPropertyCount, nullptr);

	TDynamicArray<VkExtensionProperties> DeviceExtProperties;
	DeviceExtProperties.Resize(DeviceExtPropertyCount);
	VulkanPFNFunctions.EnumerateDeviceExtensionProperties(PhysicalDevice, nullptr, &DeviceExtPropertyCount, DeviceExtProperties.GetData());

	TBool FoundExtension = false;
	for (TInt64 Index = 0; Index < DeviceExtPropertyCount; ++Index)
	{
		if (::strcmp(DeviceExtProperties[Index].extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0)
		{
			FoundExtension = true;
			break;
		}
	}

	if (!FoundExtension)
	{
		return 0;
	}

	VkPhysicalDeviceProperties PhysicalDeviceProperties;
	VulkanPFNFunctions.GetPhysicalDeviceProperties(PhysicalDevice, &PhysicalDeviceProperties);

	if (PhysicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
	{
		Score += 10000;
	}

	Score += PhysicalDeviceProperties.limits.maxImageDimension2D;

	return Score;
}

TBool TVulkanRHI::CreateVulkanDevice(const TVulkanPhysicalDevice& SelectedPhysicalDevice)
{
	VulkanAPIVersion.Major = VK_API_VERSION_MAJOR(SelectedPhysicalDevice.PhysicalDeviceProperties.apiVersion);
	VulkanAPIVersion.Minor = VK_API_VERSION_MINOR(SelectedPhysicalDevice.PhysicalDeviceProperties.apiVersion);
	VulkanAPIVersion.Patch = VK_API_VERSION_PATCH(SelectedPhysicalDevice.PhysicalDeviceProperties.apiVersion);

	VulkanDevice = new TVulkanDevice(SelectedPhysicalDevice, this);

	if (!VulkanDevice->Initialize())
	{
		TLog::Fatal<TRNT_GET_LOG_INFO(VulkanRHI)>("Failed to initialize Vulkan device!");

		VulkanDevice->Destroy();

		delete VulkanDevice;
		VulkanDevice = nullptr;

		return false;
	}
	else
	{
		TLog::Success<TRNT_GET_LOG_INFO(VulkanRHI)>("Vulkan device has been initialized successful!");
	}
	return true;
}

#pragma warning( pop )

#endif