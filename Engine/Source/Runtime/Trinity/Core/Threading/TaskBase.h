#pragma once

#include "Atomic.h"
#include "Trinity/Core/Defines.h"
#include "Trinity/Core/Types/DataTypes.h"
#include "Trinity/Core/Utilities/NonCopyable.h"

enum class TTaskState : TInt8
{
	ECreated,
	EFailed,
	ECancelled,
	EExecuting,
	ECompleted,
};

enum class TExecutionResult : TInt8
{
	EFailed,
	ECancelled,
	ECompleted,
};

////////////////////////////////////////////////////////////////////
////////////////////////// TASK INTERFACE //////////////////////////
////////////////////////////////////////////////////////////////////
class TRNT_API TTaskBase : public TNonCopyable
{
public:
	TTaskBase() = default;
	virtual ~TTaskBase() = default;

public:
	TRNT_NODISCARD TRNT_FORCE_INLINE TTaskState GetState() const
	{
		return State.load();
	}

	TRNT_FORCE_INLINE void SetState(TTaskState NewState)
	{
		State.store(NewState);
	}

public:
	TRNT_NODISCARD TRNT_FORCE_INLINE TBool IsExecuting() const
	{
		return State.load() == TTaskState::EExecuting;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE TBool IsFailed() const
	{
		return State.load() == TTaskState::EFailed;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE TBool IsCancelled() const
	{
		return State.load() == TTaskState::ECancelled;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE TBool IsCompleted() const
	{
		return State.load() == TTaskState::ECompleted;
	}

public:
	void Execute();

	virtual TExecutionResult Run() = 0;

protected:
	TAtomic<TTaskState> State{ TTaskState::ECreated };
};
