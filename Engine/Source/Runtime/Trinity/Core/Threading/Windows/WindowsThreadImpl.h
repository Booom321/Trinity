#pragma once

#include "Trinity/Core/Defines.h"
#include "Trinity/Core/PlatformDetection.h"
#include "Trinity/Core/Threading/Thread.h"

#if defined(TRNT_PLATFORM_WIN64)
	#include "Trinity/Core/Assert/AssertionMacros.h"

	#include <Windows.h>

	#include <functional>
#endif

#if defined(TRNT_PLATFORM_WIN64)

static DWORD WINAPI RunThread(LPVOID Parameter)
{
	TThread::ThreadProcType* ThreadFuncPointer = static_cast<TThread::ThreadProcType*>(Parameter);
	ThreadFuncPointer->operator()();
	delete ThreadFuncPointer;
	ThreadFuncPointer = nullptr;
	return 0;
}

TThread::TThread() noexcept = default;

template<typename Function, typename... Arguments>
TThread::TThread(Function&& Func, Arguments&&... Args)
{
	ThreadProcType* ThreadFuncPointer = new ThreadProcType(std::bind(Forward<Function>(Func), Forward<Arguments>(Args)...));
	ThreadData.ThreadHandle = CreateThread(nullptr, 0, RunThread, ThreadFuncPointer, 0, &ThreadData.ThreadID);
	TRNT_ASSERT_IS_NOT_NULL(ThreadData.ThreadHandle);
}

TThread::TThread(TThread&& Thread) noexcept
	: ThreadData(Move(Thread.ThreadData))
{
	Thread.ThreadData = {};
}

TThread& TThread::operator=(TThread&& Thread) noexcept
{
	TRNT_ASSERT(ThreadData.ThreadID == 0);

	ThreadData = Move(Thread.ThreadData);
	Thread.ThreadData = {};

	return *this;
}

TThread::~TThread()
{
	TRNT_ASSERT(ThreadData.ThreadID == 0 && ThreadData.ThreadHandle == nullptr);
}

TBool TThread::Joinable() const
{
	return ThreadData.ThreadID != 0;
}

TThreadID TThread::GetThreadID() const
{
	return ThreadData.ThreadID;
}

TThreadHandle TThread::GetThreadHandle() const
{
	return ThreadData.ThreadHandle;
}

TBool TThread::Join()
{
	TRNT_ASSERT_MESSAGE(this->ThreadData.ThreadID != GetCurrentThreadId(), "Resource deadlock would occur!");

	if (WaitForSingleObjectEx(ThreadData.ThreadHandle, INFINITE, FALSE) == WAIT_FAILED)
	{
		return false;
	}

	TBool Success = CloseHandle(ThreadData.ThreadHandle) != 0;
	ThreadData = {};

	return Success;
}

TBool TThread::Detach()
{
	TBool Success = CloseHandle(ThreadData.ThreadHandle) != 0;
	ThreadData = {};

	return Success;
}

TInt32 TThread::GetNumberOfThreads()
{
	SYSTEM_INFO SystemInfo;
	GetSystemInfo(&SystemInfo);
	return SystemInfo.dwNumberOfProcessors;
}

TThreadID TThread::GetCurrentThreadID()
{
	return GetCurrentThreadId();
}

void TThread::YieldThisThread()
{
	SwitchToThread();
}

#endif