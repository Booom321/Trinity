#pragma once

#include "Trinity/Core/PlatformDetection.h"

#if defined(TRNT_PLATFORM_WIN64)
#include <Windows.h>

#ifdef MAX_PATH
#	define TRNT_WINDOWS_MAX_PATH MAX_PATH
#endif 

using TWindowsTimeStamp = LARGE_INTEGER;
using TWindowsHandle = HANDLE;

#endif