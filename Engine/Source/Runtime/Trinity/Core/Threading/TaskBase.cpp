#include "TrinityPCH.h"

#include "TaskBase.h"

void TTaskBase::Execute()
{
	if (IsCancelled())
	{
		return;
	}

	SetState(TTaskState::EExecuting);
	TExecutionResult Result = Run();

	switch (Result)
	{
		case TExecutionResult::EFailed:
			SetState(TTaskState::EFailed);
			break;
		case TExecutionResult::ECancelled:
			SetState(TTaskState::ECancelled);
			break;
		case TExecutionResult::ECompleted:
			SetState(TTaskState::ECompleted);
			break;
		default:
			break;
	}
}
