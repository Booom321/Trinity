#include "TrinityPCH.h"

#if defined(TRNT_USE_VULKAN_RHI)

#ifdef TRNT_USE_WINDOWS_WINDOW

#include "Trinity/Core/Logging/Log.h"
#include "Trinity/RHI/Vulkan/VulkanRHI.h"
#include "Trinity/RHI/Vulkan/VulkanPlatform.h"

static HMODULE VulkanLibrary = nullptr;
PFN_vkGetInstanceProcAddr TVulkanPlatform::VulkanGetInstanceProcAddr = nullptr;

TRNT_DECLARE_LOG_INFO(VulkanPlatformWindows, TLogLevel::EDebug);

#define TRNT_VULKAN_LIB_FILE_NAME "vulkan-1.dll"

static TBool IsLoaded = false;

TBool TVulkanPlatform::LoadVulkanLibrary()
{
	if (IsLoaded)
	{
		return true;
	}

	VulkanLibrary = ::LoadLibraryA(TRNT_VULKAN_LIB_FILE_NAME);

	if (!VulkanLibrary)
	{
		const DWORD ErrorId = ::GetLastError();
		LPSTR MessageBuffer = nullptr;
		TSize_T Size = ::FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, ErrorId, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&MessageBuffer, 0, NULL);

		TLog::Error<TRNT_GET_LOG_INFO(VulkanPlatformWindows)>("Failed to load library: {} | Error id: {} | Error message: \"{}\".", TRNT_VULKAN_LIB_FILE_NAME, ErrorId, MessageBuffer);
		::LocalFree(MessageBuffer);

		return false;
	}

	VulkanGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)(::GetProcAddress(VulkanLibrary, "vkGetInstanceProcAddr"));

	if (!VulkanGetInstanceProcAddr)
	{
		::FreeLibrary(VulkanLibrary);
		VulkanLibrary = nullptr;
		return false;
	}

	IsLoaded = true;

	return true;
}

void TVulkanPlatform::FreeVulkanLibrary()
{
	if (!IsLoaded)
	{
		return;
	}

	if (VulkanLibrary)
	{
		::FreeLibrary(VulkanLibrary);
		VulkanLibrary = nullptr;
	}

	VulkanGetInstanceProcAddr = nullptr;

	IsLoaded = false;
}

void TVulkanPlatform::GetInstanceExtension(TDynamicArray<const TChar*>& Extensions)
{
	Extensions.EmplaceBack(VK_KHR_SURFACE_EXTENSION_NAME);
	Extensions.EmplaceBack(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
}

VkResult TVulkanPlatform::CreateVulkanSurface(TWindow* Window, VkInstance Instance, VkSurfaceKHR* Surface)
{
	VkWin32SurfaceCreateInfoKHR Win32SurfaceCreateInfo;
	memset(&Win32SurfaceCreateInfo, 0, sizeof(VkWin32SurfaceCreateInfoKHR));

	Win32SurfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	Win32SurfaceCreateInfo.hinstance = GetModuleHandle(nullptr);
	Win32SurfaceCreateInfo.hwnd = (HWND)Window->GetNativeHandle();
	return TVulkanRHI::PFNFunctions.CreateWin32SurfaceKHR(Instance, &Win32SurfaceCreateInfo, nullptr, Surface);
}

#endif 

#endif