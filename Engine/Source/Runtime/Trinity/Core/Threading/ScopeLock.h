#pragma once

#include "Trinity/Core/Defines.h"

#include <mutex>

template<typename MutexT>
class TScopeLock
{
public:
	using MutexType = MutexT;

	explicit TRNT_FORCE_INLINE TScopeLock(MutexType& Mutex)
		: Mutex(Mutex)
	{
		Mutex.Lock();
	}

	~TScopeLock() noexcept
	{
		Mutex.Unlock();
	}

	TRNT_DISALLOW_COPY_AND_ASSIGN(TScopeLock);

private:
	MutexType& Mutex;
};

template<>
class TScopeLock<std::mutex>
{
public:
	using MutexType = std::mutex;

	explicit TRNT_FORCE_INLINE TScopeLock(std::mutex& Mutex)
		: Mutex(Mutex)
	{
		Mutex.lock();
	}

	~TScopeLock() noexcept
	{
		Mutex.unlock();
	}

	TRNT_DISALLOW_COPY_AND_ASSIGN(TScopeLock);

private:
	std::mutex& Mutex;
};
