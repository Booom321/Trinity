#pragma once

#include "Trinity/Core/Defines.h"
#include "Trinity/Core/TypeTraits/RemoveReference.h"

template<typename T, typename U>
class TPair
{
public:
	TRNT_FORCE_INLINE TPair() {};

	TPair(const T& First, const U& Second)
		: First(First), Second(Second)
	{}

	TPair(const TPair& Other)
		: First(Other.First), Second(Other.Second)
	{}

	TPair(TPair&& Other) noexcept
		: First(Move(Other.First)), Second(Move(Other.Second))
	{}

public:
	inline bool operator==(const TPair& Other) const
	{
		return First == Other.First && Second == Other.Second;
	}

	inline bool operator!=(const TPair& Other) const
	{
		return First != Other.First || Second != Other.Second;
	}

public:
	TPair& operator=(const TPair& Other)
	{
		if (this != &Other)
		{
			First = Other.First;
			Second = Other.Second;
		}
		return *this;
	}

	TPair& operator=(TPair&& Other) noexcept
	{
		if (this != &Other)
		{
			First = Move(Other.First);
			Second = Move(Other.Second);
		}
		return *this;
	}

public:
	T First;
	U Second;
};

template<class T, class U>
inline constexpr TPair<T, U> MakePair(const T& First, const U& Second)
{
	return TPair<T, U>(First, Second);
}