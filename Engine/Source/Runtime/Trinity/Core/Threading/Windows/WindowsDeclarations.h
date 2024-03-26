#pragma once

#include "Trinity/Core/PlatformDetection.h"

#if defined(TRNT_PLATFORM_WIN64)

#include <Windows.h>

using TWindowsThreadHandle = HANDLE;
using TWindowsThreadID = unsigned long;

class TWindowsThreadData
{
public:
	TWindowsThreadHandle	ThreadHandle = nullptr;
	TWindowsThreadID		ThreadID = 0;
};

using TWindowsMutexHandle = CRITICAL_SECTION;

#endif