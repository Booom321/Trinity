#include "TrinityPCH.h"

#include "Trinity/Core/Threading/ConditionVariable.h"

#if defined(TRNT_PLATFORM_WIN64)

TConditionVariable::TConditionVariable()
{
	InitializeConditionVariable(&Handle);
}

TConditionVariable::~TConditionVariable()
{
}

void TConditionVariable::NotifyOne() noexcept
{
	WakeConditionVariable(&Handle);
}

void TConditionVariable::NotifyAll() noexcept
{
	WakeAllConditionVariable(&Handle);
}

void TConditionVariable::Wait(TMutex& Mutex)
{
	SleepConditionVariableCS(&Handle, &Mutex.Mutex, INFINITE);
}

void TConditionVariable::WaitFor(TMutex& Mutex, TInt32 TimeOutMs)
{
	SleepConditionVariableCS(&Handle, &Mutex.Mutex, TimeOutMs);
}

TWindowsConditionVarHandle TConditionVariable::GetConditionVariableHandle() const
{
	return Handle;
}

#endif