#pragma once

#include "Trinity/Core/Defines.h"
#include "Trinity/Core/PlatformDetection.h"
#include "Trinity/Core/Types/Delegate.h"

#if defined(TRNT_PLATFORM_WIN64)
#include "Windows/WindowsDeclarations.h"
using TThreadHandle = TWindowsThreadHandle;
using TThreadID		= TWindowsThreadID;
using TThreadData	= TWindowsThreadData;
#endif

class TRNT_API TThread
{
	TRNT_DISALLOW_COPY_AND_ASSIGN(TThread);

public:
	using ThreadProcType = TDelegate<void()>;

	TThread() noexcept;

	template<typename Function, typename... Arguments>
	explicit TThread(Function&& Func, Arguments&&... Args);

	TThread(TThread&& Thread) noexcept;

	TThread& operator=(TThread&& Thread) noexcept;

	~TThread();

public:
	TBool Joinable() const;

	TThreadID GetThreadID() const;

	TThreadHandle GetThreadHandle() const;

	void Join();

	void Detach();

public:
	void SetMainThreadID();

	TThreadID GetMainThreadID();

	TBool CurrentThreadIsMainThread();

private:
	TThreadData ThreadData;

	static TThreadID MainThreadID;
};

TThreadID TThread::MainThreadID;

#if defined(TRNT_PLATFORM_WIN64)
#include "Windows/WindowsThreadImpl.h"
#else
#	error "TThread isn't implemented on current platform!"
#endif