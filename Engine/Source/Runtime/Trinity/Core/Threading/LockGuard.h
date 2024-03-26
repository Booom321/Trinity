#pragma once

#include "Trinity/Core/Defines.h"
#include <mutex>

template<typename MutexT>
class FLockGuard
{
public:
	using MutexType = MutexT;

	TRNT_FORCE_INLINE explicit FLockGuard(MutexType& Mutex)
		: Mutex(Mutex)
	{
		Mutex.Lock();
	}

	~FLockGuard() noexcept
	{
		Mutex.Unlock();
	}

	TRNT_DISALLOW_COPY_AND_ASSIGN(FLockGuard);

private:
	MutexType& Mutex;
};

template<>
class FLockGuard<std::mutex>
{
public:
	using MutexType = std::mutex;

	TRNT_FORCE_INLINE explicit FLockGuard(std::mutex& Mutex)
		: Mutex(Mutex)
	{
		Mutex.lock();
	}

	~FLockGuard() noexcept
	{
		Mutex.unlock();
	}

	TRNT_DISALLOW_COPY_AND_ASSIGN(FLockGuard);

private:
	std::mutex& Mutex;
};
