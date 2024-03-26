#pragma once

#include "Trinity/Core/PlatformDetection.h"

#if defined(TRNT_PLATFORM_WIN64)

#include <Windows.h>

TDouble TTimer::GetQueryPerfomanceFrequency()
{
	TimeStampType Result;
	QueryPerformanceFrequency(&Result);
	return static_cast<TDouble>(Result.QuadPart);
}

const TDouble TTimer::PCFrequency = TTimer::GetQueryPerfomanceFrequency();

void TTimer::Reset()
{
	QueryPerformanceCounter(&Start);
}

template<typename T>
T TTimer::GetElapsed(const T TimeResolution) const
{
	return static_cast<T>(((Now().QuadPart - Start.QuadPart) * TimeResolution) / PCFrequency);
}

template<typename T>
T TTimer::GetElapsed(const TimeStampType Start, const TimeStampType End, const T TimeResolution)
{
	return static_cast<T>(((Now().QuadPart - Start.QuadPart) * TimeResolution) / PCFrequency);
}

TTimer::TimeStampType TTimer::Now()
{
	TimeStampType Time;
	QueryPerformanceCounter(&Time);
	return Time;
}

#endif