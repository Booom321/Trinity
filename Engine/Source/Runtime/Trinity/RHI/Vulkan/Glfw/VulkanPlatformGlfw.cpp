#include "TrinityPCH.h"

#if defined(TRNT_SUPPORT_VULKAN_RHI)

#include "Trinity/RHI/Vulkan/VulkanPlatform.h"

#ifdef TRNT_USE_GLFW

#include "Trinity/Core/Logging/Log.h"
#include "Trinity/RHI/Vulkan/VulkanRHI.h"

PFN_vkGetInstanceProcAddr TVulkanPlatform::VulkanGetInstanceProcAddr = nullptr;

TBool TVulkanPlatform::LoadVulkanLibrary()
{
	VulkanGetInstanceProcAddr = reinterpret_cast<PFN_vkGetInstanceProcAddr>(glfwGetInstanceProcAddress(nullptr, "vkGetInstanceProcAddr"));

	if (!VulkanGetInstanceProcAddr)
	{
		return false;
	}

	return true;
}

void TVulkanPlatform::FreeVulkanLibrary()
{
	return;
}

void TVulkanPlatform::GetInstanceExtension(TDynamicArray<const TChar*>& Extensions)
{
	TUInt32 ExtensionCount;
	const TChar** RequiredInstanceExtensions = glfwGetRequiredInstanceExtensions(&ExtensionCount);

	if (RequiredInstanceExtensions)
	{
		Extensions.Reserve(ExtensionCount);
		Extensions.Append(ExtensionCount, RequiredInstanceExtensions);
	}
	return;
}

VkResult TVulkanPlatform::CreateVulkanSurface(TWindow* Window, VkInstance Instance, VkSurfaceKHR* Surface)
{
	return glfwCreateWindowSurface(Instance, static_cast<GLFWwindow*>(Window->GetNativeHandle()), nullptr, Surface);
}

#endif

#endif