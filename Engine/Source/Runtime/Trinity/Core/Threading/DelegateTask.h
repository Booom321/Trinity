#pragma once

#include "TaskBase.h"
#include "Trinity/Core/Types/Delegate.h"

#include <functional>

template<typename ReturnT>
class TDelegateTask;

template<typename ReturnT, typename... Args>
class TDelegateTask<ReturnT(Args...)> : public TTaskBase
{
public:
	using ReturnType = ReturnT;
	using FunctionType = TDelegate<ReturnType(Args...)>;
	using BindedFunctionType = TDelegate<ReturnType()>;

public:
	TDelegateTask(FunctionType&& Function, Args&&... Arguments) noexcept
		: Function(std::bind(Forward<FunctionType>(Function), Forward<Args>(Arguments)...))
	{}

	TDelegateTask(TDelegateTask<ReturnType(Args...)>&& Task) noexcept
		: Function(Move(Task.Function)), ObjectPtr(Move(Task.ObjectPtr))
	{
		Task.ObjectPtr = nullptr;
	}

	TRNT_FORCE_INLINE void SetObjectPointer(ReturnType* NewObjectPtr)
	{
		ObjectPtr = NewObjectPtr;
	}

public:
	virtual TExecutionResult Run() override
	{
		if (ObjectPtr)
		{
			*ObjectPtr = Function();
		}
		return TExecutionResult::ECompleted;
	}

private:
	BindedFunctionType Function;
	ReturnType* ObjectPtr;
};

template<typename... Args>
class TDelegateTask<void(Args...)> : public TTaskBase
{
public:
	using ReturnType = void;
	using FunctionType = TDelegate<void(Args...)>;
	using BindedFunctionType = TDelegate<void()>;

public:
	TDelegateTask(FunctionType&& Function, Args&&... Arguments) noexcept
		: Function(std::bind(Forward<FunctionType>(Function), Forward<Args>(Arguments)...))
	{}

	TDelegateTask(TDelegateTask<void(Args...)>&& Task) noexcept
		: Function(Move(Task.Function))
	{}

public:
	virtual TExecutionResult Run() override
	{
		Function();
		return TExecutionResult::ECompleted;
	}

private:
	BindedFunctionType Function;
};