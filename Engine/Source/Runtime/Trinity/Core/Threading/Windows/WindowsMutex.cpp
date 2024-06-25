#include "TrinityPCH.h"
#include "Trinity/Core/Threading/Mutex.h"

#if defined(TRNT_PLATFORM_WIN64)

TMutex::TMutex() noexcept
{
	InitializeCriticalSection(&Mutex);
}

TMutex::~TMutex()
{
	DeleteCriticalSection(&Mutex);
}

void TMutex::Lock()
{
	EnterCriticalSection(&Mutex);
}

void TMutex::Unlock()
{
	LeaveCriticalSection(&Mutex);
}

bool TMutex::TryLock()
{
	return TryEnterCriticalSection(&Mutex) != 0;
}

#endif