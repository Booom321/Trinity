#pragma once

#include <stdio.h>

#include <cstring>
#include <type_traits>

#if defined(_WIN64)
	#include <Windows.h>
#endif

//---------------------------------------------------------------------------------------------------------//
//-------------------------------------------------- RHI --------------------------------------------------//
//---------------------------------------------------------------------------------------------------------//

#include "Trinity/Core/Config.h"
#include "Trinity/Core/Defines.h"

#if defined(TRNT_SUPPORT_NULL_RHI)
// ...
#endif

#if defined(TRNT_SUPPORT_VULKAN_RHI)
	#include "Trinity/RHI/Vulkan/IncludeVulkanHeader.h"
#endif

#if defined(TRNT_SUPPORT_DIRECTX11_RHI)
// ...
#endif

#if defined(TRNT_SUPPORT_DIRECTX12_RHI)
// ...
#endif