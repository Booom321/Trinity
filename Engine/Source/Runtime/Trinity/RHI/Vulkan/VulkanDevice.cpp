#include "TrinityPCH.h"

#include "VulkanUtils.h"
#include "VulkanDevice.h"

#if defined(TRNT_USE_VULKAN_RHI)

TVulkanDevice::TVulkanDevice(const TVulkanPhysicalDevice& PhysicalDevice, TVulkanRHI* VulkanRHIPointer)
	: Device(VK_NULL_HANDLE), VulkanRHIPointer(VulkanRHIPointer), PhysicalDevice(PhysicalDevice)
{

}

TVulkanDevice::~TVulkanDevice()
{

}

static TBool HasDeviceExtension(const TChar* ExtensionName, const TDynamicArray<VkExtensionProperties>& AvailableDeviceExtensions)
{
	const TInt64 FoundIndex = AvailableDeviceExtensions.FindElementIf([ExtensionName](const VkExtensionProperties& Extension) -> TBool 
	{
			return ::strcmp(ExtensionName, Extension.extensionName) == 0;
	});

	return FoundIndex != TDynamicArray<VkExtensionProperties>::Npos;
}

TBool TVulkanDevice::Initialize()
{
	TRNT_ASSERT(Device == VK_NULL_HANDLE && PhysicalDevice.Handle != VK_NULL_HANDLE && VulkanRHIPointer != nullptr);

	TDynamicArray<VkExtensionProperties> AvailableDeviceExtensions;
	TUInt32 DeviceExtensionCount = 0; 
	
	TRNT_CHECK_VULKAN_RESULT(TVulkanRHI::VulkanPFNFunctions.EnumerateDeviceExtensionProperties(PhysicalDevice.Handle, nullptr, &DeviceExtensionCount, nullptr));

	if (DeviceExtensionCount > 0)
	{
		AvailableDeviceExtensions.Resize(DeviceExtensionCount);
		TRNT_CHECK_VULKAN_RESULT(TVulkanRHI::VulkanPFNFunctions.EnumerateDeviceExtensionProperties(PhysicalDevice.Handle, nullptr, &DeviceExtensionCount, AvailableDeviceExtensions.GetData()));
	}

	if (HasDeviceExtension(VK_KHR_VIDEO_QUEUE_EXTENSION_NAME, AvailableDeviceExtensions))
	{
		VulkanRHIPointer->VulkanRHIFeatures.SupportsVideoQueue = true;
	}

	TVulkanRHI::VulkanPFNFunctions.GetPhysicalDeviceMemoryProperties(PhysicalDevice.Handle, &MemoryProperties);

	TUInt32 QueueFamilyCount = 0;
	TVulkanRHI::VulkanPFNFunctions.GetPhysicalDeviceQueueFamilyProperties2(PhysicalDevice.Handle, &QueueFamilyCount, nullptr);

	TDynamicArray<VkQueueFamilyProperties2> QueueFamilyProps;
	TDynamicArray<VkQueueFamilyVideoPropertiesKHR> QueueFamilyVideoProperties;
	TDynamicArray<VkDeviceQueueCreateInfo> QueueCreateInfos;
	TDynamicArray<TFloat> QueuePriorities;

	TUInt32 QueueFamilyIndices[(TUInt32)TVulkanQueueFlags::EMax];
	TMemory::Memset(QueueFamilyIndices, VK_QUEUE_FAMILY_IGNORED, sizeof(QueueFamilyIndices));

	if (QueueFamilyCount > 0)
	{
		QueueFamilyProps.Resize(static_cast<TInt64>(QueueFamilyCount));
		QueueFamilyVideoProperties.Resize(static_cast<TInt64>(QueueFamilyCount));

		for (TUInt32 Index = 0; Index < QueueFamilyCount; ++Index)
		{
			QueueFamilyProps[Index].sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2;
			if (VulkanRHIPointer->VulkanRHIFeatures.SupportsVideoQueue)
			{
				QueueFamilyProps[Index].pNext = &QueueFamilyVideoProperties[Index];
				QueueFamilyVideoProperties[Index].sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_VIDEO_PROPERTIES_KHR;
			}
		}

		TVulkanRHI::VulkanPFNFunctions.GetPhysicalDeviceQueueFamilyProperties2(PhysicalDevice.Handle, &QueueFamilyCount, QueueFamilyProps.GetData());

		for (TUInt32 Index = 0; Index < QueueFamilyCount; ++Index)
		{
			const VkQueueFamilyProperties2& CurrentQueueFamilyProps = QueueFamilyProps[Index];
			const VkQueueFamilyVideoPropertiesKHR& CurrentQueueFamilyVideoProps = QueueFamilyVideoProperties[Index];

			TBool IsValidQueue = false;

			if (CurrentQueueFamilyProps.queueFamilyProperties.queueCount > 0)
			{
				if ((CurrentQueueFamilyProps.queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) == VK_QUEUE_GRAPHICS_BIT)
				{
					if (QueueFamilyIndices[(TUInt32)TVulkanQueueFlags::EGraphicsQueue] == VK_QUEUE_FAMILY_IGNORED)
					{
						QueueFamilyIndices[(TUInt32)TVulkanQueueFlags::EGraphicsQueue] = static_cast<TUInt32>(Index);
						IsValidQueue = true;
					}
				}

				if ((CurrentQueueFamilyProps.queueFamilyProperties.queueFlags & VK_QUEUE_COMPUTE_BIT) == VK_QUEUE_COMPUTE_BIT)
				{
					if (QueueFamilyIndices[(TUInt32)TVulkanQueueFlags::EComputeQueue] == VK_QUEUE_FAMILY_IGNORED && QueueFamilyIndices[(TUInt32)TVulkanQueueFlags::EGraphicsQueue] != Index)
					{
						QueueFamilyIndices[(TUInt32)TVulkanQueueFlags::EComputeQueue] = static_cast<TUInt32>(Index);
						IsValidQueue = true;
					}
				}

				if ((CurrentQueueFamilyProps.queueFamilyProperties.queueFlags & VK_QUEUE_TRANSFER_BIT) == VK_QUEUE_TRANSFER_BIT)
				{
					if (QueueFamilyIndices[(TUInt32)TVulkanQueueFlags::ETransferQueue] == VK_QUEUE_FAMILY_IGNORED &&
						!(CurrentQueueFamilyProps.queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) && !(CurrentQueueFamilyProps.queueFamilyProperties.queueFlags & VK_QUEUE_COMPUTE_BIT))
					{
						QueueFamilyIndices[(TUInt32)TVulkanQueueFlags::ETransferQueue] = static_cast<TUInt32>(Index);
						IsValidQueue = true;
					}
				}
		
				if (VulkanRHIPointer->VulkanRHIFeatures.SupportsVideoQueue && (CurrentQueueFamilyProps.queueFamilyProperties.queueFlags & VK_QUEUE_VIDEO_DECODE_BIT_KHR) == VK_QUEUE_VIDEO_DECODE_BIT_KHR)
				{
					if (QueueFamilyIndices[(TUInt32)TVulkanQueueFlags::EDecodeVideoQueue] == VK_QUEUE_FAMILY_IGNORED &&
					   ((CurrentQueueFamilyVideoProps.videoCodecOperations & VK_VIDEO_CODEC_OPERATION_DECODE_H264_BIT_KHR) != 0 || (CurrentQueueFamilyVideoProps.videoCodecOperations & VK_VIDEO_CODEC_OPERATION_DECODE_H265_BIT_KHR) != 0))
					{
						QueueFamilyIndices[(TUInt32)TVulkanQueueFlags::EDecodeVideoQueue] = static_cast<TUInt32>(Index);
						IsValidQueue = true;
					}
				}

				if (VulkanRHIPointer->VulkanRHIFeatures.SupportsVideoQueue && (CurrentQueueFamilyProps.queueFamilyProperties.queueFlags & VK_QUEUE_VIDEO_ENCODE_BIT_KHR) == VK_QUEUE_VIDEO_ENCODE_BIT_KHR)
				{
					if (QueueFamilyIndices[(TUInt32)TVulkanQueueFlags::EEncodeVideoQueue] == VK_QUEUE_FAMILY_IGNORED &&
					   ((CurrentQueueFamilyVideoProps.videoCodecOperations & VK_VIDEO_CODEC_OPERATION_ENCODE_H264_BIT_KHR) != 0 || (CurrentQueueFamilyVideoProps.videoCodecOperations & VK_VIDEO_CODEC_OPERATION_ENCODE_H265_BIT_KHR) != 0))
					{
						QueueFamilyIndices[(TUInt32)TVulkanQueueFlags::EEncodeVideoQueue] = static_cast<TUInt32>(Index);
						IsValidQueue = true;
					}
				}
			}

			if (IsValidQueue)
			{
				static const TFloat QueuePriority = 1.0f;
				VkDeviceQueueCreateInfo QueueCreateInfo = {};
				TMemory::Memset(&QueueCreateInfo, 0, sizeof(QueueCreateInfo));

				QueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				QueueCreateInfo.queueFamilyIndex = static_cast<TUInt32>(Index);
				QueueCreateInfo.queueCount = CurrentQueueFamilyProps.queueFamilyProperties.queueCount;
				QueueCreateInfo.pQueuePriorities = &QueuePriority;

				QueueCreateInfos.EmplaceBack(QueueCreateInfo);

				TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>(
					"Using queue family: Index = {}, Count = {}, QueueFlags = {}", 
					Index, CurrentQueueFamilyProps.queueFamilyProperties.queueCount, TVulkanUtils::ConvertQueueFlagsToString(CurrentQueueFamilyProps.queueFamilyProperties.queueFlags).GetData());
			}
		}

		if (QueueFamilyIndices[(TUInt32)TVulkanQueueFlags::EGraphicsQueue] == VK_QUEUE_FAMILY_IGNORED)
		{
			TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>("Could not find queue family that supports VK_QUEUE_GRAPHICS_BIT!");
			return false;
		}
		else
		{
			TimestampValidBits = QueueFamilyProps[QueueFamilyIndices[(TUInt32)TVulkanQueueFlags::EGraphicsQueue]].queueFamilyProperties.timestampValidBits;
			TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>("TimestampValidBits = {}", TimestampValidBits);
		}

		if (QueueFamilyIndices[(TUInt32)TVulkanQueueFlags::EComputeQueue] == VK_QUEUE_FAMILY_IGNORED)
		{
			TLog::Warning<TRNT_GET_LOG_INFO(VulkanRHI)>("Compute queue not found! Fallback to graphics queue.");
			QueueFamilyIndices[(TUInt32)TVulkanQueueFlags::EComputeQueue] = QueueFamilyIndices[(TUInt32)TVulkanQueueFlags::EGraphicsQueue];
		}

		if (QueueFamilyIndices[(TUInt32)TVulkanQueueFlags::ETransferQueue] == VK_QUEUE_FAMILY_IGNORED)
		{
			TLog::Warning<TRNT_GET_LOG_INFO(VulkanRHI)>("Transfer queue not found! Fallback to compute queue.");
			QueueFamilyIndices[(TUInt32)TVulkanQueueFlags::ETransferQueue] = QueueFamilyIndices[(TUInt32)TVulkanQueueFlags::EComputeQueue];
		}

		if (VulkanRHIPointer->VulkanRHIFeatures.SupportsVideoQueue)
		{
			if (QueueFamilyIndices[(TUInt32)TVulkanQueueFlags::EDecodeVideoQueue] == VK_QUEUE_FAMILY_IGNORED)
			{
				TLog::Warning<TRNT_GET_LOG_INFO(VulkanRHI)>("Decode video queue not found!");
			}

			if (QueueFamilyIndices[(TUInt32)TVulkanQueueFlags::EEncodeVideoQueue] == VK_QUEUE_FAMILY_IGNORED)
			{
				TLog::Warning<TRNT_GET_LOG_INFO(VulkanRHI)>("Encode video queue not found!");
			}
		}
	}

	if (!GetEnabledDeviceExtensions(AvailableDeviceExtensions, QueueFamilyIndices))
	{
		return false;
	}

	GetEnabledPhysicalDeviceFeatures();

	VkDeviceCreateInfo DeviceCreateInfo = {};
	TMemory::Memset(&DeviceCreateInfo, 0, sizeof(DeviceCreateInfo));

	DeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	DeviceCreateInfo.queueCreateInfoCount = static_cast<TUInt32>(QueueCreateInfos.GetElementCount());
	DeviceCreateInfo.pQueueCreateInfos = QueueCreateInfos.IsEmpty() ? nullptr : QueueCreateInfos.GetData();

	DeviceCreateInfo.pNext = nullptr;

	DeviceCreateInfo.enabledExtensionCount = static_cast<TUInt32>(EnabledDeviceExtensions.GetElementCount());
	DeviceCreateInfo.ppEnabledExtensionNames = EnabledDeviceExtensions.IsEmpty() ? nullptr : EnabledDeviceExtensions.GetData();

	DeviceCreateInfo.pEnabledFeatures = &EnabledPhysicalDeviceFeatures;

	VkResult Result = TVulkanRHI::VulkanPFNFunctions.CreateDevice(PhysicalDevice.Handle, &DeviceCreateInfo, nullptr, &Device);

	if (Result != VK_SUCCESS)
	{
		return false;
	}

	for (TUInt32 Index = 0, QueueFamilyIndicesCount = TRNT_GET_ARRAY_LENGTH(QueueFamilyIndices); Index < QueueFamilyIndicesCount; ++Index)
	{
		if (QueueFamilyIndices[Index] != VK_QUEUE_FAMILY_IGNORED)
		{
			Queues[Index].QueueFamilyIndex = QueueFamilyIndices[Index];
			TVulkanRHI::VulkanPFNFunctions.GetDeviceQueue(Device, QueueFamilyIndices[Index], 0, &Queues[Index].QueueHandle);
		}
	}
	
	TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>("================ Enabling {} device extension(s) ================", EnabledDeviceExtensions.GetElementCount());
	for (const TChar* EnabledExtension : EnabledDeviceExtensions)
	{
		TLog::Info<TRNT_GET_LOG_INFO(VulkanRHI)>("* \"{}\"", EnabledExtension);
	}

	return true;
}

void TVulkanDevice::GetEnabledPhysicalDeviceFeatures()
{
	const VkPhysicalDeviceFeatures& PhysicalDeviceFeatures = PhysicalDevice.PhysicalDeviceFeatures;
	EnabledPhysicalDeviceFeatures = {};

	if (PhysicalDeviceFeatures.fullDrawIndexUint32)
	{
		EnabledPhysicalDeviceFeatures.fullDrawIndexUint32 = VK_TRUE;
	}

	if (PhysicalDeviceFeatures.imageCubeArray)
	{
		EnabledPhysicalDeviceFeatures.imageCubeArray = VK_TRUE;
	}

	if (PhysicalDeviceFeatures.independentBlend)
	{
		EnabledPhysicalDeviceFeatures.independentBlend = VK_TRUE;
	}

	if (PhysicalDeviceFeatures.geometryShader)
	{
		EnabledPhysicalDeviceFeatures.geometryShader = VK_TRUE;
	}

	if (PhysicalDeviceFeatures.tessellationShader)
	{
		EnabledPhysicalDeviceFeatures.tessellationShader = VK_TRUE;
	}

	if (PhysicalDeviceFeatures.sampleRateShading)
	{
		EnabledPhysicalDeviceFeatures.sampleRateShading = VK_TRUE;
	}

	if (PhysicalDeviceFeatures.dualSrcBlend)
	{
		EnabledPhysicalDeviceFeatures.dualSrcBlend = VK_TRUE;
	}

	if (PhysicalDeviceFeatures.logicOp)
	{
		EnabledPhysicalDeviceFeatures.logicOp = VK_TRUE;
	}

	if (PhysicalDeviceFeatures.multiDrawIndirect)
	{
		EnabledPhysicalDeviceFeatures.multiDrawIndirect = VK_TRUE;
	}

	if (PhysicalDeviceFeatures.depthClamp)
	{
		EnabledPhysicalDeviceFeatures.depthClamp = VK_TRUE;
	}

	if (PhysicalDeviceFeatures.depthBiasClamp)
	{
		EnabledPhysicalDeviceFeatures.depthBiasClamp = VK_TRUE;
	}

	if (PhysicalDeviceFeatures.fillModeNonSolid)
	{
		EnabledPhysicalDeviceFeatures.fillModeNonSolid = VK_TRUE;
	}

	if (PhysicalDeviceFeatures.depthBounds)
	{
		EnabledPhysicalDeviceFeatures.depthBounds = VK_TRUE;
	}

	if (PhysicalDeviceFeatures.wideLines)
	{
		EnabledPhysicalDeviceFeatures.wideLines = VK_TRUE;
	}

	if (PhysicalDeviceFeatures.largePoints)
	{
		EnabledPhysicalDeviceFeatures.largePoints = VK_TRUE;
	}

	if (PhysicalDeviceFeatures.alphaToOne)
	{
		EnabledPhysicalDeviceFeatures.alphaToOne = VK_TRUE;
	}

	if (PhysicalDeviceFeatures.multiViewport)
	{
		EnabledPhysicalDeviceFeatures.multiViewport = VK_TRUE;
	}

	if (PhysicalDeviceFeatures.textureCompressionETC2)
	{
		EnabledPhysicalDeviceFeatures.textureCompressionETC2 = VK_TRUE;
	}

	if (PhysicalDeviceFeatures.textureCompressionASTC_LDR)
	{
		EnabledPhysicalDeviceFeatures.textureCompressionASTC_LDR = VK_TRUE;
	}

	if (PhysicalDeviceFeatures.textureCompressionBC)
	{
		EnabledPhysicalDeviceFeatures.textureCompressionBC = VK_TRUE;
	}

	if (PhysicalDeviceFeatures.pipelineStatisticsQuery)
	{
		EnabledPhysicalDeviceFeatures.pipelineStatisticsQuery = VK_TRUE;
	}

	if (PhysicalDeviceFeatures.vertexPipelineStoresAndAtomics)
	{
		EnabledPhysicalDeviceFeatures.vertexPipelineStoresAndAtomics = VK_TRUE;
	}

	if (PhysicalDeviceFeatures.fragmentStoresAndAtomics)
	{
		EnabledPhysicalDeviceFeatures.fragmentStoresAndAtomics = VK_TRUE;
	}

	if (PhysicalDeviceFeatures.shaderTessellationAndGeometryPointSize)
	{
		EnabledPhysicalDeviceFeatures.shaderTessellationAndGeometryPointSize = VK_TRUE;
	}
	if (PhysicalDeviceFeatures.shaderImageGatherExtended)
	{
		EnabledPhysicalDeviceFeatures.shaderImageGatherExtended = VK_TRUE;
	}

	if (PhysicalDeviceFeatures.shaderStorageImageExtendedFormats)
	{
		EnabledPhysicalDeviceFeatures.shaderStorageImageExtendedFormats = VK_TRUE;
	}

	if (PhysicalDeviceFeatures.shaderStorageImageMultisample)
	{
		EnabledPhysicalDeviceFeatures.shaderStorageImageMultisample = VK_TRUE;
	}

	if (PhysicalDeviceFeatures.shaderStorageImageReadWithoutFormat)
	{
		EnabledPhysicalDeviceFeatures.shaderStorageImageReadWithoutFormat = VK_TRUE;
	}

	if (PhysicalDeviceFeatures.shaderStorageImageWriteWithoutFormat)
	{
		EnabledPhysicalDeviceFeatures.shaderStorageImageWriteWithoutFormat = VK_TRUE;
	}

	if (PhysicalDeviceFeatures.shaderUniformBufferArrayDynamicIndexing)
	{
		EnabledPhysicalDeviceFeatures.shaderUniformBufferArrayDynamicIndexing = VK_TRUE;
	}
	if (PhysicalDeviceFeatures.shaderSampledImageArrayDynamicIndexing)
	{
		EnabledPhysicalDeviceFeatures.shaderSampledImageArrayDynamicIndexing = VK_TRUE;
	}

	if (PhysicalDeviceFeatures.shaderStorageBufferArrayDynamicIndexing)
	{
		EnabledPhysicalDeviceFeatures.shaderStorageBufferArrayDynamicIndexing = VK_TRUE;
	}

	if (PhysicalDeviceFeatures.shaderClipDistance)
	{
		EnabledPhysicalDeviceFeatures.shaderClipDistance = VK_TRUE;
	}

	if (PhysicalDeviceFeatures.shaderCullDistance)
	{
		EnabledPhysicalDeviceFeatures.shaderCullDistance = VK_TRUE;
	}

	if (PhysicalDeviceFeatures.shaderFloat64)
	{
		EnabledPhysicalDeviceFeatures.shaderFloat64 = VK_TRUE;
	}

	if (PhysicalDeviceFeatures.shaderInt64)
	{
		EnabledPhysicalDeviceFeatures.shaderInt64 = VK_TRUE;
	}

	if (PhysicalDeviceFeatures.shaderInt16)
	{
		EnabledPhysicalDeviceFeatures.shaderInt16 = VK_TRUE;
	}

	if (PhysicalDeviceFeatures.shaderResourceMinLod)
	{
		EnabledPhysicalDeviceFeatures.shaderResourceMinLod = VK_TRUE;
	}

	if (PhysicalDeviceFeatures.variableMultisampleRate)
	{
		EnabledPhysicalDeviceFeatures.variableMultisampleRate = VK_TRUE;
	}
}

TBool TVulkanDevice::GetEnabledDeviceExtensions(const TDynamicArray<VkExtensionProperties>& AvailableDeviceExtensions, const TUInt32* QueueFamilyIndices)
{	
	if (HasDeviceExtension(VK_KHR_SWAPCHAIN_EXTENSION_NAME, AvailableDeviceExtensions))
	{
		VulkanRHIPointer->VulkanRHIFeatures.SupportsKHRSwapchain = true;

		EnabledDeviceExtensions.EmplaceBack(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	}

#if defined(TRNT_PLATFORM_MAC) || defined(TRNT_PLATFORM_IOS)
	if (HasDeviceExtension(VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME, AvailableDeviceExtensions))
	{
		EnabledDeviceExtensions.EmplaceBack(VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME);
	}
#endif

#if defined(TRNT_PLATFORM_WIN64)
	if (VulkanRHIPointer->VulkanRHIFeatures.SupportsSurfaceCapabilities2 && HasDeviceExtension(VK_EXT_FULL_SCREEN_EXCLUSIVE_EXTENSION_NAME, AvailableDeviceExtensions))
	{
		VulkanRHIPointer->VulkanRHIFeatures.SupportsFullscreenExclusive = true;
		EnabledDeviceExtensions.EmplaceBack(VK_EXT_FULL_SCREEN_EXCLUSIVE_EXTENSION_NAME);
	}
#endif

#if defined(TRNT_PLATFORM_WIN64)
	if (HasDeviceExtension(VK_KHR_EXTERNAL_SEMAPHORE_WIN32_EXTENSION_NAME, AvailableDeviceExtensions) && HasDeviceExtension(VK_KHR_EXTERNAL_MEMORY_WIN32_EXTENSION_NAME, AvailableDeviceExtensions))
	{
		VulkanRHIPointer->VulkanRHIFeatures.SupportsExternalSemaphore = true;
		VulkanRHIPointer->VulkanRHIFeatures.SupportsExternalMemory = true;

		EnabledDeviceExtensions.EmplaceBack(VK_KHR_EXTERNAL_SEMAPHORE_WIN32_EXTENSION_NAME);
		EnabledDeviceExtensions.EmplaceBack(VK_KHR_EXTERNAL_MEMORY_WIN32_EXTENSION_NAME);
	}
#elif defined(TRNT_PLATFORM_LINUX) || defined(TRNT_PLATFORM_UNIX) || defined(TRNT_PLATFORM_POSIX)
	if (HasDeviceExtension(VK_KHR_EXTERNAL_SEMAPHORE_FD_EXTENSION_NAME, AvailableDeviceExtensions) && HasDeviceExtension(VK_KHR_EXTERNAL_MEMORY_FD_EXTENSION_NAME, AvailableDeviceExtensions))
	{
		VulkanRHIPointer->VulkanRHIFeatures.SupportsExternalSemaphore = true;
		VulkanRHIPointer->VulkanRHIFeatures.SupportsExternalMemory = true;

		EnabledDeviceExtensions.EmplaceBack(VK_KHR_EXTERNAL_SEMAPHORE_FD_EXTENSION_NAME);
		EnabledDeviceExtensions.EmplaceBack(VK_KHR_EXTERNAL_MEMORY_FD_EXTENSION_NAME);
	}
#else
	if (HasDeviceExtension(VK_KHR_EXTERNAL_SEMAPHORE_EXTENSION_NAME, AvailableDeviceExtensions) && HasDeviceExtension(VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME, AvailableDeviceExtensions))
	{
		VulkanRHIPointer->VulkanRHIFeatures.SupportsExternalSemaphore = true;
		VulkanRHIPointer->VulkanRHIFeatures.SupportsExternalMemory = true;

		EnabledDeviceExtensions.EmplaceBack(VK_KHR_EXTERNAL_SEMAPHORE_EXTENSION_NAME);
		EnabledDeviceExtensions.EmplaceBack(VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME);
	}
#endif

	if (HasDeviceExtension(VK_EXT_CONSERVATIVE_RASTERIZATION_EXTENSION_NAME, AvailableDeviceExtensions))
	{
		VulkanRHIPointer->VulkanRHIFeatures.SupportsConservativeRasterization = true;
		EnabledDeviceExtensions.EmplaceBack(VK_EXT_CONSERVATIVE_RASTERIZATION_EXTENSION_NAME);
	}

	if (PhysicalDevice.PhysicalDeviceProperties.apiVersion < VK_API_VERSION_1_2)
	{
		if (!HasDeviceExtension(VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME, AvailableDeviceExtensions))
		{
			TLog::Warning<TRNT_GET_LOG_INFO(VulkanRHI)>("VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME is not supported!");
			return false;
		}
		EnabledDeviceExtensions.EmplaceBack(VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME);

		if (HasDeviceExtension(VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME, AvailableDeviceExtensions))
		{
			VulkanRHIPointer->VulkanRHIFeatures.SupportsFormatList = true;
			EnabledDeviceExtensions.EmplaceBack(VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME);
		}
	}
	else
	{
		VulkanRHIPointer->VulkanRHIFeatures.SupportsFormatList = true;
	}

	if (HasDeviceExtension(VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME, AvailableDeviceExtensions))
	{
		VulkanRHIPointer->VulkanRHIFeatures.SupportsAcceleratorStructure = true;
		EnabledDeviceExtensions.EmplaceBack(VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME);
	}

	if (HasDeviceExtension(VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME, AvailableDeviceExtensions))
	{
		VulkanRHIPointer->VulkanRHIFeatures.SupportsRayTracingPipeline = true;
		EnabledDeviceExtensions.EmplaceBack(VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME);
	}

	if (HasDeviceExtension(VK_KHR_RAY_QUERY_EXTENSION_NAME, AvailableDeviceExtensions))
	{
		VulkanRHIPointer->VulkanRHIFeatures.SupportsRayQuery = true;
		EnabledDeviceExtensions.EmplaceBack(VK_KHR_RAY_QUERY_EXTENSION_NAME);
	}

	if (HasDeviceExtension(VK_KHR_PIPELINE_LIBRARY_EXTENSION_NAME, AvailableDeviceExtensions))
	{
		VulkanRHIPointer->VulkanRHIFeatures.SupportsPipelineLibrary = true;
		EnabledDeviceExtensions.EmplaceBack(VK_KHR_PIPELINE_LIBRARY_EXTENSION_NAME);
	}

	if (HasDeviceExtension(VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME, AvailableDeviceExtensions))
	{
		VulkanRHIPointer->VulkanRHIFeatures.SupportsDeferredHostOperations = true;
		EnabledDeviceExtensions.EmplaceBack(VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME);
	}

	if (HasDeviceExtension(VK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME, AvailableDeviceExtensions))
	{
		VulkanRHIPointer->VulkanRHIFeatures.SupportsPerformanceQuery = true;
		EnabledDeviceExtensions.EmplaceBack(VK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME);
	}

	if (HasDeviceExtension(VK_EXT_DEPTH_CLIP_ENABLE_EXTENSION_NAME, AvailableDeviceExtensions))
	{
		VulkanRHIPointer->VulkanRHIFeatures.SupportsDepthClipEnable = true;
		EnabledDeviceExtensions.EmplaceBack(VK_EXT_DEPTH_CLIP_ENABLE_EXTENSION_NAME);
	}

	if (HasDeviceExtension(VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME, AvailableDeviceExtensions))
	{
		VulkanRHIPointer->VulkanRHIFeatures.SupportsPushDescriptor = true;
		EnabledDeviceExtensions.EmplaceBack(VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME);
	}

	if (VulkanRHIPointer->VulkanRHIFeatures.SupportsVideoQueue)
	{
		EnabledDeviceExtensions.EmplaceBack(VK_KHR_VIDEO_QUEUE_EXTENSION_NAME);

		if (HasDeviceExtension(VK_KHR_VIDEO_DECODE_QUEUE_EXTENSION_NAME, AvailableDeviceExtensions))
		{
			VulkanRHIPointer->VulkanRHIFeatures.SupportsVideoDecodeQueue = true;
			EnabledDeviceExtensions.EmplaceBack(VK_KHR_VIDEO_DECODE_QUEUE_EXTENSION_NAME);

			TBool HasDecodeVideoQueueIndex = QueueFamilyIndices[(TUInt32)TVulkanQueueFlags::EDecodeVideoQueue] != VK_QUEUE_FAMILY_IGNORED;

			if (HasDecodeVideoQueueIndex)
			{
				if (HasDeviceExtension(VK_KHR_VIDEO_DECODE_H264_EXTENSION_NAME, AvailableDeviceExtensions))
				{
					VulkanRHIPointer->VulkanRHIFeatures.SupportsVideoDecodeH264 = true;
					EnabledDeviceExtensions.EmplaceBack(VK_KHR_VIDEO_DECODE_H264_EXTENSION_NAME);
				}

				if (HasDeviceExtension(VK_KHR_VIDEO_DECODE_H265_EXTENSION_NAME, AvailableDeviceExtensions))
				{
					VulkanRHIPointer->VulkanRHIFeatures.SupportsVideoDecodeH265 = true;
					EnabledDeviceExtensions.EmplaceBack(VK_KHR_VIDEO_DECODE_H265_EXTENSION_NAME);
				}
			}
		}

		if (HasDeviceExtension(VK_KHR_VIDEO_ENCODE_QUEUE_EXTENSION_NAME, AvailableDeviceExtensions))
		{
			VulkanRHIPointer->VulkanRHIFeatures.SupportsVideoEncodeQueue = true;
			EnabledDeviceExtensions.EmplaceBack(VK_KHR_VIDEO_ENCODE_QUEUE_EXTENSION_NAME);

			TBool HasEncodeVideoQueueIndex = QueueFamilyIndices[(TUInt32)TVulkanQueueFlags::EEncodeVideoQueue] != VK_QUEUE_FAMILY_IGNORED;

			if (HasEncodeVideoQueueIndex)
			{
				if (HasDeviceExtension(VK_KHR_VIDEO_ENCODE_H264_EXTENSION_NAME, AvailableDeviceExtensions))
				{
					VulkanRHIPointer->VulkanRHIFeatures.SupportsVideoEncodeH264 = true;
					EnabledDeviceExtensions.EmplaceBack(VK_KHR_VIDEO_ENCODE_H264_EXTENSION_NAME);
				}

				if (HasDeviceExtension(VK_KHR_VIDEO_ENCODE_H265_EXTENSION_NAME, AvailableDeviceExtensions))
				{
					VulkanRHIPointer->VulkanRHIFeatures.SupportsVideoEncodeH265 = true;
					EnabledDeviceExtensions.EmplaceBack(VK_KHR_VIDEO_ENCODE_H265_EXTENSION_NAME);
				}
			}
		}
	}

	if (HasDeviceExtension(VK_EXT_MESH_SHADER_EXTENSION_NAME, AvailableDeviceExtensions))
	{
		EnabledDeviceExtensions.EmplaceBack(VK_EXT_MESH_SHADER_EXTENSION_NAME);
	}

	if (HasDeviceExtension(VK_EXT_PAGEABLE_DEVICE_LOCAL_MEMORY_EXTENSION_NAME, AvailableDeviceExtensions))
	{
		EnabledDeviceExtensions.EmplaceBack(VK_EXT_PAGEABLE_DEVICE_LOCAL_MEMORY_EXTENSION_NAME);
	}

	if (HasDeviceExtension(VK_NV_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME, AvailableDeviceExtensions))
	{
		EnabledDeviceExtensions.EmplaceBack(VK_NV_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME);
	}

	if (HasDeviceExtension(VK_NV_DEVICE_GENERATED_COMMANDS_COMPUTE_EXTENSION_NAME, AvailableDeviceExtensions))
	{
		EnabledDeviceExtensions.EmplaceBack(VK_NV_DEVICE_GENERATED_COMMANDS_COMPUTE_EXTENSION_NAME);
	}

	if (HasDeviceExtension(VK_EXT_INDEX_TYPE_UINT8_EXTENSION_NAME, AvailableDeviceExtensions))
	{
		EnabledDeviceExtensions.EmplaceBack(VK_EXT_INDEX_TYPE_UINT8_EXTENSION_NAME);
	}

	if (HasDeviceExtension(VK_KHR_FRAGMENT_SHADER_BARYCENTRIC_EXTENSION_NAME, AvailableDeviceExtensions))
	{
		EnabledDeviceExtensions.EmplaceBack(VK_KHR_FRAGMENT_SHADER_BARYCENTRIC_EXTENSION_NAME);
	}

	if (VulkanRHIPointer->VulkanRHIFeatures.SupportsKHRSwapchain)
	{
		if (HasDeviceExtension(VK_KHR_PRESENT_ID_EXTENSION_NAME, AvailableDeviceExtensions))
		{
			EnabledDeviceExtensions.EmplaceBack(VK_KHR_PRESENT_ID_EXTENSION_NAME);
		}

		if (HasDeviceExtension(VK_KHR_PRESENT_WAIT_EXTENSION_NAME, AvailableDeviceExtensions))
		{
			EnabledDeviceExtensions.EmplaceBack(VK_KHR_PRESENT_WAIT_EXTENSION_NAME);
		}

		if (VulkanRHIPointer->VulkanRHIFeatures.SupportsSurfaceMaintenance1 && HasDeviceExtension(VK_EXT_SWAPCHAIN_MAINTENANCE_1_EXTENSION_NAME, AvailableDeviceExtensions))
		{
			EnabledDeviceExtensions.EmplaceBack(VK_EXT_SWAPCHAIN_MAINTENANCE_1_EXTENSION_NAME);
		}

		if (VulkanRHIPointer->VulkanRHIFeatures.SupportsSwapchainColorspace && HasDeviceExtension(VK_EXT_HDR_METADATA_EXTENSION_NAME, AvailableDeviceExtensions))
		{
			VulkanRHIPointer->VulkanRHIFeatures.SupportsHDRMetadata = true;
			EnabledDeviceExtensions.EmplaceBack(VK_EXT_HDR_METADATA_EXTENSION_NAME);
		}
	}

	return true;
}

void TVulkanDevice::Destroy()
{
	if (Device)
	{
		TVulkanRHI::VulkanPFNFunctions.DeviceWaitIdle(Device);
		TVulkanRHI::VulkanPFNFunctions.DestroyDevice(Device, nullptr);
		Device = VK_NULL_HANDLE;
	}
}

#endif