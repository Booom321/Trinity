#pragma once

#include "Trinity/Core/Defines.h"
#include "Trinity/Core/TypeTraits/RemoveReference.h"
#include "Trinity/Core/Assert/AssertionMacros.h"

template<typename Type>
class TSingleton
{
protected:
	TSingleton() = default;
	~TSingleton() = default;

private:
	TSingleton(const TSingleton&) = delete;
	TSingleton& operator=(const TSingleton&) = delete;

	TSingleton(TSingleton&&) = delete;
	TSingleton& operator=(TSingleton&&) = delete;

public:
	template<typename... Arguments>
	static TRNT_FORCE_INLINE void CreateInstance(Arguments&&... Args)
	{
		TRNT_ASSERT(Instance == nullptr);
		Instance = new Type(Forward<Arguments>(Args)...);
	}

	static TRNT_FORCE_INLINE Type* GetInstance()
	{
		return Instance;
	}

	static TRNT_FORCE_INLINE void DeleteInstance()
	{
		if (Instance)
		{
			delete Instance;
			Instance = nullptr;
		}
	}

protected:
	static Type* Instance;
};

template<typename Type>
Type* TSingleton<Type>::Instance = nullptr;