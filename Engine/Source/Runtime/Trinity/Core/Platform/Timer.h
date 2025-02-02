#pragma once

#include "PlatformDeclarations.h"
#include "Trinity/Core/Defines.h"
#include "Trinity/Core/PlatformDetection.h"
#include "Trinity/Core/Types/DataTypes.h"

class TRNT_API TTimer
{
public:
	using TimeStampType = TTimeStamp;

	TTimer() = default;
	~TTimer() = default;

public:
	void Reset();

	template<typename T>
	T GetElapsed(const T TimeResolution) const
	{
		return static_cast<T>(((Now().QuadPart - Start.QuadPart) * TimeResolution) / PCFrequency);
	}

	template<typename T>
	static T GetElapsed(const TimeStampType Start, const TimeStampType End, const T TimeResolution)
	{
		return static_cast<T>(((Now().QuadPart - Start.QuadPart) * TimeResolution) / PCFrequency);
	}

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
		return GetElapsed(static_cast<T>(1'000));
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