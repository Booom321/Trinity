#pragma once

#include "Trinity/Core/Defines.h"
#include "Trinity/Core/PlatformDetection.h"
#include "Trinity/Core/Types/Delegate.h"

#if defined(TRNT_PLATFORM_WIN64)
	#include "Windows/WindowsDeclarations.h"
using TConditionVariableHandle = TWindowsConditionVarHandle;
#else
	#error "TThreadHandle isn't declared on current platform!"
	#error "TThreadID isn't declared on current platform!"
	#error "TThreadData isn't declared on current platform!"
#endif

#include "Mutex.h"

class TRNT_API TConditionVariable
{
public:
	TRNT_DISALLOW_COPY_AND_ASSIGN(TConditionVariable);

	TConditionVariable();
	~TConditionVariable();

public:
	void NotifyOne() noexcept;
	void NotifyAll() noexcept;

	void Wait(TMutex& Mutex);
	void WaitFor(TMutex& Mutex, TInt32 TimeOutMs);

public:
	TConditionVariableHandle GetConditionVariableHandle() const;

private:
	TConditionVariableHandle Handle;
};