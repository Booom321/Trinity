#include "TrinityPCH.h"

#include "RHI.h"

#ifdef TRNT_USE_VULKAN_RHI
#	include "Vulkan/VulkanRHI.h"
#endif

#ifdef TRNT_PLATFORM_WIN64
#	ifdef TRNT_USE_DIRECTX11_RHI
//#		include "DirectX11/DirectX11RHI.h"
#	endif // 

#	ifdef TRNT_USE_DIRECTX12_RHI
//#		include "DirectX12/DirectX12RHI.h"
#	endif // 
#endif

#ifdef TRNT_USE_NULL_RHI
#	include "Null/NullRHI.h"
#endif

TRNT_DECLARE_LOG_INFO(RHI, TLogLevel::EDebug);

TRHI* TRHI::GlobalRHIInstance = nullptr;

TRHI::~TRHI() = default;

TRHI* TRHI::CreateRHI(const TRHIType RHIType)
{
	TRHI* RHI = nullptr;

	switch (RHIType)
	{
	case TRHIType::EUnknown:
		TLog::Error<TRNT_GET_LOG_INFO(RHI)>("Can not create `RHI` with TRHIType::EUnknown!");
		return nullptr;
	case TRHIType::ENull:
		RHI = new TNullRHI();
		break;
	case TRHIType::EVulkan:
		RHI = new TVulkanRHI();
		break;
#if defined(TRNT_PLATFORM_WIN64)
	case TRHIType::EDirectX11:
		//RHI = new TDirectX11RHI();
		break;
	case TRHIType::EDirectX12:
		//RHI = new TDirectX12RHI();
		break;
#else
	case TRHIType::EDirectX11:
		TLog::Error<TRNT_GET_LOG_INFO(RHI)>("DirectX 11 RHI is not supported on current platform!");
		break;
	case TRHIType::EDirectX12:
		TLog::Error<TRNT_GET_LOG_INFO(RHI)>("DirectX 11 RHI is not supported on current platform!");
		break;
#endif
	}

	if (RHI)
	{
		RHI->Initialize();
	}
	return RHI;
}

void TRHI::DestroyRHI(TRHI* RHIPointer)
{
	if (RHIPointer)
	{
		RHIPointer->Shutdown();

		delete RHIPointer;
		RHIPointer = nullptr;
	}
}

void TRHI::SetGlobalInstance(TRHI* RHIPointer)
{
	TRNT_ASSERT_IS_NOT_NULL(RHIPointer);

	GlobalRHIInstance = RHIPointer;
	return;
}

const TChar* TRHI::ConvertRHITypeToCString(const TRHIType RHIType)
{
	switch (RHIType)
	{
	case TRHIType::ENull:
		return TRNT_NULL_RHI_NAME;
	case TRHIType::EVulkan:
		return TRNT_VULKAN_RHI_NAME;
	case TRHIType::EDirectX11:
		return TRNT_DIRECTX11_RHI_NAME;
	case TRHIType::EDirectX12:
		return TRNT_DIRECTX12_RHI_NAME;
	}

	return "Unknown";
}