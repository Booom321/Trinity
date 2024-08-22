#pragma once

#include "Trinity/Core/Defines.h"
#include "Trinity/Core/Types/DataTypes.h"
#include "Trinity/Core/TypeTraits/RemoveReference.h"

template<typename T, typename U>
class TPair
{
public:
	TRNT_FORCE_INLINE TPair() = default;

	TPair(const T& First, const U& Second)
		: First(First), Second(Second)
	{}

	TPair(T&& First, U&& Second) noexcept
		: First(Move(First)), Second(Move(Second))
	{}

	TPair(T&& First, const U& Second) noexcept
		: First(Move(First)), Second(Second)
	{}

	TPair(const T& First, U&& Second) noexcept
		: First(First), Second(Move(Second))
	{}

	TPair(const TPair& Other)
		: First(Other.First), Second(Other.Second)
	{}

	TPair(TPair&& Other) noexcept
		: First(Move(Other.First)), Second(Move(Other.Second))
	{}

public:
	TRNT_INLINE TBool operator==(const TPair& Other) const
	{
		return First == Other.First && Second == Other.Second;
	}

	TRNT_INLINE TBool operator!=(const TPair& Other) const
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