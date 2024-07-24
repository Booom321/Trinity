#pragma once

#if defined(TRNT_SUPPORT_VULKAN_RHI)

#include "Trinity/RHI/RHI.h"

#include "VulkanPlatform.h"
#include "IncludeVulkanHeader.h"

#undef CreateSemaphore 

class TRNT_API TVulkanPFNFunctions
{
public:
	PFN_vkEnumerateInstanceExtensionProperties EnumerateInstanceExtensionProperties = nullptr;
	PFN_vkEnumerateInstanceLayerProperties EnumerateInstanceLayerProperties = nullptr;
	PFN_vkCreateInstance CreateInstance = nullptr;

#if defined(TRNT_PLATFORM_WIN64)
	PFN_vkCreateWin32SurfaceKHR CreateWin32SurfaceKHR = nullptr;
#endif
	PFN_vkDestroyInstance DestroyInstance = nullptr;

	PFN_vkGetPhysicalDeviceProperties GetPhysicalDeviceProperties = nullptr;
	PFN_vkGetPhysicalDeviceFeatures GetPhysicalDeviceFeatures = nullptr;
	PFN_vkEnumeratePhysicalDevices EnumeratePhysicalDevices = nullptr;
	PFN_vkEnumerateDeviceExtensionProperties EnumerateDeviceExtensionProperties = nullptr;
	PFN_vkGetPhysicalDeviceQueueFamilyProperties2 GetPhysicalDeviceQueueFamilyProperties2 = nullptr;
	PFN_vkGetPhysicalDeviceMemoryProperties GetPhysicalDeviceMemoryProperties = nullptr;

	PFN_vkCreateDevice CreateDevice = nullptr;
	PFN_vkGetDeviceQueue GetDeviceQueue = nullptr;
	PFN_vkDeviceWaitIdle DeviceWaitIdle = nullptr;
	PFN_vkDestroyDevice DestroyDevice = nullptr;

	PFN_vkGetPhysicalDeviceSurfaceFormatsKHR GetPhysicalDeviceSurfaceFormatsKHR = nullptr;
	PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR GetPhysicalDeviceSurfaceCapabilitiesKHR = nullptr;
	PFN_vkGetPhysicalDeviceSurfacePresentModesKHR GetPhysicalDeviceSurfacePresentModesKHR = nullptr;
	PFN_vkGetPhysicalDeviceSurfaceSupportKHR GetPhysicalDeviceSurfaceSupportKHR = nullptr;

	PFN_vkCreateSwapchainKHR CreateSwapchainKHR = nullptr;
	PFN_vkDestroySurfaceKHR DestroySurfaceKHR = nullptr;
	PFN_vkDestroySwapchainKHR DestroySwapchainKHR = nullptr;

	PFN_vkGetSwapchainImagesKHR GetSwapchainImagesKHR = nullptr;
	PFN_vkCreateImageView CreateImageView = nullptr;
	PFN_vkDestroyImageView DestroyImageView = nullptr;

	PFN_vkCreateShaderModule CreateShaderModule = nullptr;
	PFN_vkDestroyShaderModule DestroyShaderModule = nullptr;

	PFN_vkCreateRenderPass CreateRenderPass = nullptr;
	PFN_vkDestroyRenderPass DestroyRenderPass = nullptr;

	PFN_vkCreatePipelineLayout CreatePipelineLayout = nullptr;
	PFN_vkDestroyPipelineLayout DestroyPipelineLayout = nullptr;
	PFN_vkCreateGraphicsPipelines CreateGraphicsPipelines = nullptr;
	PFN_vkDestroyPipeline DestroyPipeline = nullptr;

	PFN_vkCreateDescriptorSetLayout CreateDescriptorSetLayout = nullptr;
	PFN_vkDestroyDescriptorSetLayout DestroyDescriptorSetLayout = nullptr;

	PFN_vkGetPhysicalDeviceFormatProperties GetPhysicalDeviceFormatProperties = nullptr;

	PFN_vkCreateFramebuffer CreateFramebuffer = nullptr;
	PFN_vkDestroyFramebuffer DestroyFramebuffer = nullptr;

	PFN_vkCreateImage CreateImage = nullptr;
	PFN_vkDestroyImage DestroyImage = nullptr;
	PFN_vkAllocateMemory AllocateMemory = nullptr;
	PFN_vkFreeMemory FreeMemory = nullptr;
	PFN_vkBindImageMemory BindImageMemory = nullptr;
	PFN_vkGetImageMemoryRequirements GetImageMemoryRequirements = nullptr;

	PFN_vkCreateCommandPool CreateCommandPool = nullptr;
	PFN_vkDestroyCommandPool DestroyCommandPool = nullptr;
	PFN_vkAllocateCommandBuffers AllocateCommandBuffers = nullptr;
	PFN_vkFreeCommandBuffers FreeCommandBuffers = nullptr;

	PFN_vkBeginCommandBuffer BeginCommandBuffer = nullptr;
	PFN_vkEndCommandBuffer EndCommandBuffer = nullptr;
	PFN_vkResetCommandBuffer ResetCommandBuffer = nullptr;
	PFN_vkCmdBeginRenderPass CmdBeginRenderPass = nullptr;
	PFN_vkCmdBeginRenderPass2KHR CmdBeginRenderPass2KHR = nullptr;
	PFN_vkCmdEndRenderPass CmdEndRenderPass = nullptr;
	PFN_vkCmdEndRenderPass2KHR CmdEndRenderPass2KHR = nullptr;

	PFN_vkCreateSemaphore CreateSemaphore = nullptr;
	PFN_vkCreateFence CreateFence = nullptr;
	PFN_vkDestroySemaphore DestroySemaphore = nullptr;
	PFN_vkDestroyFence DestroyFence = nullptr;
	PFN_vkWaitForFences WaitForFences = nullptr;
	PFN_vkResetFences ResetFences = nullptr;
	PFN_vkAcquireNextImageKHR AcquireNextImageKHR = nullptr;

	PFN_vkQueueSubmit QueueSubmit = nullptr;
	PFN_vkQueuePresentKHR QueuePresentKHR = nullptr;
	PFN_vkQueueWaitIdle QueueWaitIdle = nullptr;

	TBool LoadVulkanInstanceFunctions();
	TBool LoadVulkanPFNFunctions(VkInstance Instance);
};

class TVulkanLayerAndExtension
{
public:
	TVulkanLayerAndExtension() = default;
	~TVulkanLayerAndExtension() = default;

	VkLayerProperties LayerProperties;
	TDynamicArray<VkExtensionProperties> ExtensionProperties;
};

class TVulkanRHIFeatures
{
public:
	TVulkanRHIFeatures() = default;
	~TVulkanRHIFeatures() = default;

	static void Reset(TVulkanRHIFeatures& Features);

	TBool SupportsDebugUtils = false;
	TBool SupportsDebugReport = false;
	TBool EnableValidationLayer = false;
	TBool SupportsSwapchainColorspace = false;
	TBool SupportsSurfaceMaintenance1 = false;
	TBool SupportsSurfaceCapabilities2 = false;
	TBool HasValidationFeaturesExtension = false;
	TBool SupportsVideoQueue = false;
	TBool SupportsKHRSwapchain = false;
	TBool SupportsFullscreenExclusive = false;
	TBool SupportsExternalSemaphore = false;
	TBool SupportsExternalMemory = false;
	TBool SupportsConservativeRasterization = false;
	TBool SupportsFormatList = false;
	TBool SupportsAcceleratorStructure = false;
	TBool SupportsRayTracingPipeline = false;
	TBool SupportsRayQuery = false;
	TBool SupportsPipelineLibrary = false;
	TBool SupportsDeferredHostOperations = false;
	TBool SupportsPerformanceQuery = false;
	TBool SupportsDepthClipEnable = false;

	TBool SupportsVideoDecodeQueue = false;
	TBool SupportsVideoEncodeQueue = false;
	TBool SupportsVideoDecodeH264 = false;
	TBool SupportsVideoDecodeH265 = false;
	TBool SupportsVideoEncodeH264 = false;
	TBool SupportsVideoEncodeH265 = false;

	TBool SupportsPushDescriptor = false;

	TBool SupportsHDRMetadata = false;
	TBool SupportsCreateRenderPass2KHR = false;
};

class TVulkanPhysicalDevice
{
public:
	TVulkanPhysicalDevice(VkPhysicalDevice PhysicalDevice);

	TVulkanPhysicalDevice(const TVulkanPhysicalDevice&) = default;
	TVulkanPhysicalDevice& operator=(const TVulkanPhysicalDevice&) = default;

	~TVulkanPhysicalDevice();

	VkPhysicalDevice Handle;
	VkPhysicalDeviceFeatures PhysicalDeviceFeatures;
	VkPhysicalDeviceProperties PhysicalDeviceProperties;
};

class TVulkanDevice;

class TRNT_API TVulkanRHI : public TGenericRHI
{
public:
	friend class TVulkanPlatform;
	friend class TVulkanDevice;
	friend class TVulkanPhysicalDevice;
	friend class TVulkanCommandBuffer; 

	TVulkanRHI();
	virtual ~TVulkanRHI();

public:
	virtual TBool Initialize() override;
	virtual void Shutdown() override;
	virtual const TChar* GetName() override;
	virtual TVersion GetVersion() const override;
	virtual TGraphicsAPI GetGraphicsAPI() override;

public:
	static TRNT_NODISCARD TRNT_FORCE_INLINE TVulkanRHI* GetGlobalVulkanRHIInstance() { return static_cast<TVulkanRHI*>(GlobalRHIInstance); }

	TRNT_NODISCARD TRNT_FORCE_INLINE const TVersion GetVulkanInstanceVersion() const { return VulkanInstanceVersion; }
	TRNT_NODISCARD TRNT_FORCE_INLINE const VkInstance GetVkInstance() const { return Instance; }

	TRNT_NODISCARD TRNT_FORCE_INLINE TDynamicArray<const TChar*>& GetInstanceLayers() { return InstanceLayers; }
	TRNT_NODISCARD TRNT_FORCE_INLINE TDynamicArray<const TChar*>& GetInstanceExtensions() { return InstanceExtensions; }
	TRNT_NODISCARD TRNT_FORCE_INLINE const TDynamicArray<const TChar*>& GetInstanceLayers() const { return InstanceLayers; }
	TRNT_NODISCARD TRNT_FORCE_INLINE const TDynamicArray<const TChar*>& GetInstanceExtensions() const { return InstanceExtensions; }

	TRNT_NODISCARD TRNT_FORCE_INLINE TVulkanRHIFeatures& GetVulkanRHIFeatures() { return VulkanRHIFeatures; }
	TRNT_NODISCARD TRNT_FORCE_INLINE const TVulkanRHIFeatures& GetVulkanRHIFeatures() const { return VulkanRHIFeatures; }

	TRNT_NODISCARD TRNT_FORCE_INLINE TVulkanDevice* GetVulkanDevice() { return VulkanDevice; }
	TRNT_NODISCARD TRNT_FORCE_INLINE const TVulkanDevice* GetVulkanDevice() const { return VulkanDevice; }
	TRNT_NODISCARD TRNT_FORCE_INLINE const TVersion GetVulkanAPIVersion() const { return VulkanAPIVersion; }

	static TVulkanPFNFunctions VulkanPFNFunctions;

private:
	VkInstance Instance;

	TVersion VulkanInstanceVersion;
	TVulkanRHIFeatures VulkanRHIFeatures;


	TDynamicArray<const TChar*> InstanceExtensions;
	TDynamicArray<const TChar*> InstanceLayers;

	static TUInt32 EnumerateInstanceExtensionProperties(const TChar* LayerName, TDynamicArray<VkExtensionProperties>& AvailableExtensionProps);
	static TUInt32 EnumerateInstanceLayerProperties(TDynamicArray<VkLayerProperties>& AvailableExtensionProps);

	void SetupInstanceLayersAndExtensions();
	TBool CreateVulkanInstance();
	void SetupDebugLayerCallback();

#if defined(TRNT_DEBUG)
#	if defined(TRNT_USE_VULKAN_DEBUG_UTILS)

	VkDebugUtilsMessengerEXT DebugUtilsMessenger = VK_NULL_HANDLE;
	static VKAPI_ATTR VkBool32 VKAPI_PTR DebugUtilsCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT MessageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT MessageType,
		const VkDebugUtilsMessengerCallbackDataEXT* CallbackData,
		void* UserData);
	static VkResult VKCreateDebugUtilsMessengerEXT(
		VkInstance Instance, const VkDebugUtilsMessengerCreateInfoEXT* DebugUtilsMessengerCreateInfo, const VkAllocationCallbacks* AllocationCallbacks,	VkDebugUtilsMessengerEXT* DebugUtilsMessenger);

	static void VKDestroyDebugUtilsMessengerEXT(VkInstance Instance, VkDebugUtilsMessengerEXT DebugUtilsMessenger, const VkAllocationCallbacks* AllocationCallbacks);

#	elif defined(TRNT_USE_VULKAN_DEBUG_REPORT)
	VkDebugReportCallbackEXT DebugReportCallback = VK_NULL_HANDLE;
	static VkBool32 VKAPI_PTR DebugReportFunction(
		VkDebugReportFlagsEXT ReportFlags,
		VkDebugReportObjectTypeEXT ObjectType,
		TUInt64 SourceObject,
		TSize_T Location,
		TInt32 MessageCode,
		const TChar* LayerPrefix,
		const TChar* Message,
		void* UserData);
	static VkResult VKCreateDebugReportCallbackEXT(
		VkInstance Instance, const VkDebugReportCallbackCreateInfoEXT* DebugReportCallbackCreateInfo, const VkAllocationCallbacks* AllocationCallbacks,	VkDebugReportCallbackEXT* DebugReportCallback);

	static void VKDestroyDebugReportCallbackEXT(VkInstance Instance, VkDebugReportCallbackEXT DebugReportCallback, const VkAllocationCallbacks* AllocationCallbacks);

#	endif
#endif

private:
	TVulkanDevice* VulkanDevice;
	TVersion VulkanAPIVersion;
	VkPhysicalDevice ChoosePhysicalDevice();
	TBool CreateVulkanDevice(const TVulkanPhysicalDevice& SelectedPhysicalDevice);
	static TInt32 RatePhysicalDeviceSuitability(VkPhysicalDevice PhysicalDevice);
};

#endif