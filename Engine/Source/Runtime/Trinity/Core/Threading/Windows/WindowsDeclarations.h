#pragma once

#include "Trinity/Core/PlatformDetection.h"

#if defined(TRNT_PLATFORM_WIN64)
	#include <Windows.h>
#endif

#if defined(TRNT_PLATFORM_WIN64)

using TWindowsThreadHandle = HANDLE;
using TWindowsThreadID = DWORD;

class TWindowsThreadData
{
public:
	TWindowsThreadHandle ThreadHandle = nullptr;
	TWindowsThreadID ThreadID = 0;
};

using TWindowsMutexHandle = CRITICAL_SECTION;
using TWindowsConditionVarHandle = CONDITION_VARIABLE;

#endif