#pragma once

#include "Trinity/Core/PlatformDetection.h"

#if defined(TRNT_PLATFORM_WIN64)

#include "Windows/WindowsDeclarations.h"
using TTimeStamp = TWindowsTimeStamp;
using TFileHandle = TWindowsHandle;

#endif