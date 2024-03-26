#pragma once

#include "Trinity/Core/Defines.h"
#include "Trinity/Core/PlatformDetection.h"
#include "Trinity/Core/Types/DataTypes.h"

#include "PlatformDeclarations.h"

class TRNT_API TTimer
{
public:
	using TimeStampType = TTimeStamp;

	TTimer() = default;
	~TTimer() = default;

public:
	void Reset();

	template<typename T>
	T GetElapsed(const T TimeResolution) const;

	template<typename T>
	static T GetElapsed(const TimeStampType Start, const TimeStampType End, const T TimeResolution);

	static TimeStampType Now();

public:
	template<typename T>
	TRNT_FORCE_INLINE T GetElapsedSecond() const
	{
		return GetElapsed(static_cast<T>(1));
	}

	template<typename T>
	TRNT_FORCE_INLINE T GetElapsedMillisecond() const
	{
		return GetElapsed(static_cast<T>(1000));
	}

	template<typename T>
	TRNT_FORCE_INLINE T GetElapsedMicroSecond() const
	{
		return GetElapsed(static_cast<T>(1'000'000));
	}

private:
	static TDouble GetQueryPerfomanceFrequency();

	static const TDouble PCFrequency;

	TimeStampType Start{ Now() };
};

#if defined(TRNT_PLATFORM_WIN64)
#include "Windows/WindowsTimerImpl.h"
#else
#	error "TTimer isn't implemented on current platform!"
#endif