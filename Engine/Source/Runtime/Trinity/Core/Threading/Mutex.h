#pragma once

#include "Trinity/Core/Defines.h"
#include "Trinity/Core/PlatformDetection.h"

#if defined(TRNT_PLATFORM_WIN64)
	#include "Windows/WindowsDeclarations.h"
using TMutexHandle = TWindowsMutexHandle;
#else
	#error "TMutexHandle isn't declared on current platform!"
#endif

class TConditionVariable;

class TRNT_API TMutex
{
	TRNT_DISALLOW_COPY_AND_ASSIGN(TMutex);

public:
	friend class TConditionVariable;

	TMutex() noexcept;

	~TMutex();

public:
	void Lock();

	void Unlock();

	bool TryLock();

public:
	TMutexHandle GetMutexHandle() const;

private:
	TMutexHandle Mutex;
};