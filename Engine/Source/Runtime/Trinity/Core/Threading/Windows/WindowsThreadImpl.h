#pragma once

#include "Trinity/Core/Defines.h"
#include "Trinity/Core/PlatformDetection.h"

#if defined(TRNT_PLATFORM_WIN64)

#include "Trinity/Core/Threading/Thread.h"
#include "Trinity/Core/Assert/AssertionMacros.h"

#include <functional>
#include <Windows.h>

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

TThread::TThread(TThread&& Thread) noexcept : ThreadData(Move(Thread.ThreadData))
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

void TThread::Join()
{
	WaitForSingleObject(ThreadData.ThreadHandle, INFINITE);
	ThreadData = {};
}

void TThread::Detach()
{
	CloseHandle(ThreadData.ThreadHandle);
	ThreadData = {};
}

void TThread::SetMainThreadID()
{
	MainThreadID = GetCurrentThreadId();
}

TThreadID TThread::GetMainThreadID()
{
	return MainThreadID;
}

TBool TThread::CurrentThreadIsMainThread()
{
	return GetCurrentThreadId() == MainThreadID;
}

#endif