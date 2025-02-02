#include "TrinityPCH.h"

#include "Trinity/Core/Platform/Timer.h"

#ifdef TRNT_PLATFORM_WIN64

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

TTimer::TimeStampType TTimer::Now()
{
	TimeStampType Time;
	QueryPerformanceCounter(&Time);
	return Time;
}

#endif