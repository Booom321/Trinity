#include "TrinityPCH.h"

#if defined(TRNT_SUPPORT_VULKAN_RHI)

#include "VulkanUtils.h"

const TChar* TVulkanUtils::ConvertVkResultToCString(VkResult Result)
{
	switch (Result)
	{
	case VK_SUCCESS:
		return TRNT_STRINGIFY(VK_SUCCESS);
	case VK_NOT_READY:
		return TRNT_STRINGIFY(VK_NOT_READY);
	case VK_TIMEOUT:
		return TRNT_STRINGIFY(VK_TIMEOUT);
	case VK_EVENT_SET:
		return TRNT_STRINGIFY(VK_EVENT_SET);
	case VK_EVENT_RESET:
		return TRNT_STRINGIFY(VK_EVENT_RESET);
	case VK_INCOMPLETE:
		return TRNT_STRINGIFY(VK_INCOMPLETE);
	case VK_ERROR_OUT_OF_HOST_MEMORY:
		return TRNT_STRINGIFY(VK_ERROR_OUT_OF_HOST_MEMORY);
	case VK_ERROR_OUT_OF_DEVICE_MEMORY:
		return TRNT_STRINGIFY(VK_ERROR_OUT_OF_DEVICE_MEMORY);
	case VK_ERROR_INITIALIZATION_FAILED:
		return TRNT_STRINGIFY(VK_ERROR_INITIALIZATION_FAILED);
	case VK_ERROR_DEVICE_LOST:
		return TRNT_STRINGIFY(VK_ERROR_DEVICE_LOST);
	case VK_ERROR_MEMORY_MAP_FAILED:
		return TRNT_STRINGIFY(VK_ERROR_MEMORY_MAP_FAILED);
	case VK_ERROR_LAYER_NOT_PRESENT:
		return TRNT_STRINGIFY(VK_ERROR_LAYER_NOT_PRESENT);
	case VK_ERROR_EXTENSION_NOT_PRESENT:
		return TRNT_STRINGIFY(VK_ERROR_EXTENSION_NOT_PRESENT);
	case VK_ERROR_FEATURE_NOT_PRESENT:
		return TRNT_STRINGIFY(VK_ERROR_FEATURE_NOT_PRESENT);
	case VK_ERROR_INCOMPATIBLE_DRIVER:
		return TRNT_STRINGIFY(VK_ERROR_INCOMPATIBLE_DRIVER);
	case VK_ERROR_TOO_MANY_OBJECTS:
		return TRNT_STRINGIFY(VK_ERROR_TOO_MANY_OBJECTS);
	case VK_ERROR_FORMAT_NOT_SUPPORTED:
		return TRNT_STRINGIFY(VK_ERROR_FORMAT_NOT_SUPPORTED);
	case VK_ERROR_FRAGMENTED_POOL:
		return TRNT_STRINGIFY(VK_ERROR_FRAGMENTED_POOL);
	case VK_ERROR_UNKNOWN:
		return TRNT_STRINGIFY(VK_ERROR_UNKNOWN);
	case VK_ERROR_OUT_OF_POOL_MEMORY:
		return TRNT_STRINGIFY(VK_ERROR_OUT_OF_POOL_MEMORY);
	case VK_ERROR_INVALID_EXTERNAL_HANDLE:
		return TRNT_STRINGIFY(VK_ERROR_INVALID_EXTERNAL_HANDLE);
	case VK_ERROR_FRAGMENTATION:
		return TRNT_STRINGIFY(VK_ERROR_FRAGMENTATION);
	case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
		return TRNT_STRINGIFY(VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS);
	case VK_PIPELINE_COMPILE_REQUIRED:
		return TRNT_STRINGIFY(VK_PIPELINE_COMPILE_REQUIRED);
	case VK_ERROR_SURFACE_LOST_KHR:
		return TRNT_STRINGIFY(VK_ERROR_SURFACE_LOST_KHR);
	case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
		return TRNT_STRINGIFY(VK_ERROR_NATIVE_WINDOW_IN_USE_KHR);
	case VK_SUBOPTIMAL_KHR:
		return TRNT_STRINGIFY(VK_SUBOPTIMAL_KHR);
	case VK_ERROR_OUT_OF_DATE_KHR:
		return TRNT_STRINGIFY(VK_ERROR_OUT_OF_DATE_KHR);
	case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
		return TRNT_STRINGIFY(VK_ERROR_INCOMPATIBLE_DISPLAY_KHR);
	case VK_ERROR_VALIDATION_FAILED_EXT:
		return TRNT_STRINGIFY(VK_ERROR_VALIDATION_FAILED_EXT);
	case VK_ERROR_INVALID_SHADER_NV:
		return TRNT_STRINGIFY(VK_ERROR_INVALID_SHADER_NV);
	case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR:
		return TRNT_STRINGIFY(VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR);
	case VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR:
		return TRNT_STRINGIFY(VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR);
	case VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR:
		return TRNT_STRINGIFY(VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR);
	case VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR:
		return TRNT_STRINGIFY(VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR);
	case VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR:
		return TRNT_STRINGIFY(VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR);
	case VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR:
		return TRNT_STRINGIFY(VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR);
	case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
		return TRNT_STRINGIFY(VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT);
	case VK_ERROR_NOT_PERMITTED_KHR:
		return TRNT_STRINGIFY(VK_ERROR_NOT_PERMITTED_KHR);
	case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
		return TRNT_STRINGIFY(VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT);
	case VK_THREAD_IDLE_KHR:
		return TRNT_STRINGIFY(VK_THREAD_IDLE_KHR);
	case VK_THREAD_DONE_KHR:
		return TRNT_STRINGIFY(VK_THREAD_DONE_KHR);
	case VK_OPERATION_DEFERRED_KHR:
		return TRNT_STRINGIFY(VK_OPERATION_DEFERRED_KHR);
	case VK_OPERATION_NOT_DEFERRED_KHR:
		return TRNT_STRINGIFY(VK_OPERATION_NOT_DEFERRED_KHR);
	case VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR:
		return TRNT_STRINGIFY(VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR);
	case VK_ERROR_COMPRESSION_EXHAUSTED_EXT:
		return TRNT_STRINGIFY(VK_ERROR_COMPRESSION_EXHAUSTED_EXT);
	case VK_ERROR_INCOMPATIBLE_SHADER_BINARY_EXT:
		return TRNT_STRINGIFY(VK_ERROR_INCOMPATIBLE_SHADER_BINARY_EXT);
	case VK_RESULT_MAX_ENUM:
		return TRNT_STRINGIFY(VK_RESULT_MAX_ENUM);
	}

	return "";
}

const TChar* TVulkanUtils::ConvertPhysicalDeviceTypeToCString(VkPhysicalDeviceType PhysicalDeviceType)
{
	switch (PhysicalDeviceType)
	{
	case VK_PHYSICAL_DEVICE_TYPE_OTHER:
		return "Other";
	case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
		return "Integrated GPU";
	case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
		return "Discrete GPU";
	case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
		return "Virtual GPU";
	case VK_PHYSICAL_DEVICE_TYPE_CPU:
		return "CPU";
	default:
		break;
	}
	return "Unknown";
}

const TChar* TVulkanUtils::ConvertVendorIDToVendorName(TUInt32 VendorID)
{
	switch (VendorID)
	{
	case 0x1002:
		return "AMD";
	case 0x1010:
		return "ImgTec";
	case 0x10DE:
		return "NVIDIA";
	case 0x13B5:
		return "ARM";
	case 0x5143:
		return "Qualcomm";
	case 0x8086:
		return "Intel";
	}
	return "Unknown";
}

TString TVulkanUtils::ConvertQueueFlagsToString(const VkQueueFlags QueueFlags)
{
	TString FinalOutput;

	if ((QueueFlags & VK_QUEUE_GRAPHICS_BIT) == VK_QUEUE_GRAPHICS_BIT)
	{
		FinalOutput += "Graphics ";
	}

	if ((QueueFlags & VK_QUEUE_COMPUTE_BIT) == VK_QUEUE_COMPUTE_BIT)
	{
		FinalOutput += "Compute ";
	}

	if ((QueueFlags & VK_QUEUE_TRANSFER_BIT) == VK_QUEUE_TRANSFER_BIT)
	{
		FinalOutput += "Transfer ";
	}

	if ((QueueFlags & VK_QUEUE_SPARSE_BINDING_BIT) == VK_QUEUE_SPARSE_BINDING_BIT)
	{
		FinalOutput += "Sparse ";
	}

	FinalOutput.TrimEndInternal();

	return FinalOutput;
}

const TChar* TVulkanUtils::ConvertShaderStageToCString(TShaderStage ShaderStage)
{
	switch (ShaderStage)
	{
	case TShaderStage::EVertex:
		return "Vertex";
	case TShaderStage::ETessellationControl:
		return "TessellationControl";
	case TShaderStage::ETessellationEvaluation:
		return "TessellationEvaluation";
	case TShaderStage::EGeometry:
		return "Geometry";
	case TShaderStage::EFragment:
		return "Fragment";
	case TShaderStage::ECompute:
		return "Compute";
	case TShaderStage::EHull:
		return "Hull";
	case TShaderStage::EDomain:
		return "Domain";
	case TShaderStage::EPixel:
		return "Pixel";
	case TShaderStage::EMax:
		return "Max";
	}

	return "<Unknown>";
}

VkShaderStageFlagBits TVulkanUtils::ConvertTShaderStageToVkStage(TShaderStage ShaderStage)
{
	switch (ShaderStage)
	{
	case TShaderStage::EVertex:
		return VK_SHADER_STAGE_VERTEX_BIT;
	case TShaderStage::EHull:
	case TShaderStage::ETessellationControl:
		return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
	case TShaderStage::EDomain:
	case TShaderStage::ETessellationEvaluation:
		return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
	case TShaderStage::EGeometry:
		return VK_SHADER_STAGE_GEOMETRY_BIT;
	case TShaderStage::EFragment:
		return VK_SHADER_STAGE_FRAGMENT_BIT;
	case TShaderStage::EPixel:
	case TShaderStage::ECompute:
		return VK_SHADER_STAGE_COMPUTE_BIT;
	}
	return VK_SHADER_STAGE_ALL;
}

VkPolygonMode TVulkanUtils::ConvertTPolygonModeToVkPolygonMode(TPolygonMode PolygonMode)
{
	switch (PolygonMode)
	{
	case TPolygonMode::EFill:
		return VK_POLYGON_MODE_FILL;
	case TPolygonMode::ELine:
		return VK_POLYGON_MODE_LINE;
	case TPolygonMode::EPoint:
		return VK_POLYGON_MODE_POINT;
	case TPolygonMode::ERectangleNV:
		return VK_POLYGON_MODE_FILL_RECTANGLE_NV;
	case TPolygonMode::EMax:
		return VK_POLYGON_MODE_MAX_ENUM;
	}
	return VK_POLYGON_MODE_FILL;
}

VkCullModeFlagBits TVulkanUtils::ConvertTCullModeToVkCullMode(TCullMode CullMode)
{
	switch (CullMode)
	{
	case TCullMode::ENONE:
		return VK_CULL_MODE_NONE;
	case TCullMode::EFrontBit:
		return VK_CULL_MODE_FRONT_BIT;
	case TCullMode::EBackBit:
		return VK_CULL_MODE_BACK_BIT;
	case TCullMode::EFrontAndBack:
		return VK_CULL_MODE_FRONT_AND_BACK;
	case TCullMode::EMax:
		return VK_CULL_MODE_FLAG_BITS_MAX_ENUM;
	}
	return VK_CULL_MODE_NONE;
}

VkPrimitiveTopology TVulkanUtils::ConvertTPrimitiveTopologyToVkPrimitiveTopology(TPrimitiveTopology Topology)
{
	switch (Topology)
	{
	case TPrimitiveTopology::EPointList:
		return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
	case TPrimitiveTopology::ELineList:
		return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
	case TPrimitiveTopology::ELineStrip:
		return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
	case TPrimitiveTopology::ETriangleList:
		return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	case TPrimitiveTopology::ETriangleStrip:
		return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
	case TPrimitiveTopology::ETriangleFan:
		return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
	case TPrimitiveTopology::ELineListWithAdjacency:
		return VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY;
	case TPrimitiveTopology::ELineStripWithAdjacency:
		return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY;
	case TPrimitiveTopology::ETriangleListWithAdjacency:
		return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY;
	case TPrimitiveTopology::ETriangleStripWithAdjacency:
		return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY;
	case TPrimitiveTopology::EPatchList:
		return VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
	case TPrimitiveTopology::EMax:
		return VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
	}
	return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST; // return default topology
}

VkFrontFace TVulkanUtils::ConvertTFrontFaceToVkFrontFace(TFrontFace FrontFace)
{
	switch (FrontFace)
	{
	case TFrontFace::ECounterClockwise:
		return VK_FRONT_FACE_COUNTER_CLOCKWISE;
	case TFrontFace::EClockwise:
		return VK_FRONT_FACE_CLOCKWISE;
	case TFrontFace::EMax:
		return VK_FRONT_FACE_MAX_ENUM;
	}

	return VK_FRONT_FACE_CLOCKWISE;
}

#endif