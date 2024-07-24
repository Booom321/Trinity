#include "TrinityPCH.h"

#include "RHI.h"

#ifdef TRNT_SUPPORT_VULKAN_RHI
#	include "Vulkan/VulkanRHI.h"
#endif

#ifdef TRNT_PLATFORM_WIN64
#	ifdef TRNT_SUPPORT_DIRECTX11_RHI
//#		include "DirectX11/DirectX11RHI.h"
#	endif // 

#	ifdef TRNT_SUPPORT_DIRECTX12_RHI
//#		include "DirectX12/DirectX12RHI.h"
#	endif // 
#endif

#ifdef TRNT_SUPPORT_NULL_RHI
#	include "Null/NullRHI.h"
#endif

TRNT_DECLARE_LOG_INFO(RHI, TLogLevel::EDebug);

TGenericRHI* TGenericRHI::GlobalRHIInstance = nullptr;

TGenericRHI::~TGenericRHI() = default;

TGenericRHI* TGenericRHI::CreateRHI(const TGraphicsAPI GraphicsAPI, TBool& Initialized)
{
	TGenericRHI* RHI = nullptr;
	Initialized = false;

	switch (GraphicsAPI)
	{
	case TGraphicsAPI::EUnknown:
		TLog::Error<TRNT_GET_LOG_INFO(RHI)>("Can not create `RHI` with TGraphicsAPI::EUnknown!");
		return nullptr;
	case TGraphicsAPI::ENull:
		RHI = new TNullRHI();
		RHI->RHIName = TRNT_NULL_RHI;
		break;
	case TGraphicsAPI::EVulkan:
		RHI = new TVulkanRHI();
		RHI->RHIName = TRNT_VULKAN_RHI_NAME;
		break;
#if defined(TRNT_SUPPORT_DIRECTX11_RHI)
	case TGraphicsAPI::EDirectX11:
		//RHI = new TDirectX11RHI();
		RHI->RHIName = TRNT_DIRECTX11_RHI_NAME;
		break;
#else
	case TGraphicsAPI::EDirectX11:
		TLog::Error<TRNT_GET_LOG_INFO(RHI)>("DirectX 11 RHI is not supported on current platform!");
		break;
#endif

#if defined(TRNT_SUPPORT_DIRECTX12_RHI)
	case TGraphicsAPI::EDirectX12:
		//RHI = new TDirectX12RHI();
		RHI->RHIName = TRNT_DIRECTX12_RHI_NAME;
		break;
#else
	case TGraphicsAPI::EDirectX12:
		TLog::Error<TRNT_GET_LOG_INFO(RHI)>("DirectX 12 RHI is not supported on current platform!");
		break;
#endif
	}

	if (RHI)
	{
		RHI->GraphicsAPI = GraphicsAPI;
		if (!RHI->Initialize())
		{
			Initialized = false;
		}
		Initialized = true;
	}
	return RHI;
}

void TGenericRHI::DestroyRHI(TGenericRHI* RHIPointer)
{
	if (RHIPointer)
	{
		RHIPointer->Shutdown();

		delete RHIPointer;
		RHIPointer = nullptr;
	}
}

void TGenericRHI::SetGlobalInstance(TGenericRHI* RHIPointer)
{
	TRNT_ASSERT_IS_NOT_NULL(RHIPointer);

	GlobalRHIInstance = RHIPointer;
	return;
}

const TChar* TGenericRHI::ConvertGraphicsAPIToCString(const TGraphicsAPI GraphicsAPI)
{
	switch (GraphicsAPI)
	{
	case TGraphicsAPI::ENull:
		return TRNT_NULL_RHI_NAME;
	case TGraphicsAPI::EVulkan:
		return TRNT_VULKAN_RHI_NAME;
	case TGraphicsAPI::EDirectX11:
		return TRNT_DIRECTX11_RHI_NAME;
	case TGraphicsAPI::EDirectX12:
		return TRNT_DIRECTX12_RHI_NAME;
	}

	return "Unknown";
}