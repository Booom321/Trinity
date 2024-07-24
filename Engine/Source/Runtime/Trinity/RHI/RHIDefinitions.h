#pragma once

#define TRNT_NULL_RHI		0
#define TRNT_VULKAN_RHI		1
#define	TRNT_DIRECTX11_RHI  2
#define	TRNT_DIRECTX12_RHI  2

#define TRNT_NULL_RHI_NAME "Null"
#define TRNT_VULKAN_RHI_NAME "Vulkan"
#define TRNT_DIRECTX11_RHI_NAME "DirectX 11"
#define TRNT_DIRECTX12_RHI_NAME "DirectX 12"

enum class TGraphicsAPI : signed char
{
	EUnknown = -1,
	ENull,
	EVulkan,
	EDirectX11,
	EDirectX12,
};

enum class TShaderStage : signed char
{
	EVertex,
	ETessellationControl,
	ETessellationEvaluation,
	EGeometry,
	EFragment,
	ECompute,
	EHull,
	EDomain, 
	EPixel,

	EMax,
};

enum class TPolygonMode : signed char
{
	EFill,
	ELine,
	EPoint,
	ERectangleNV,
	EMax,
};

enum class TCullMode : signed char
{
	ENONE,
	EFrontBit,
	EBackBit,
	EFrontAndBack,
	EMax,
};

enum class TPrimitiveTopology : signed char
{
	EPointList,
	ELineList,
	ELineStrip,
	ETriangleList,
	ETriangleStrip,
	ETriangleFan,
	ELineListWithAdjacency,
	ELineStripWithAdjacency,
	ETriangleListWithAdjacency,
	ETriangleStripWithAdjacency,
	EPatchList,
	EMax,
};

enum class TFrontFace : signed char
{
	ECounterClockwise = 0,
	EClockwise,
	EMax,
};