#include "TrinityPCH.h"

#if defined(TRNT_SUPPORT_VULKAN_RHI)
	#include "Trinity/Core/Logging/Log.h"
	#include "Trinity/RHI/Vulkan/VulkanPlatform.h"
	#include "Trinity/RHI/Vulkan/VulkanRHI.h"
#endif

#if defined(TRNT_SUPPORT_VULKAN_RHI) && defined(TRNT_USE_GLFW)

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
