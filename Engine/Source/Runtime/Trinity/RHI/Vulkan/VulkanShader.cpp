#include "TrinityPCH.h"

#ifdef TRNT_SUPPORT_VULKAN_RHI

	#include "Trinity/Core/Platform/FileIO.h"
	#include "Trinity/Core/Platform/FileSystem.h"
	#include "Trinity/Core/String/PathUtils.h"
	#include "VulkanDevice.h"
	#include "VulkanShader.h"
	#include "VulkanUtils.h"

	#include <SPIRV/GlslangToSpv.h>
	#include <glslang/Public/ShaderLang.h>

	#include <spirv_cross.hpp>


////////////////////////////////////////////////////////////////////////////////////////////////////////
//// TVulkanShaderIncluder /////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
TRNT_DECLARE_LOG_INFO(VulkanShaderIncluder, TLogLevel::EDebug);

class TVulkanShaderIncluder : public glslang::TShader::Includer
{
public:
	TVulkanShaderIncluder() = default;
	virtual ~TVulkanShaderIncluder() = default;

	IncludeResult* includeLocal(const TChar* HeaderName, const TChar* IncluderName, TSize_T InclusionDepth) override
	{
		TLog::Warning<TRNT_GET_LOG_INFO(VulkanShaderIncluder)>("TVulkanShaderIncluder::includeLocal() is not implemented:");
		TLog::Warning<TRNT_GET_LOG_INFO(VulkanShaderIncluder)>("-> Header name: {}", HeaderName);
		TLog::Warning<TRNT_GET_LOG_INFO(VulkanShaderIncluder)>("-> Includer name: {}", IncluderName);
		TLog::Warning<TRNT_GET_LOG_INFO(VulkanShaderIncluder)>("-> Inclusion depth: {}", InclusionDepth);

		return nullptr;
	}

	IncludeResult* includeSystem(const TChar* HeaderName, const TChar* IncluderName, TSize_T InclusionDepth) override
	{
		TLog::Warning<TRNT_GET_LOG_INFO(VulkanShaderIncluder)>("TVulkanShaderIncluder::includeSystem() is not implemented:");
		TLog::Warning<TRNT_GET_LOG_INFO(VulkanShaderIncluder)>("-> Header name: {}", HeaderName);
		TLog::Warning<TRNT_GET_LOG_INFO(VulkanShaderIncluder)>("-> Includer name: {}", IncluderName);
		TLog::Warning<TRNT_GET_LOG_INFO(VulkanShaderIncluder)>("-> Inclusion depth: {}", InclusionDepth);

		return nullptr;
	}

	void releaseInclude(IncludeResult* Result) override
	{
		TLog::Warning<TRNT_GET_LOG_INFO(VulkanShaderIncluder)>("TVulkanShaderIncluder::releaseInclude() is not implemented:");
		return;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//// TVulkanShaderCompiler /////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
TRNT_DECLARE_LOG_INFO(VulkanShaderCompiler, TLogLevel::EDebug);

class TVulkanShaderCompiler
{
public:
	static TBool ReadSourceCodeFromFile(const TString& FilePath, TString* OutSourceCode);
	static TBool CompileIntoSPIRVCode(TVersion ClientVersion, TShaderStage ShaderStage, const TChar* SourceCode, TDynamicArray<TUInt32>& SPIRVData);

private:
	static EShLanguage ConvertTShaderStageToEShLanguage(TShaderStage ShaderStage);
	static TFileReader FileReader;
};

TFileReader TVulkanShaderCompiler::FileReader{};

TBool TVulkanShaderCompiler::ReadSourceCodeFromFile(const TString& FilePath, TString* OutSourceCode)
{
	TBool Ok = FileReader.Open(FilePath.GetData()) && OutSourceCode;
	if (Ok)
	{
		*OutSourceCode = FileReader.ReadTextFile();
	}
	FileReader.Close();
	return Ok;
}

void BuildGlslangResource(TBuiltInResource& Resource)
{
	Resource = {};
	Resource.maxLights = 32;
	Resource.maxClipPlanes = 6;
	Resource.maxTextureUnits = 32;
	Resource.maxTextureCoords = 32;
	Resource.maxVertexAttribs = 64;
	Resource.maxVertexUniformComponents = 4096;
	Resource.maxVaryingFloats = 64;
	Resource.maxVertexTextureImageUnits = 32;
	Resource.maxCombinedTextureImageUnits = 80;
	Resource.maxTextureImageUnits = 32;
	Resource.maxFragmentUniformComponents = 4096;
	Resource.maxDrawBuffers = 32;
	Resource.maxVertexUniformVectors = 128;
	Resource.maxVaryingVectors = 8;
	Resource.maxFragmentUniformVectors = 16;
	Resource.maxVertexOutputVectors = 16;
	Resource.maxFragmentInputVectors = 15;
	Resource.minProgramTexelOffset = -8;
	Resource.maxProgramTexelOffset = 7;
	Resource.maxClipDistances = 8;
	Resource.maxComputeWorkGroupCountX = 65535;
	Resource.maxComputeWorkGroupCountY = 65535;
	Resource.maxComputeWorkGroupCountZ = 65535;
	Resource.maxComputeWorkGroupSizeX = 1024;
	Resource.maxComputeWorkGroupSizeY = 1024;
	Resource.maxComputeWorkGroupSizeZ = 64;
	Resource.maxComputeUniformComponents = 1024;
	Resource.maxComputeTextureImageUnits = 16;
	Resource.maxComputeImageUniforms = 8;
	Resource.maxComputeAtomicCounters = 8;
	Resource.maxComputeAtomicCounterBuffers = 1;
	Resource.maxVaryingComponents = 60;
	Resource.maxVertexOutputComponents = 64;
	Resource.maxGeometryInputComponents = 64;
	Resource.maxGeometryOutputComponents = 128;
	Resource.maxFragmentInputComponents = 128;
	Resource.maxImageUnits = 8;
	Resource.maxCombinedImageUnitsAndFragmentOutputs = 8;
	Resource.maxCombinedShaderOutputResources = 8;
	Resource.maxImageSamples = 0;
	Resource.maxVertexImageUniforms = 0;
	Resource.maxTessControlImageUniforms = 0;
	Resource.maxTessEvaluationImageUniforms = 0;
	Resource.maxGeometryImageUniforms = 0;
	Resource.maxFragmentImageUniforms = 8;
	Resource.maxCombinedImageUniforms = 8;
	Resource.maxGeometryTextureImageUnits = 16;
	Resource.maxGeometryOutputVertices = 256;
	Resource.maxGeometryTotalOutputComponents = 1024;
	Resource.maxGeometryUniformComponents = 1024;
	Resource.maxGeometryVaryingComponents = 64;
	Resource.maxTessControlInputComponents = 128;
	Resource.maxTessControlOutputComponents = 128;
	Resource.maxTessControlTextureImageUnits = 16;
	Resource.maxTessControlUniformComponents = 1024;
	Resource.maxTessControlTotalOutputComponents = 4096;
	Resource.maxTessEvaluationInputComponents = 128;
	Resource.maxTessEvaluationOutputComponents = 128;
	Resource.maxTessEvaluationTextureImageUnits = 16;
	Resource.maxTessEvaluationUniformComponents = 1024;
	Resource.maxTessPatchComponents = 120;
	Resource.maxPatchVertices = 32;
	Resource.maxTessGenLevel = 64;
	Resource.maxViewports = 16;
	Resource.maxVertexAtomicCounters = 0;
	Resource.maxTessControlAtomicCounters = 0;
	Resource.maxTessEvaluationAtomicCounters = 0;
	Resource.maxGeometryAtomicCounters = 0;
	Resource.maxFragmentAtomicCounters = 8;
	Resource.maxCombinedAtomicCounters = 8;
	Resource.maxAtomicCounterBindings = 1;
	Resource.maxVertexAtomicCounterBuffers = 0;
	Resource.maxTessControlAtomicCounterBuffers = 0;
	Resource.maxTessEvaluationAtomicCounterBuffers = 0;
	Resource.maxGeometryAtomicCounterBuffers = 0;
	Resource.maxFragmentAtomicCounterBuffers = 1;
	Resource.maxCombinedAtomicCounterBuffers = 1;
	Resource.maxAtomicCounterBufferSize = 16384;
	Resource.maxTransformFeedbackBuffers = 4;
	Resource.maxTransformFeedbackInterleavedComponents = 64;
	Resource.maxCullDistances = 8;
	Resource.maxCombinedClipAndCullDistances = 8;
	Resource.maxSamples = 4;
	Resource.maxMeshOutputVerticesNV = 256,
	Resource.maxMeshOutputPrimitivesNV = 512,
	Resource.maxMeshWorkGroupSizeX_NV = 32,
	Resource.maxMeshWorkGroupSizeY_NV = 1,
	Resource.maxMeshWorkGroupSizeZ_NV = 1,
	Resource.maxTaskWorkGroupSizeX_NV = 32,
	Resource.maxTaskWorkGroupSizeY_NV = 1,
	Resource.maxTaskWorkGroupSizeZ_NV = 1,
	Resource.maxMeshViewCountNV = 4,
	Resource.maxMeshOutputVerticesEXT = 256,
	Resource.maxMeshOutputPrimitivesEXT = 256,
	Resource.maxMeshWorkGroupSizeX_EXT = 128,
	Resource.maxMeshWorkGroupSizeY_EXT = 128,
	Resource.maxMeshWorkGroupSizeZ_EXT = 128,
	Resource.maxTaskWorkGroupSizeX_EXT = 128,
	Resource.maxTaskWorkGroupSizeY_EXT = 128,
	Resource.maxTaskWorkGroupSizeZ_EXT = 128,
	Resource.maxMeshViewCountEXT = 4,
	Resource.maxDualSourceDrawBuffersEXT = 1,
	Resource.limits.nonInductiveForLoops = true;
	Resource.limits.whileLoops = true;
	Resource.limits.doWhileLoops = true;
	Resource.limits.generalUniformIndexing = true;
	Resource.limits.generalAttributeMatrixVectorIndexing = true;
	Resource.limits.generalVaryingIndexing = true;
	Resource.limits.generalSamplerIndexing = true;
	Resource.limits.generalVariableIndexing = true;
	Resource.limits.generalConstantMatrixVectorIndexing = true;
}

TBool TVulkanShaderCompiler::CompileIntoSPIRVCode(TVersion ClientVersion, TShaderStage ShaderStage, const TChar* SourceCode, TDynamicArray<TUInt32>& SPIRVData)
{
	glslang::InitializeProcess();
	EShLanguage ShaderLanguage = ConvertTShaderStageToEShLanguage(ShaderStage);
	glslang::TProgram Program{};
	glslang::TShader Shader(ShaderLanguage);

	TBuiltInResource Resource;
	BuildGlslangResource(Resource);

	#if TRNT_DEBUG
	const EShMessages Messages = static_cast<EShMessages>(EShMsgSpvRules | EShMsgVulkanRules | EShMsgDefault | EShMsgDebugInfo);
	#else
	const EShMessages Messages = static_cast<EShMessages>(EShMsgSpvRules | EShMsgVulkanRules | EShMsgDefault);
	#endif
	const TChar* Sources[1] = { SourceCode };
	Shader.setStrings(Sources, 1);

	const TInt32 DefaultVersion = 450;
	glslang::EshTargetClientVersion TargetAPIVersion = static_cast<glslang::EshTargetClientVersion>(VK_MAKE_API_VERSION(0, ClientVersion.Major, ClientVersion.Minor, 0));
	Shader.setEnvClient(glslang::EShClientVulkan, TargetAPIVersion);
	Shader.setEnvTarget(glslang::EshTargetSpv, (TargetAPIVersion >= glslang::EShTargetVulkan_1_1) ? glslang::EShTargetSpv_1_3 : glslang::EShTargetSpv_1_0);
	Shader.setEnvInput(glslang::EShSourceGlsl, ShaderLanguage, glslang::EShClientVulkan, DefaultVersion);
	Shader.setEntryPoint("main");
	Shader.setSourceEntryPoint("main");

	const TBool ForwardCompatible = false;
	const TBool ForceDefaultVersionAndProfile = false;
	const EProfile Profile = EProfile::ENoProfile;

	TVulkanShaderIncluder ShaderIncluder;
	std::string OutputString;

	/*if (!Shader.preprocess(&Resource, DefaultVersion, Profile, ForceDefaultVersionAndProfile, ForwardCompatible, Messages, &OutputString, ShaderIncluder))
	{
		TLog::Error<TRNT_GET_LOG_INFO(VulkanShaderCompiler)>("Failed to process shader:");
		TLog::Error<TRNT_GET_LOG_INFO(VulkanShaderCompiler)>("-> Info log: {}", Shader.getInfoLog());
		TLog::Error<TRNT_GET_LOG_INFO(VulkanShaderCompiler)>("-> Info debug log: {}", Shader.getInfoDebugLog());
		glslang::FinalizeProcess();

		return false;
	}*/

	if (!Shader.parse(&Resource, DefaultVersion, Profile, ForceDefaultVersionAndProfile, ForwardCompatible, Messages, ShaderIncluder))
	{
		TLog::Error<TRNT_GET_LOG_INFO(VulkanShaderCompiler)>("Failed to parse shader:");
		TLog::Error<TRNT_GET_LOG_INFO(VulkanShaderCompiler)>("-> Info log: {}", Shader.getInfoLog());
		TLog::Error<TRNT_GET_LOG_INFO(VulkanShaderCompiler)>("-> Info debug log: {}", Shader.getInfoDebugLog());
		glslang::FinalizeProcess();

		return false;
	}

	Program.addShader(&Shader);

	if (!Program.link(Messages))
	{
		TLog::Error<TRNT_GET_LOG_INFO(VulkanShaderCompiler)>("Failed to link shader:");
		TLog::Error<TRNT_GET_LOG_INFO(VulkanShaderCompiler)>("-> Info log: {}", Program.getInfoLog());
		TLog::Error<TRNT_GET_LOG_INFO(VulkanShaderCompiler)>("-> Info debug log: {}", Program.getInfoDebugLog());
		glslang::FinalizeProcess();

		return false;
	}

	glslang::SpvOptions SpvOptions;
	#ifdef TRNT_DEBUG
	SpvOptions.generateDebugInfo = true;
	SpvOptions.disableOptimizer = true;
	SpvOptions.optimizeSize = false;
	#else
	SpvOptions.generateDebugInfo = false;
	SpvOptions.disableOptimizer = false;
	SpvOptions.optimizeSize = true;
	#endif

	spv::SpvBuildLogger BuildLogger;
	std::vector<TUInt32> SPIRV;
	glslang::GlslangToSpv(*Program.getIntermediate(ShaderLanguage), SPIRV, &BuildLogger, &SpvOptions);
	glslang::FinalizeProcess();

	TSize_T SPIRVCodeSize = SPIRV.size();
	SPIRVData.Clear();
	SPIRVData.Append(SPIRVCodeSize, SPIRV.data());

	return true;
}

EShLanguage TVulkanShaderCompiler::ConvertTShaderStageToEShLanguage(TShaderStage ShaderStage)
{
	switch (ShaderStage)
	{
		case TShaderStage::EVertex:
			return EShLangVertex;
		case TShaderStage::ETessellationControl:
			return EShLangTessControl;
		case TShaderStage::ETessellationEvaluation:
			return EShLangTessEvaluation;
		case TShaderStage::EGeometry:
			return EShLangGeometry;
		case TShaderStage::EFragment:
			return EShLangFragment;
		case TShaderStage::ECompute:
			return EShLangCompute;
	}

	return EShLangVertex;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//// TVulkanShader /////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
static VkFormat ConvertSPIRVTypeToVkFormat(const spirv_cross::SPIRType& SPIRType)
{
	if (SPIRType.basetype == spirv_cross::SPIRType::Struct || SPIRType.basetype == spirv_cross::SPIRType::Sampler)
	{
		return VK_FORMAT_UNDEFINED;
	}
	else if (SPIRType.basetype == spirv_cross::SPIRType::Image || SPIRType.basetype == spirv_cross::SPIRType::SampledImage)
	{
		switch (SPIRType.image.format)
		{
			case spv::ImageFormatR8:
				return VK_FORMAT_R8_UNORM;
			case spv::ImageFormatR8Snorm:
				return VK_FORMAT_R8_SNORM;
			case spv::ImageFormatR8ui:
				return VK_FORMAT_R8_UINT;
			case spv::ImageFormatR8i:
				return VK_FORMAT_R8_SINT;
			case spv::ImageFormatRg8:
				return VK_FORMAT_R8G8_UNORM;
			case spv::ImageFormatRg8Snorm:
				return VK_FORMAT_R8G8_SNORM;
			case spv::ImageFormatRg8ui:
				return VK_FORMAT_R8G8_UINT;
			case spv::ImageFormatRg8i:
				return VK_FORMAT_R8G8_SINT;
			case spv::ImageFormatRgba8i:
				return VK_FORMAT_R8G8B8A8_SINT;
			case spv::ImageFormatRgba8ui:
				return VK_FORMAT_R8G8B8A8_UINT;
			case spv::ImageFormatRgba8:
				return VK_FORMAT_R8G8B8A8_UNORM;
			case spv::ImageFormatRgba8Snorm:
				return VK_FORMAT_R8G8B8A8_SNORM;
			case spv::ImageFormatR32i:
				return VK_FORMAT_R32_SINT;
			case spv::ImageFormatR32ui:
				return VK_FORMAT_R32_UINT;
			case spv::ImageFormatRg32i:
				return VK_FORMAT_R32G32_SINT;
			case spv::ImageFormatRg32ui:
				return VK_FORMAT_R32G32_UINT;
			case spv::ImageFormatRgba32f:
				return VK_FORMAT_R32G32B32A32_SFLOAT;
			case spv::ImageFormatRgba16f:
				return VK_FORMAT_R16G16B16A16_SFLOAT;
			case spv::ImageFormatR32f:
				return VK_FORMAT_R32_SFLOAT;
			case spv::ImageFormatRg32f:
				return VK_FORMAT_R32G32_SFLOAT;
			case spv::ImageFormatR16f:
				return VK_FORMAT_R16_SFLOAT;
			case spv::ImageFormatRgba32i:
				return VK_FORMAT_R32G32B32A32_SINT;
			case spv::ImageFormatRgba32ui:
				return VK_FORMAT_R32G32B32A32_UINT;
			default:
				return VK_FORMAT_UNDEFINED;
		}
	}
	else if (SPIRType.vecsize == 1)
	{
		if (SPIRType.width == 8)
		{
			switch (SPIRType.basetype)
			{
				case spirv_cross::SPIRType::Int:
					return VK_FORMAT_R8_SINT;
				case spirv_cross::SPIRType::UInt:
					return VK_FORMAT_R8_UINT;
				default:
					return VK_FORMAT_UNDEFINED;
			}
		}
		else if (SPIRType.width == 16)
		{
			switch (SPIRType.basetype)
			{
				case spirv_cross::SPIRType::Int:
					return VK_FORMAT_R16_SINT;
				case spirv_cross::SPIRType::UInt:
					return VK_FORMAT_R16_UINT;
				case spirv_cross::SPIRType::Float:
					return VK_FORMAT_R16_SFLOAT;
				default:
					return VK_FORMAT_UNDEFINED;
			}
		}
		else if (SPIRType.width == 32)
		{
			switch (SPIRType.basetype)
			{
				case spirv_cross::SPIRType::Int:
					return VK_FORMAT_R32_SINT;
				case spirv_cross::SPIRType::UInt:
					return VK_FORMAT_R32_UINT;
				case spirv_cross::SPIRType::Float:
					return VK_FORMAT_R32_SFLOAT;
				default:
					return VK_FORMAT_UNDEFINED;
			}
		}
		else if (SPIRType.width == 64)
		{
			switch (SPIRType.basetype)
			{
				case spirv_cross::SPIRType::Int64:
					return VK_FORMAT_R64_SINT;
				case spirv_cross::SPIRType::UInt64:
					return VK_FORMAT_R64_UINT;
				case spirv_cross::SPIRType::Double:
					return VK_FORMAT_R64_SFLOAT;
				default:
					return VK_FORMAT_UNDEFINED;
			}
		}
		else
		{
			return VK_FORMAT_UNDEFINED;
		}
	}
	else if (SPIRType.vecsize == 2)
	{
		if (SPIRType.width == 8)
		{
			switch (SPIRType.basetype)
			{
				case spirv_cross::SPIRType::Int:
					return VK_FORMAT_R8G8_SINT;
				case spirv_cross::SPIRType::UInt:
					return VK_FORMAT_R8G8_UINT;
				default:
					return VK_FORMAT_UNDEFINED;
			}
		}
		else if (SPIRType.width == 16)
		{
			switch (SPIRType.basetype)
			{
				case spirv_cross::SPIRType::Int:
					return VK_FORMAT_R16G16_SINT;
				case spirv_cross::SPIRType::UInt:
					return VK_FORMAT_R16G16_UINT;
				case spirv_cross::SPIRType::Float:
					return VK_FORMAT_R16G16_SFLOAT;
				default:
					return VK_FORMAT_UNDEFINED;
			}
		}
		else if (SPIRType.width == 32)
		{
			switch (SPIRType.basetype)
			{
				case spirv_cross::SPIRType::Int:
					return VK_FORMAT_R32G32_SINT;
				case spirv_cross::SPIRType::UInt:
					return VK_FORMAT_R32G32_UINT;
				case spirv_cross::SPIRType::Float:
					return VK_FORMAT_R32G32_SFLOAT;
				default:
					return VK_FORMAT_UNDEFINED;
			}
		}
		else if (SPIRType.width == 64)
		{
			switch (SPIRType.basetype)
			{
				case spirv_cross::SPIRType::Int64:
					return VK_FORMAT_R64G64_SINT;
				case spirv_cross::SPIRType::UInt64:
					return VK_FORMAT_R64G64_UINT;
				case spirv_cross::SPIRType::Double:
					return VK_FORMAT_R64G64_SFLOAT;
				default:
					return VK_FORMAT_UNDEFINED;
			}
		}
		else
		{
			return VK_FORMAT_UNDEFINED;
		}
	}
	else if (SPIRType.vecsize == 3)
	{
		if (SPIRType.width == 8)
		{
			switch (SPIRType.basetype)
			{
				case spirv_cross::SPIRType::Int:
					return VK_FORMAT_R8G8B8_SINT;
				case spirv_cross::SPIRType::UInt:
					return VK_FORMAT_R8G8B8_UINT;
				default:
					return VK_FORMAT_UNDEFINED;
			}
		}
		else if (SPIRType.width == 16)
		{
			switch (SPIRType.basetype)
			{
				case spirv_cross::SPIRType::Int:
					return VK_FORMAT_R16G16B16_SINT;
				case spirv_cross::SPIRType::UInt:
					return VK_FORMAT_R16G16B16_UINT;
				case spirv_cross::SPIRType::Float:
					return VK_FORMAT_R16G16B16_SFLOAT;
				default:
					return VK_FORMAT_UNDEFINED;
			}
		}
		else if (SPIRType.width == 32)
		{
			switch (SPIRType.basetype)
			{
				case spirv_cross::SPIRType::Int:
					return VK_FORMAT_R32G32B32_SINT;
				case spirv_cross::SPIRType::UInt:
					return VK_FORMAT_R32G32B32_UINT;
				case spirv_cross::SPIRType::Float:
					return VK_FORMAT_R32G32B32_SFLOAT;
				default:
					return VK_FORMAT_UNDEFINED;
			}
		}
		else if (SPIRType.width == 64)
		{
			switch (SPIRType.basetype)
			{
				case spirv_cross::SPIRType::Int64:
					return VK_FORMAT_R64G64B64_SINT;
				case spirv_cross::SPIRType::UInt64:
					return VK_FORMAT_R64G64B64_UINT;
				case spirv_cross::SPIRType::Double:
					return VK_FORMAT_R64G64B64_SFLOAT;
				default:
					return VK_FORMAT_UNDEFINED;
			}
		}
		else
		{
			return VK_FORMAT_UNDEFINED;
		}
	}
	else if (SPIRType.vecsize == 4)
	{
		if (SPIRType.width == 8)
		{
			switch (SPIRType.basetype)
			{
				case spirv_cross::SPIRType::Int:
					return VK_FORMAT_R8G8B8A8_SINT;
				case spirv_cross::SPIRType::UInt:
					return VK_FORMAT_R8G8B8A8_UINT;
				default:
					return VK_FORMAT_UNDEFINED;
			}
		}
		else if (SPIRType.width == 16)
		{
			switch (SPIRType.basetype)
			{
				case spirv_cross::SPIRType::Int:
					return VK_FORMAT_R16G16B16A16_SINT;
				case spirv_cross::SPIRType::UInt:
					return VK_FORMAT_R16G16B16A16_UINT;
				case spirv_cross::SPIRType::Float:
					return VK_FORMAT_R16G16B16A16_SFLOAT;
				default:
					return VK_FORMAT_UNDEFINED;
			}
		}
		else if (SPIRType.width == 32)
		{
			switch (SPIRType.basetype)
			{
				case spirv_cross::SPIRType::Int:
					return VK_FORMAT_R32G32B32A32_SINT;
				case spirv_cross::SPIRType::UInt:
					return VK_FORMAT_R32G32B32A32_UINT;
				case spirv_cross::SPIRType::Float:
					return VK_FORMAT_R32G32B32A32_SFLOAT;
				default:
					return VK_FORMAT_UNDEFINED;
			}
		}
		else if (SPIRType.width == 64)
		{
			switch (SPIRType.basetype)
			{
				case spirv_cross::SPIRType::Int64:
					return VK_FORMAT_R64G64B64A64_SINT;
				case spirv_cross::SPIRType::UInt64:
					return VK_FORMAT_R64G64B64A64_UINT;
				case spirv_cross::SPIRType::Double:
					return VK_FORMAT_R64G64B64A64_SFLOAT;
				default:
					return VK_FORMAT_UNDEFINED;
			}
		}
		else
		{
			return VK_FORMAT_UNDEFINED;
		}
	}
	else
	{
		return VK_FORMAT_UNDEFINED;
	}
}

static TUInt32 GetOffsetByVkFormat(VkFormat Format)
{
	switch (Format)
	{
		case VK_FORMAT_UNDEFINED:
			return 0;
		case VK_FORMAT_R8_UNORM:
		case VK_FORMAT_R8_SNORM:
		case VK_FORMAT_R8_USCALED:
		case VK_FORMAT_R8_SSCALED:
		case VK_FORMAT_R8_UINT:
		case VK_FORMAT_R8_SINT:
		case VK_FORMAT_R8_SRGB:
		case VK_FORMAT_R4G4_UNORM_PACK8:
			return 1;
		case VK_FORMAT_R4G4B4A4_UNORM_PACK16:
		case VK_FORMAT_B4G4R4A4_UNORM_PACK16:
		case VK_FORMAT_R5G6B5_UNORM_PACK16:
		case VK_FORMAT_B5G6R5_UNORM_PACK16:
		case VK_FORMAT_R5G5B5A1_UNORM_PACK16:
		case VK_FORMAT_B5G5R5A1_UNORM_PACK16:
		case VK_FORMAT_A1R5G5B5_UNORM_PACK16:
		case VK_FORMAT_R8G8_UNORM:
		case VK_FORMAT_R8G8_SNORM:
		case VK_FORMAT_R8G8_USCALED:
		case VK_FORMAT_R8G8_SSCALED:
		case VK_FORMAT_R8G8_UINT:
		case VK_FORMAT_R8G8_SINT:
		case VK_FORMAT_R8G8_SRGB:
		case VK_FORMAT_R16_UNORM:
		case VK_FORMAT_R16_SNORM:
		case VK_FORMAT_R16_USCALED:
		case VK_FORMAT_R16_SSCALED:
		case VK_FORMAT_R16_UINT:
		case VK_FORMAT_R16_SINT:
		case VK_FORMAT_R16_SFLOAT:
			return 2;
		case VK_FORMAT_R8G8B8_UNORM:
		case VK_FORMAT_R8G8B8_SNORM:
		case VK_FORMAT_R8G8B8_USCALED:
		case VK_FORMAT_R8G8B8_SSCALED:
		case VK_FORMAT_R8G8B8_UINT:
		case VK_FORMAT_R8G8B8_SINT:
		case VK_FORMAT_R8G8B8_SRGB:
		case VK_FORMAT_B8G8R8_UNORM:
		case VK_FORMAT_B8G8R8_SNORM:
		case VK_FORMAT_B8G8R8_USCALED:
		case VK_FORMAT_B8G8R8_SSCALED:
		case VK_FORMAT_B8G8R8_UINT:
		case VK_FORMAT_B8G8R8_SINT:
		case VK_FORMAT_B8G8R8_SRGB:
			return 3;
		case VK_FORMAT_R8G8B8A8_UNORM:
		case VK_FORMAT_R8G8B8A8_SNORM:
		case VK_FORMAT_R8G8B8A8_USCALED:
		case VK_FORMAT_R8G8B8A8_SSCALED:
		case VK_FORMAT_R8G8B8A8_UINT:
		case VK_FORMAT_R8G8B8A8_SINT:
		case VK_FORMAT_R8G8B8A8_SRGB:
		case VK_FORMAT_B8G8R8A8_UNORM:
		case VK_FORMAT_B8G8R8A8_SNORM:
		case VK_FORMAT_B8G8R8A8_USCALED:
		case VK_FORMAT_B8G8R8A8_SSCALED:
		case VK_FORMAT_B8G8R8A8_UINT:
		case VK_FORMAT_B8G8R8A8_SINT:
		case VK_FORMAT_B8G8R8A8_SRGB:
		case VK_FORMAT_A8B8G8R8_UNORM_PACK32:
		case VK_FORMAT_A8B8G8R8_SNORM_PACK32:
		case VK_FORMAT_A8B8G8R8_USCALED_PACK32:
		case VK_FORMAT_A8B8G8R8_SSCALED_PACK32:
		case VK_FORMAT_A8B8G8R8_UINT_PACK32:
		case VK_FORMAT_A8B8G8R8_SINT_PACK32:
		case VK_FORMAT_A8B8G8R8_SRGB_PACK32:
		case VK_FORMAT_A2R10G10B10_UNORM_PACK32:
		case VK_FORMAT_A2R10G10B10_SNORM_PACK32:
		case VK_FORMAT_A2R10G10B10_USCALED_PACK32:
		case VK_FORMAT_A2R10G10B10_SSCALED_PACK32:
		case VK_FORMAT_A2R10G10B10_UINT_PACK32:
		case VK_FORMAT_A2R10G10B10_SINT_PACK32:
		case VK_FORMAT_A2B10G10R10_UNORM_PACK32:
		case VK_FORMAT_A2B10G10R10_SNORM_PACK32:
		case VK_FORMAT_A2B10G10R10_USCALED_PACK32:
		case VK_FORMAT_A2B10G10R10_SSCALED_PACK32:
		case VK_FORMAT_A2B10G10R10_UINT_PACK32:
		case VK_FORMAT_A2B10G10R10_SINT_PACK32:
		case VK_FORMAT_R16G16_UNORM:
		case VK_FORMAT_R16G16_SNORM:
		case VK_FORMAT_R16G16_USCALED:
		case VK_FORMAT_R16G16_SSCALED:
		case VK_FORMAT_R16G16_UINT:
		case VK_FORMAT_R16G16_SINT:
		case VK_FORMAT_R16G16_SFLOAT:
		case VK_FORMAT_R32_UINT:
		case VK_FORMAT_R32_SINT:
		case VK_FORMAT_R32_SFLOAT:
		case VK_FORMAT_B10G11R11_UFLOAT_PACK32:
		case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32:
			return 4;
		case VK_FORMAT_R16G16B16_UNORM:
		case VK_FORMAT_R16G16B16_SNORM:
		case VK_FORMAT_R16G16B16_USCALED:
		case VK_FORMAT_R16G16B16_SSCALED:
		case VK_FORMAT_R16G16B16_UINT:
		case VK_FORMAT_R16G16B16_SINT:
		case VK_FORMAT_R16G16B16_SFLOAT:
			return 6;
		case VK_FORMAT_R16G16B16A16_UNORM:
		case VK_FORMAT_R16G16B16A16_SNORM:
		case VK_FORMAT_R16G16B16A16_USCALED:
		case VK_FORMAT_R16G16B16A16_SSCALED:
		case VK_FORMAT_R16G16B16A16_UINT:
		case VK_FORMAT_R16G16B16A16_SINT:
		case VK_FORMAT_R16G16B16A16_SFLOAT:
		case VK_FORMAT_R32G32_UINT:
		case VK_FORMAT_R32G32_SINT:
		case VK_FORMAT_R32G32_SFLOAT:
		case VK_FORMAT_R64_UINT:
		case VK_FORMAT_R64_SINT:
		case VK_FORMAT_R64_SFLOAT:
			return 8;
		case VK_FORMAT_R32G32B32_UINT:
		case VK_FORMAT_R32G32B32_SINT:
		case VK_FORMAT_R32G32B32_SFLOAT:
			return 12;
		case VK_FORMAT_R32G32B32A32_UINT:
		case VK_FORMAT_R32G32B32A32_SINT:
		case VK_FORMAT_R32G32B32A32_SFLOAT:
		case VK_FORMAT_R64G64_UINT:
		case VK_FORMAT_R64G64_SINT:
		case VK_FORMAT_R64G64_SFLOAT:
			return 16;
		case VK_FORMAT_R64G64B64_UINT:
		case VK_FORMAT_R64G64B64_SINT:
		case VK_FORMAT_R64G64B64_SFLOAT:
			return 24;
		case VK_FORMAT_R64G64B64A64_UINT:
		case VK_FORMAT_R64G64B64A64_SINT:
		case VK_FORMAT_R64G64B64A64_SFLOAT:
			return 32;
	}
	return 0;
}

TVulkanShader::TVulkanShader(TVulkanDevice* VulkanDevice, const TChar* Name)
	: VulkanDevice(VulkanDevice), Name(Name)
{
	for (TInt32 StageIndex = 0, StageCount = static_cast<TInt32>(TShaderStage::EMax); StageIndex < StageCount; ++StageIndex)
	{
		ShaderModules[StageIndex] = VK_NULL_HANDLE;
	}
}

TVulkanShader::~TVulkanShader()
{
}

void TVulkanShader::SetName(const TChar* Name)
{
	this->Name = (Name) ? Name : "";
}

TBool TVulkanShader::LoadFromFile(TShaderStage ShaderStage, const TString& FilePath)
{
	TDynamicArray<TUInt32> SPIRVResult{};

	TString ShaderSourceCode{};
	TVulkanShaderCompiler::ReadSourceCodeFromFile(FilePath, &ShaderSourceCode);

	TBool CompiledOk = TVulkanShaderCompiler::CompileIntoSPIRVCode(VulkanDevice->GetVulkanAPIVersion(), ShaderStage, ShaderSourceCode.GetData(), SPIRVResult);

	if (CompiledOk)
	{
		TLog::Success<TRNT_GET_LOG_INFO(VulkanShaderCompiler)>("Shader file \"{}\" was compiled successfully!", FilePath.GetData());
	}
	else
	{
		TLog::Error<TRNT_GET_LOG_INFO(VulkanShaderCompiler)>("Failed to compile shader file \"{}\"!", FilePath.GetData());
		return false;
	}

	TBool Success = BuildShaderInternal(ShaderStage, SPIRVResult);
	return Success;
}

TBool TVulkanShader::LoadFromSourceCode(TShaderStage ShaderStage, const TString& SourceCode)
{
	TDynamicArray<TUInt32> SPIRVResult{};

	TBool CompiledOk = TVulkanShaderCompiler::CompileIntoSPIRVCode(VulkanDevice->GetVulkanAPIVersion(), ShaderStage, SourceCode.GetData(), SPIRVResult);
	if (CompiledOk)
	{
		TLog::Success<TRNT_GET_LOG_INFO(VulkanShaderCompiler)>("Shader source code was compiled successfully!");
	}
	else
	{
		TLog::Error<TRNT_GET_LOG_INFO(VulkanShaderCompiler)>("Failed to compile shader source code!");
		return false;
	}

	TBool Success = BuildShaderInternal(ShaderStage, SPIRVResult);
	return Success;
}

TBool TVulkanShader::LoadFromSPIRVCode(TShaderStage ShaderStage, const TDynamicArray<TUInt32>& SPIRVCode)
{
	TBool Success = BuildShaderInternal(ShaderStage, SPIRVCode);
	return Success;
}

TBool TVulkanShader::BuildShaderInternal(TShaderStage ShaderStage, const TDynamicArray<TUInt32>& SPIRVData)
{
	if (ShaderModules[static_cast<TInt32>(ShaderStage)] != VK_NULL_HANDLE)
	{
		TLog::Warning<TRNT_GET_LOG_INFO(VulkanRHI)>("VkShaderModule with stage={} already exists!", TVulkanUtils::ConvertShaderStageToCString(ShaderStage));
		return false;
	}

	if (CreateShaderModule(ShaderStage, SPIRVData))
	{
		TLog::Success<TRNT_GET_LOG_INFO(VulkanRHI)>("VkShaderModule with stage={} was created successfully!", TVulkanUtils::ConvertShaderStageToCString(ShaderStage));
	}
	else
	{
		TLog::Success<TRNT_GET_LOG_INFO(VulkanRHI)>("Failed to create VkShaderModule with stage={}!", TVulkanUtils::ConvertShaderStageToCString(ShaderStage));
		return false;
	}

	ReflectShader(ShaderStage, SPIRVData);

	return true;
}

TBool TVulkanShader::CreateShaderModule(TShaderStage ShaderStage, const TDynamicArray<TUInt32>& SPIRVData)
{
	VkShaderModule ShaderModule = VK_NULL_HANDLE;
	VkShaderModuleCreateInfo ShaderModuleCreateInfo;
	TMemory::Memset(&ShaderModuleCreateInfo, 0, sizeof(ShaderModuleCreateInfo));

	ShaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	ShaderModuleCreateInfo.pCode = SPIRVData.GetData();
	ShaderModuleCreateInfo.codeSize = SPIRVData.GetElementCount() * sizeof(TUInt32);

	if (TVulkanRHI::VulkanPFNFunctions.CreateShaderModule(VulkanDevice->GetDevice(), &ShaderModuleCreateInfo, nullptr, &ShaderModule) != VK_SUCCESS)
	{
		TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>(
			"Failed to create shader module [stage={}, SPIRV code size={}]",
			TVulkanUtils::ConvertShaderStageToCString(ShaderStage),
			SPIRVData.GetElementCount() * sizeof(TUInt32));

		return false;
	}

	ShaderModules[static_cast<TInt32>(ShaderStage)] = ShaderModule;

	TLog::Success<TRNT_GET_LOG_INFO(VulkanRHI)>(
		"Create shader module successfully [stage={}, SPIRV code size={}]",
		TVulkanUtils::ConvertShaderStageToCString(ShaderStage),
		SPIRVData.GetElementCount() * sizeof(TUInt32));

	return true;
}

void TVulkanShader::ReflectShader(TShaderStage ShaderStage, const TDynamicArray<TUInt32>& SPIRVData)
{
	TInt64 SPIRVDataSize = SPIRVData.GetElementCount();
	VkShaderStageFlags VkShaderStage = TVulkanUtils::ConvertTShaderStageToVkStage(ShaderStage);

	std::vector<TUInt32> SPIRV{};
	SPIRV.resize((TSize_T)SPIRVDataSize);
	TMemory::Memcpy(SPIRV.data(), SPIRVData.GetData(), SPIRVDataSize * sizeof(TUInt32));

	spirv_cross::Compiler Compiler(Move(SPIRV));
	spirv_cross::ShaderResources ShaderResources = Compiler.get_shader_resources();

	if (ShaderStage == TShaderStage::EVertex)
	{
		TUInt32 CurrentOffset = 0;
		for (const spirv_cross::Resource& StageInpResource : ShaderResources.stage_inputs)
		{
			const spirv_cross::SPIRType& SPIRVType = Compiler.get_type(StageInpResource.type_id);

			VkVertexInputAttributeDescription VertexInputAttributeDescription{};
			VertexInputAttributeDescription.binding = Compiler.get_decoration(StageInpResource.id, spv::DecorationBinding);
			VertexInputAttributeDescription.location = Compiler.get_decoration(StageInpResource.id, spv::DecorationLocation);
			VertexInputAttributeDescription.offset = CurrentOffset;
			VertexInputAttributeDescription.format = ConvertSPIRVTypeToVkFormat(SPIRVType);
			VertexInpAttributeDescriptions.Add(VertexInputAttributeDescription);

			CurrentOffset += GetOffsetByVkFormat(VertexInputAttributeDescription.format);
		}

		VertexInputStride = CurrentOffset;
	}

	TUInt32 Offset = 0;
	for (const spirv_cross::Resource& PushConstantBuffer : ShaderResources.push_constant_buffers)
	{
		TUInt32 ID = PushConstantBuffer.id;
		spirv_cross::SmallVector<spirv_cross::BufferRange> BufferRanges = Compiler.get_active_buffer_ranges(ID);

		TUInt32 Size = 0;
		for (const spirv_cross::BufferRange& BufferRange : BufferRanges)
		{
			Size += static_cast<TUInt32>(BufferRange.range);
		}

		VkPushConstantRange PushConstantRange{};
		PushConstantRange.size = Size;
		PushConstantRange.stageFlags = VkShaderStage;
		PushConstantRange.offset = Offset;

		PushConstantRanges.Add(PushConstantRange);
		Offset += Size;
	}

	THashMap<TUInt32, TDynamicArray<VkDescriptorSetLayoutBinding>> SetLayoutBindingMap;

	for (const spirv_cross::Resource& SampledImage : ShaderResources.sampled_images)
	{
		TUInt32 Binding = Compiler.get_decoration(SampledImage.id, spv::DecorationBinding);
		TUInt32 Set = Compiler.get_decoration(SampledImage.id, spv::DecorationDescriptorSet);

		const spirv_cross::SPIRType& Type = Compiler.get_type(SampledImage.type_id);

		TDynamicArray<VkDescriptorSetLayoutBinding>& SetLayoutBindings = SetLayoutBindingMap[Set];

		VkDescriptorSetLayoutBinding SetLayoutBinding{};
		SetLayoutBinding.binding = Binding;
		SetLayoutBinding.descriptorCount = Type.array.size() ? static_cast<TUInt32>(Type.array[0]) : 1;
		SetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		SetLayoutBinding.stageFlags = VkShaderStage;
		SetLayoutBinding.pImmutableSamplers = nullptr;

		SetLayoutBindings.Add(SetLayoutBinding);
	}

	for (const spirv_cross::Resource& StorageImage : ShaderResources.storage_images)
	{
		TUInt32 Binding = Compiler.get_decoration(StorageImage.id, spv::DecorationBinding);
		TUInt32 Set = Compiler.get_decoration(StorageImage.id, spv::DecorationDescriptorSet);

		const spirv_cross::SPIRType& Type = Compiler.get_type(StorageImage.type_id);

		TDynamicArray<VkDescriptorSetLayoutBinding>& SetLayoutBindings = SetLayoutBindingMap[Set];

		VkDescriptorSetLayoutBinding SetLayoutBinding{};
		SetLayoutBinding.binding = Binding;
		SetLayoutBinding.descriptorCount = Type.array.size() ? static_cast<TUInt32>(Type.array[0]) : 1;
		SetLayoutBinding.descriptorType = (Type.image.dim == spv::Dim::DimBuffer) ? VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER : VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
		SetLayoutBinding.stageFlags = VkShaderStage;
		SetLayoutBinding.pImmutableSamplers = nullptr;

		SetLayoutBindings.Add(SetLayoutBinding);
	}

	for (const spirv_cross::Resource& StorageImage : ShaderResources.separate_images)
	{
		TUInt32 Binding = Compiler.get_decoration(StorageImage.id, spv::DecorationBinding);
		TUInt32 Set = Compiler.get_decoration(StorageImage.id, spv::DecorationDescriptorSet);

		const spirv_cross::SPIRType& Type = Compiler.get_type(StorageImage.type_id);

		TDynamicArray<VkDescriptorSetLayoutBinding>& SetLayoutBindings = SetLayoutBindingMap[Set];

		VkDescriptorSetLayoutBinding SetLayoutBinding{};
		SetLayoutBinding.binding = Binding;
		SetLayoutBinding.descriptorCount = Type.array.size() ? static_cast<TUInt32>(Type.array[0]) : 1;
		SetLayoutBinding.descriptorType = (Type.image.dim == spv::Dim::DimBuffer) ? VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER : VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		SetLayoutBinding.stageFlags = VkShaderStage;
		SetLayoutBinding.pImmutableSamplers = nullptr;

		SetLayoutBindings.Add(SetLayoutBinding);
	}

	for (const spirv_cross::Resource& UniformBuffer : ShaderResources.uniform_buffers)
	{
		TUInt32 Binding = Compiler.get_decoration(UniformBuffer.id, spv::DecorationBinding);
		TUInt32 Set = Compiler.get_decoration(UniformBuffer.id, spv::DecorationDescriptorSet);

		const spirv_cross::SPIRType& Type = Compiler.get_type(UniformBuffer.type_id);

		TDynamicArray<VkDescriptorSetLayoutBinding>& SetLayoutBindings = SetLayoutBindingMap[Set];

		VkDescriptorSetLayoutBinding SetLayoutBinding = {};
		SetLayoutBinding.binding = Binding;
		SetLayoutBinding.descriptorCount = Type.array.size() ? static_cast<TUInt32>(Type.array[0]) : 1;
		SetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		SetLayoutBinding.stageFlags = VkShaderStage;
		SetLayoutBinding.pImmutableSamplers = nullptr;

		SetLayoutBindings.Add(SetLayoutBinding);
	}

	for (const spirv_cross::Resource& StorageBuffer : ShaderResources.storage_buffers)
	{
		TUInt32 Binding = Compiler.get_decoration(StorageBuffer.id, spv::DecorationBinding);
		TUInt32 Set = Compiler.get_decoration(StorageBuffer.id, spv::DecorationDescriptorSet);

		const spirv_cross::SPIRType& Type = Compiler.get_type(StorageBuffer.type_id);

		TDynamicArray<VkDescriptorSetLayoutBinding>& SetLayoutBindings = SetLayoutBindingMap[Set];

		VkDescriptorSetLayoutBinding SetLayoutBinding{};
		SetLayoutBinding.binding = Binding;
		SetLayoutBinding.descriptorCount = Type.array.size() ? static_cast<TUInt32>(Type.array[0]) : 1;
		SetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		SetLayoutBinding.stageFlags = VkShaderStage;
		SetLayoutBinding.pImmutableSamplers = nullptr;

		SetLayoutBindings.Add(SetLayoutBinding);
	}

	BuildDescriptorSetLayouts(SetLayoutBindingMap);
}

void TVulkanShader::BuildDescriptorSetLayouts(const THashMap<TUInt32, TDynamicArray<VkDescriptorSetLayoutBinding>>& SetLayoutBindingMap)
{
	DescriptorSetLayouts.Reserve(SetLayoutBindingMap.GetElementCount());

	for (const THashMapElement<TUInt32, TDynamicArray<VkDescriptorSetLayoutBinding>>& Element : SetLayoutBindingMap)
	{
		TVulkanDescriptorSetLayout DescriptorSetLayout{ VulkanDevice };

		for (const VkDescriptorSetLayoutBinding& SetLayoutBinding : Element.Value)
		{
			DescriptorSetLayout.AddLayoutBinding(SetLayoutBinding.binding, SetLayoutBinding.descriptorType, SetLayoutBinding.descriptorCount, SetLayoutBinding.stageFlags);
		}

		if (!DescriptorSetLayout.Built() && DescriptorSetLayout.Build())
		{
			TLog::Success<TRNT_GET_LOG_INFO(VulkanRHI)>("Descriptor set layout was created successfully!");
			DescriptorSetLayouts.Add(DescriptorSetLayout);
		}
		else
		{
			TLog::Error<TRNT_GET_LOG_INFO(VulkanRHI)>("Failed to create Descriptor set layout!");
		}
	}
}

void TVulkanShader::Destroy(TBool ClearSetLayouts)
{
	PushConstantRanges.Clear();
	VertexInpAttributeDescriptions.Clear();

	if (ClearSetLayouts)
	{
		ClearDescriptorSetLayouts();
	}

	VkDevice Device = VulkanDevice->GetDevice();

	for (TInt32 StageIndex = 0, StageCount = static_cast<TInt32>(TShaderStage::EMax); StageIndex < StageCount; ++StageIndex)
	{
		TVulkanRHI::VulkanPFNFunctions.DestroyShaderModule(Device, ShaderModules[StageIndex], nullptr);
		ShaderModules[StageIndex] = VK_NULL_HANDLE;
	}
}

void TVulkanShader::ClearDescriptorSetLayouts()
{
	for (TVulkanDescriptorSetLayout& SetLayout : DescriptorSetLayouts)
	{
		SetLayout.Destroy();
	}
}

#endif
