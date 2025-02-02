#include "TrinityPCH.h"

#include "TaskSystem.h"

#include "Thread.h"
#include "Trinity/Core/Math/Math.h"

TTaskSystem::TConfig TTaskSystem::TConfig::GetDefault()
{
	TInt32 NumThreads = TThread::GetNumberOfThreads();

	TTaskSystem::TConfig Config;
	Config.NumLowPriorityThread = NumThreads - 4;
	Config.NumMediumPriorityThread = NumThreads - 3;
	Config.NumHighPriorityThread = NumThreads - 1;

	return Config;
}

void TTaskSystem::TPriorityThreadPool::Work(TInt32 ThreadIdx)
{
	TTaskRef CurrentTask;

	for (TInt32 i = 0; i < this->NumThreads; ++i)
	{
		TTaskQueue& Queue = this->TaskQueues[ThreadIdx % this->NumThreads];

		while (Queue.Dequeue(CurrentTask))
		{
			CurrentTask.Task->Execute();

			delete CurrentTask.Task;
			CurrentTask.Task = nullptr;

			CurrentTask.Context->Counter.fetch_add(-1);
		}
		++ThreadIdx;
	}
}

TTaskSystem::TTaskSystem() = default;

TTaskSystem::~TTaskSystem()
{
	Shutdown();
}

TBool TTaskSystem::Initialize(const TTaskSystem::TConfig& Config)
{
	if (Initialized.load())
	{
		return false;
	}

	Initialized.store(true);

	TInt32 NumLowPriorityThread = TNsMath::Max<TInt32>(1, Config.NumLowPriorityThread);
	TInt32 NumMediumPriorityThread = TNsMath::Max<TInt32>(1, Config.NumMediumPriorityThread);
	TInt32 NumHighPriorityThread = TNsMath::Max<TInt32>(1, Config.NumHighPriorityThread);

	for (TInt32 ThreadPrior = 0; ThreadPrior < static_cast<TInt32>(TThreadPriority::EMax); ++ThreadPrior)
	{
		TTaskSystem::TPriorityThreadPool& CurrentPool = this->Pools[ThreadPrior];
		TThreadPriority ThreadPriorAsEnum = static_cast<TThreadPriority>(ThreadPrior);

		CurrentPool.ThreadPriority = ThreadPriorAsEnum;

		switch (ThreadPriorAsEnum)
		{
			case TThreadPriority::ELow:
				CurrentPool.NumThreads = NumLowPriorityThread;
				break;
			case TThreadPriority::EMedium:
				CurrentPool.NumThreads = NumMediumPriorityThread;
				break;
			case TThreadPriority::EHigh:
				CurrentPool.NumThreads = NumHighPriorityThread;
				break;
		}

		CurrentPool.TaskQueues.Reset(new TTaskQueue[CurrentPool.NumThreads]);
		CurrentPool.Threads.Reserve(CurrentPool.NumThreads);

		for (TInt32 ThreadIdx = 0; ThreadIdx < CurrentPool.NumThreads; ++ThreadIdx)
		{
			TThread& Ref = CurrentPool.Threads.EmplaceBackGetRef([this, ThreadIdx, &CurrentPool]()
			{
				while (this->Initialized.load())
				{
					CurrentPool.Work(ThreadIdx);

					CurrentPool.WakeMutex.Lock();
					CurrentPool.WakeConditionVar.Wait(CurrentPool.WakeMutex);
					CurrentPool.WakeMutex.Unlock();
				}
			});

			TThreadHandle Handle = Ref.GetThreadHandle();
			TInt32 CoreId = ThreadIdx + 1;

#ifdef TRNT_PLATFORM_WIN64
			DWORD_PTR AffinityMask = 1ull << CoreId;
			TRNT_ASSERT(SetThreadAffinityMask(Handle, AffinityMask) > 0);

			switch (ThreadPriorAsEnum)
			{
				case TThreadPriority::ELow:
					TRNT_ASSERT(SetThreadPriority(Handle, THREAD_PRIORITY_LOWEST) != 0);
					break;
				case TThreadPriority::EMedium:
					TRNT_ASSERT(SetThreadPriority(Handle, THREAD_PRIORITY_NORMAL) != 0);
					break;
				case TThreadPriority::EHigh:
					TRNT_ASSERT(SetThreadPriority(Handle, THREAD_PRIORITY_HIGHEST) != 0);
					break;
			}
#elif TRNT_PLATFORM_LINUX
			// Not implemented yet!!! :((
#endif
		}
	}

	return true;
}

void TTaskSystem::Shutdown()
{
	if (Initialized.load() == false)
	{
		return;
	}

	Initialized.store(false);

	TBool WakeLoop = true;
	TThread Waker([&]
	{
		while (WakeLoop)
		{
			for (TInt32 ThreadPrior = 0; ThreadPrior < static_cast<TInt32>(TThreadPriority::EMax); ++ThreadPrior)
			{
				TTaskSystem::TPriorityThreadPool& CurrentPool = this->Pools[ThreadPrior];
				CurrentPool.WakeConditionVar.NotifyAll();
			}
		}
	});

	for (TInt32 ThreadPrior = 0; ThreadPrior < static_cast<TInt32>(TThreadPriority::EMax); ++ThreadPrior)
	{
		// Join all threads
		TTaskSystem::TPriorityThreadPool& CurrentPool = this->Pools[ThreadPrior];

		for (TInt32 ThreadIdx = 0; ThreadIdx < CurrentPool.NumThreads; ++ThreadIdx)
		{
			CurrentPool.Threads[ThreadIdx].Join();
		}
	}

	WakeLoop = false;
	Waker.Join();

	////////////// Release all resources //////////////
	for (TInt32 ThreadPrior = 0; ThreadPrior < static_cast<TInt32>(TThreadPriority::EMax); ++ThreadPrior)
	{
		TTaskSystem::TPriorityThreadPool& CurrentPool = this->Pools[ThreadPrior];
		CurrentPool.NumThreads = 0;
		CurrentPool.TaskQueues.Reset(nullptr);
		CurrentPool.Threads.Clear();
	}
}

void TTaskSystem::ExecuteSingleTask(TTaskBase* Task, TTaskSystem::TContext& Context)
{
	if (Task)
	{
		if (Initialized.load())
		{
			TTaskSystem::TPriorityThreadPool& Pool = Pools[static_cast<TInt32>(Context.Priority)];

			if (Pool.NumThreads < 1)
			{
				Task->Execute();
				return;
			}

			Context.Counter.fetch_add(1);

			TTaskRef TaskRef;
			TaskRef.Context = &Context;
			TaskRef.Task = Task;

			Pool.TaskQueues[Pool.QueueIndex.fetch_add(1) % Pool.NumThreads].Enqueue(TaskRef);

			Pool.WakeConditionVar.NotifyOne();
		}
		else
		{
			Task->Execute();
		}
	}
}

void TTaskSystem::ExecuteMultipleTasks(const TDoublyLinkedList<TTaskBase*>& Tasks, TTaskSystem::TContext& Context)
{
	const TInt32 TaskCount = static_cast<TInt32>(Tasks.GetElementCount());

	if (TaskCount == 0)
	{
		return;
	}

	TTaskSystem::TPriorityThreadPool& Pool = Pools[static_cast<TInt32>(Context.Priority)];
	const typename TDoublyLinkedList<TTaskBase*>::NodeType* CurrentNode = Tasks.GetHead();

	Context.Counter.fetch_add(TaskCount);

	while (CurrentNode)
	{
		TTaskBase* CurrentTask = CurrentNode->Value;

		if (CurrentTask)
		{
			if (Initialized.load())
			{
				if (Pool.NumThreads < 1)
				{
					CurrentTask->Execute();
				}
				else
				{
					TTaskRef TaskRef;
					TaskRef.Context = &Context;
					TaskRef.Task = CurrentTask;

					Pool.TaskQueues[Pool.QueueIndex.fetch_add(1) % Pool.NumThreads].Enqueue(TaskRef);
				}
			}
			else
			{
				CurrentTask->Execute();
			}
		}

		CurrentNode = CurrentNode->Next;
	}

	if (Pool.NumThreads > 1)
	{
		if (TaskCount == 1)
		{
			Pool.WakeConditionVar.NotifyOne();
		}
		else
		{
			Pool.WakeConditionVar.NotifyAll();
		}
	}
}

void TTaskSystem::Wait(const TTaskSystem::TContext& Context)
{
	if (IsBusy(Context))
	{
		TTaskSystem::TPriorityThreadPool& Pool = Pools[static_cast<TInt32>(Context.Priority)];

		Pool.WakeConditionVar.NotifyAll();

		Pool.Work(Pool.QueueIndex.fetch_add(1) % Pool.NumThreads);

		while (IsBusy(Context))
		{
			TThread::YieldThisThread();
		}
	}
}