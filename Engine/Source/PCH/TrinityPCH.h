#pragma once

#include <stdio.h>
#include <type_traits>
#include <cstring>

#if defined(_WIN64)
#	include <Windows.h>
#endif

//---------------------------------------------------------------------------------------------------------//
//-------------------------------------------------- RHI --------------------------------------------------//
//---------------------------------------------------------------------------------------------------------//

#if defined(TRNT_SUPPORT_NULL_RHI)
// ...
#endif

#if defined(TRNT_SUPPORT_VULKAN_RHI)
#	include "Trinity/RHI/Vulkan/IncludeVulkanHeader.h"
#endif

#if defined(TRNT_SUPPORT_DIRECTX11_RHI)
// ...
#endif

#if defined(TRNT_SUPPORT_DIRECTX12_RHI)
// ...
#endif