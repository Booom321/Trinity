#pragma once

#pragma once

#include "Trinity/Core/PlatformDetection.h"

#if defined(TRNT_PLATFORM_WIN64)

#include <Windows.h>

using TWindowsWindowHandle = HWND;

#endif