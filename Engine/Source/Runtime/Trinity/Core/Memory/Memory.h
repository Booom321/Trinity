#pragma once

#pragma once

#include <stdlib.h>
#include <cstring>

#include "Trinity/Core/Defines.h"
#include "Trinity/Core/Types/DataTypes.h"
#include "Trinity/Core/TypeTraits/Trivial.h"
#include "Trinity/Core/TypeTraits/Logical.h"
#include "Trinity/Core/TypeTraits/PrimaryTypes.h"
#include "Trinity/Core/TypeTraits/IsZeroConstructType.h"
#include "Trinity/Core/TypeTraits/IsBitwiseConstructible.h"
#include "Trinity/Core/TypeTraits/RemoveReference.h"

class TMemory
{
public:
	static TRNT_FORCE_INLINE void* Malloc(TSize_T Size)
	{
		return malloc(Size);
	}

	static TRNT_FORCE_INLINE void Free(void* Ptr)
	{
		free(Ptr);
	}

	static TRNT_FORCE_INLINE void* Memcpy(void* Dest, const void* Source, TSize_T Size)
	{
		return memcpy(Dest, Source, Size);
	}

	static TRNT_FORCE_INLINE void* Memset(void* Dest, int Value, TSize_T Size)
	{
		return memset(Dest, Value, Size);
	}

	static TRNT_FORCE_INLINE void* Memmove(void* Dest, const void* Source, TSize_T Size)
	{
		return memmove(Dest, Source, Size);
	}

	static TRNT_FORCE_INLINE TInt32 Memcmp(const void* Buf1, const void* Buf2, TSize_T Size)
	{
		return memcmp(Buf1, Buf2, Size);
	}

public:
	template<typename Type>
	static TRNT_FORCE_INLINE void DefaultConstructItem(Type* Dest)
	{
		if constexpr (TIsZeroConstructType<Type>::Value)
		{
			memset(Dest, 0, sizeof(Type));
		}
		else
		{
			new (Dest) Type();
		}
	}

	template<typename Type, typename SizeType = TSize_T>
	static TRNT_FORCE_INLINE void DefaultConstructItems(Type* Dest, SizeType Count)
	{
		if constexpr (TIsZeroConstructType<Type>::Value)
		{
			memset(Dest, 0, Count * sizeof(Type));
		}
		else
		{
			while (Count)
			{
				new (Dest) Type();
				++Dest;
				--Count;
			}
		}
	}

	template<typename Type>
	static TRNT_FORCE_INLINE void DestructItem(Type* Ptr)
	{
		if constexpr (!TIsTriviallyDestructible<Type>::Value)
		{
			Ptr->~Type();
		}
	}

	template<typename Type, typename SizeType = TSize_T>
	static TRNT_FORCE_INLINE void DestructItems(Type* Ptr, SizeType Count)
	{
		if constexpr (!TIsTriviallyDestructible<Type>::Value)
		{
			while (Count)
			{
				Ptr->~Type();
				++Ptr;
				--Count;
			}
		}
	}

	template<typename DestinationType, typename SourceType, typename SizeType = TSize_T>
	static TRNT_FORCE_INLINE void ConstructItems(DestinationType* Dest, const SourceType* Source, SizeType Count)
	{
		if constexpr (TIsBitwiseConstructible<DestinationType, SourceType>::Value)
		{
			memcpy(Dest, Source, Count * sizeof(SourceType));
		}
		else
		{
			while (Count)
			{
				new (Dest) DestinationType(*Source);
				++Dest;
				++Source;
				--Count;
			}
		}
	}

	template<typename DestinationType, typename SourceType, typename SizeType = TSize_T>
	static TRNT_FORCE_INLINE void ConstructBackwardItems(DestinationType* Dest, const SourceType* Source, SizeType Count)
	{
		if constexpr (TIsBitwiseConstructible<DestinationType, SourceType>::Value)
		{
			memmove(Dest - Count, Source - Count, Count * sizeof(SourceType));
		}
		else
		{
			while (Count)
			{
				new (--Dest) DestinationType(*--Source);
				--Count;
			}
		}
	}

	template<typename Type, typename SizeType = TSize_T>
	static TRNT_FORCE_INLINE void CopyAssignItems(Type* Dest, const Type* Source, SizeType Count)
	{
		if constexpr (TIsTriviallyCopyAssignable<Type>::Value)
		{
			memcpy(Dest, Source, Count * sizeof(Type));
		}
		else
		{
			while (Count)
			{
				*Dest = *Source;
				++Dest;
				++Source;
				--Count;
			}
		}
	}

	template<typename Type, typename SizeType = TSize_T>
	static TRNT_FORCE_INLINE void CopyAssignBackwardItems(Type* Dest, const Type* Source, SizeType Count)
	{
		if constexpr (TIsTriviallyCopyAssignable<Type>::Value)
		{
			memmove(Dest - Count, Source - Count, Count * sizeof(Type));
		}
		else
		{
			while (Count)
			{
				*--Dest = *--Source;
				--Count;
			}
		}
	}

	template<typename Type, typename SizeType = TSize_T>
	static TRNT_FORCE_INLINE void MoveConstructItems(Type* Dest, Type* Source, SizeType Count)
	{
		if constexpr (TIsTriviallyMoveConstructible<Type>::Value)
		{
			memcpy(Dest, Source, Count * sizeof(Type));
		}
		else
		{
			while (Count)
			{
				new (Dest) Type(Move(*Source));
				++Dest;
				++Source;
				--Count;
			}
		}
	}

	template<typename Type, typename SizeType = TSize_T>
	static TRNT_FORCE_INLINE void MoveConstructBackwardItems(Type* Dest, Type* Source, SizeType Count)
	{
		if constexpr (TIsTriviallyMoveConstructible<Type>::Value)
		{
			memmove(Dest - Count, Source - Count, Count * sizeof(Type));
		}
		else
		{
			while (Count)
			{
				new (--Dest) Type(Move(*--Source));
				--Count;
			}
		}
	}

	template<typename Type, typename SizeType = TSize_T>
	static TRNT_FORCE_INLINE void MoveAssignItems(Type* Dest, Type* Source, SizeType Count)
	{
		if constexpr (TIsTriviallyMoveAssignable<Type>::Value)
		{
			memcpy(Dest, Source, Count * sizeof(Type));
		}
		else
		{
			while (Count)
			{
				*Dest = Move(*Source);
				++Dest;
				++Source;
				--Count;
			}
		}
	}

	template<typename Type, typename SizeType = TSize_T>
	static TRNT_FORCE_INLINE void MoveAssignBackwardItems(Type* Dest, Type* Source, SizeType Count)
	{
		if constexpr (TIsTriviallyMoveAssignable<Type>::Value)
		{
			memmove(Dest - Count, Source - Count, Count * sizeof(Type));
		}
		else
		{
			while (Count)
			{
				*--Dest = Move(*--Source);
				--Count;
			}
		}
	}

	template<typename Type, typename SizeType = TSize_T>
	static TRNT_FORCE_INLINE void FillConstructItems(Type* Dest, const Type& Value, SizeType Count)
	{
		if constexpr (TIsZeroConstructType<Type>::Value)
		{
			memset(Dest, &Value, Count * sizeof(Type));
		}
		else
		{
			while (Count)
			{
				new (Dest) Type(Value);
				++Dest;
				--Count;
			}
		}
	}

	template<typename Type, typename SizeType = TSize_T>
	static TRNT_FORCE_INLINE void FillAssignItems(Type* Dest, const Type& Value, SizeType Count)
	{
		if constexpr (TIsZeroConstructType<Type>::Value)
		{
			memset(Dest, &Value, Count * sizeof(Type));
		}
		else
		{
			while (Count)
			{
				*Dest = Value;
				++Dest;
				--Count;
			}
		}
	}

	template<typename Type, typename SizeType = TSize_T>
	static TRNT_FORCE_INLINE void CompareItems(const Type* A, const Type* B, SizeType Count)
	{
		if constexpr (TOr<TIsEnum<Type>, TIsArithmetic<Type>, TIsPointer<Type>>::Value)
		{
			return !Count || memcmp(A, B, Count * sizeof(Type));
		}
		else
		{
			while (Count)
			{
				if (!(*A == *B))
				{
					return false;
				}

				++A;
				++B;
				--Count;
			}

			return true;
		}
	}
};