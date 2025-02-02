#pragma once

#include "ConditionVariable.h"
#include "DelegateTask.h"
#include "Mutex.h"
#include "ScopeLock.h"
#include "TaskBase.h"
#include "Trinity/Core/Containers/DynamicArray.h"
#include "Trinity/Core/Containers/LinkedList.h"
#include "Trinity/Core/Memory/UniquePtr.h"
#include "Trinity/Core/Utilities/Singleton.h"

#include <condition_variable>

//////////////////////////////////////////////////////////////////////////////////////////////
/// Got idea from:
///	"https://wickedengine.net/2018/11/simple-job-system-using-standard-c/"
///	"https://github.com/turanszkij/WickedEngine/blob/master/WickedEngine/wiJobSystem.cpp"
//////////////////////////////////////////////////////////////////////////////////////////////

enum class TThreadPriority : TInt8
{
	ELow,
	EMedium,
	EHigh,

	EMax,
};

class TThread;

class TRNT_API TTaskSystem : public TSingleton<TTaskSystem>
{
public:
	class TTaskRef;

private:
	////////////// TASK QUEUE //////////////
	class TTaskQueue
	{
	public:
		using QueueType = TDoublyLinkedList<TTaskRef>;

		TTaskQueue() = default;

		~TTaskQueue()
		{
			Clear();
		}

		TRNT_FORCE_INLINE void Enqueue(const TTaskRef& NewTask)
		{
			TScopeLock<TMutex> Lock(Mutex);
			Tasks.InsertAtTail(NewTask);
		}

		TRNT_FORCE_INLINE TBool Dequeue(TTaskRef& Task)
		{
			TScopeLock<TMutex> Lock(Mutex);

			if (Tasks.IsEmpty())
			{
				return false;
			}

			Task = Move(Tasks.PopFront());

			return true;
		}

		TRNT_FORCE_INLINE void Clear()
		{
			TScopeLock<TMutex> Lock(Mutex);

			if (Tasks.IsEmpty())
			{
				return;
			}

			typename QueueType::NodeType* Node = Tasks.GetHead();

			while (Node != nullptr)
			{
				if (Node->Value.Task)
				{
					delete Node->Value.Task;
					Node->Value.Task = nullptr;
				}

				Node = Node->Next;
			}
		}

	public:
		TMutex Mutex;
		QueueType Tasks;
	};

	////////////// PRIORITY THREAD POOL //////////////
	class TRNT_API TPriorityThreadPool
	{
	public:
		void Work(TInt32 ThreadIdx);

	public:
		TMutex WakeMutex;
		TDynamicArray<TThread> Threads;
		TUniquePtr<TTaskQueue[]> TaskQueues;
		TConditionVariable WakeConditionVar;
		TAtomic<TInt32> QueueIndex = 0;
		TInt32 NumThreads;
		TThreadPriority ThreadPriority;
	};

public:
	////////////// CONFIG //////////////
	class TRNT_API TConfig
	{
	public:
		static TConfig GetDefault();

	public:
		TInt32 NumLowPriorityThread;
		TInt32 NumMediumPriorityThread;
		TInt32 NumHighPriorityThread;
	};

	////////////// CONTEXT //////////////
	class TContext
	{
	public:
		TAtomic<TInt32> Counter = 0;
		TThreadPriority Priority = TThreadPriority::EMedium;
	};

	////////////// TASK REF //////////////
	class TTaskRef
	{
	public:
		TContext* Context = nullptr;
		TTaskBase* Task = nullptr;
	};

	TTaskSystem();
	~TTaskSystem();

public:
	TBool Initialize(const TConfig& Config /* = TConfig::GetDefault() */);
	void Shutdown();

public:
	void ExecuteSingleTask(TTaskBase* Task, TContext& Context);
	void ExecuteMultipleTasks(const TDoublyLinkedList<TTaskBase*>& Tasks, TContext& Context);

	template<typename FunctionType>
	void ParallelFor(TUInt32 JobCount, TUInt32 GroupSize, FunctionType&& Func, TContext& Context)
	{
		if (JobCount == 0 || GroupSize == 0)
		{
			return;
		}

		const TUInt32 GroupCount = (JobCount + GroupSize - 1) / GroupSize;

		TDoublyLinkedList<TTaskBase*> Tasks{};
		for (TUInt32 GroupIndex = 0; GroupIndex < GroupCount; ++GroupIndex)
		{
			TTaskBase* Task = new TDelegateTask<void()>([JobCount, GroupSize, Func, GroupIndex]()
			{
				const TUInt32 GroupJobOffset = GroupIndex * GroupSize;
				const TUInt32 GroupJobEnd = TRNT_MIN(GroupJobOffset + GroupSize, JobCount);

				for (TUInt32 Index = GroupJobOffset; Index < GroupJobEnd; ++Index)
				{
					Func(Index, GroupIndex);
				}
			});

			Tasks.InsertAtTail(Task);
		}

		ExecuteMultipleTasks(Tasks, Context);
	}

	// ContiguousContainerType: TDynamicArray, TString, etc
	template<typename ContiguousContainerType, typename FunctionType>
	void ParallelFor(ContiguousContainerType&& Container, TUInt32 GroupSize, FunctionType&& Func, TContext& Context)
	{
		TUInt32 JobCount = static_cast<TUInt32>(Container.GetElementCount());

		if (JobCount == 0 || GroupSize == 0)
		{
			return;
		}

		const TUInt32 GroupCount = (JobCount + GroupSize - 1) / GroupSize;
		typename ContiguousContainerType::PointerType* Data = Container.GetData();
		TDoublyLinkedList<TTaskBase*> Tasks{};

		for (TUInt32 GroupIndex = 0; GroupIndex < GroupCount; ++GroupIndex)
		{
			TTaskBase* Task = new TDelegateTask<void()>([JobCount, GroupSize, Func, GroupIndex, Data]()
			{
				const TUInt32 GroupJobOffset = GroupIndex * GroupSize;
				const TUInt32 GroupJobEnd = TRNT_MIN(GroupJobOffset + GroupSize, JobCount);

				for (TUInt32 Index = GroupJobOffset; Index < GroupJobEnd; ++Index)
				{
					Func(*(Data + Index), Index, GroupIndex);
				}
			});

			Tasks.InsertAtTail(Task);
		}

		ExecuteMultipleTasks(Tasks, Context);
	}

public:
	TRNT_FORCE_INLINE TInt32 GetThreadCountByPriority(TThreadPriority Prior) const
	{
		return Pools[static_cast<TInt32>(Prior)].NumThreads;
	}

	TRNT_FORCE_INLINE TBool IsInitialized() const
	{
		return Initialized.load();
	}

	TRNT_FORCE_INLINE TBool IsBusy(const TContext& Context)
	{
		return Context.Counter.load() > 0;
	}

	void Wait(const TContext& Context);

private:
	TPriorityThreadPool Pools[static_cast<TInt32>(TThreadPriority::EMax)];
	TAtomic<TBool> Initialized = false;
};