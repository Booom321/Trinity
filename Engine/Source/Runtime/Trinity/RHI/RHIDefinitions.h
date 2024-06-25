#pragma once

#define TRNT_NULL_RHI		0
#define TRNT_VULKAN_RHI		1
#define	TRNT_DIRECTX11_RHI  2
#define	TRNT_DIRECTX12_RHI  2

#define TRNT_NULL_RHI_NAME "Null"
#define TRNT_VULKAN_RHI_NAME "Vulkan"
#define TRNT_DIRECTX11_RHI_NAME "DirectX 11"
#define TRNT_DIRECTX12_RHI_NAME "DirectX 12"

enum class TRHIType
{
	EUnknown = -1,
	ENull,
	EVulkan,
	EDirectX11,
	EDirectX12,
};
