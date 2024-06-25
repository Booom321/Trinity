#pragma once

#include <stdio.h>
#include <type_traits>
#include <cstring>

#include "Trinity/Core/Core.h"

#include "Trinity/Platform/Timer.h"
#include "Trinity/Platform/FileIO.h"
#include "Trinity/Platform/FileSystem.h"

#if defined(TRNT_PLATFORM_WIN64)
#	include <Windows.h>
#endif

//---------------------------------------------------------------------------------------------------------//
//-------------------------------------------------- RHI --------------------------------------------------//
//---------------------------------------------------------------------------------------------------------//

#if defined(TRNT_USE_NULL_RHI)
// ...
#endif

#if defined(TRNT_USE_VULKAN_RHI)
#	include "Trinity/RHI/Vulkan/IncludeVulkanHeader.h"
#endif

#if defined(TRNT_USE_DIRECTX11_RHI)
// ...
#endif

#if defined(TRNT_USE_DIRECTX12_RHI)
// ...
#endif