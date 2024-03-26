#pragma once

#include <initializer_list>

#include "Trinity/Core/Defines.h"

#include "Trinity/Core/Types/DataTypes.h"
#include "Trinity/Core/Types/Iterators.h"

#include "Trinity/Core/Memory/Memory.h"

#include "Trinity/Core/TypeTraits/Trivial.h"
#include "Trinity/Core/TypeTraits/IsCopyConstructible.h"
#include "Trinity/Core/TypeTraits/IsMoveConstructible.h"
#include "Trinity/Core/TypeTraits/IsCopyAssignable.h"
#include "Trinity/Core/TypeTraits/IsMoveAssignable.h"

#include "Trinity/Core/Assert/AssertionMacros.h"

template<typename Type>
class TDynamicArray
{
public:
	using SizeType = TInt64;

	using ElementType = Type;
	using ReferenceType = Type&;
	using ConstReferenceType = const Type&;
	using PointerType = Type*;
	using ConstPointerType = const Type*;

	using IteratorType = TContiguousIterator<ElementType, SizeType>;
	using ConstIteratorType = TContiguousIterator<const ElementType, SizeType>;

	static constexpr SizeType Npos = -1;
	static constexpr TFloat GrowSize = 2.0f;

private:
	TRNT_FORCE_INLINE void ConstructData(SizeType PtrLength, ConstPointerType Ptr)
	{
		Data = static_cast<PointerType>(malloc(Cap * sizeof(ElementType)));

		if (PtrLength > 0 && Ptr)
		{
			if constexpr (TIsTriviallyCopyConstructible<ElementType>::Value)
			{
				memcpy(Data, Ptr, PtrLength * sizeof(ElementType));
			}
			else
			{
				for (SizeType Index = 0; Index < PtrLength; ++Index)
				{
					new (Data + Index) ElementType(Ptr[Index]);
				}
			}
		}
	}

	TRNT_FORCE_INLINE void Reallocate(SizeType NewCap, ConstPointerType Source, SizeType SourceLen)
	{
		PointerType NewData = static_cast<PointerType>(malloc(NewCap * sizeof(ElementType)));

		if (SourceLen > 0 && Source)
		{
			if constexpr (TIsTriviallyCopyConstructible<ElementType>::Value)
			{
				memcpy(NewData, Source, SourceLen * sizeof(ElementType));
			}
			else
			{
				for (SizeType Index = 0; Index < SourceLen; ++Index)
				{
					new (NewData + Index) ElementType(Source[Index]);
				}
			}
		}

		if constexpr (!TIsTriviallyDestructible<ElementType>::Value)
		{
			for (SizeType Index = 0; Index < Len; ++Index)
			{
				Data[Index].~ElementType();
			}
		}

		free(Data);
		Data = NewData;
		Cap = NewCap;
	}

public:
	TDynamicArray()
		: Data(nullptr), Len(0), Cap(0)
	{}

	TDynamicArray(SizeType SourceLen, ConstPointerType Source)
	{
		TRNT_ASSERT(SourceLen >= 0);
		Len = SourceLen;
		Cap = SourceLen;
		ConstructData(SourceLen, Source);
	}

	TDynamicArray(SizeType SourceLen, ConstPointerType Source, SizeType ExtraCapacity)
	{
		TRNT_ASSERT(SourceLen >= 0 && ExtraCapacity >= 0);
		Len = SourceLen;
		Cap = SourceLen + ExtraCapacity;
		ConstructData(Len, Source);
	}

	TDynamicArray(std::initializer_list<ElementType> ElementList)
		: Len(ElementList.size()), Cap(Len)
	{
		ConstructData(Len, ElementList.begin());
	}

	TDynamicArray(const TDynamicArray& Other)
		: Len(Other.Len), Cap(Other.Len)
	{
		ConstructData(Other.Len, Other.Data);
	}

	TDynamicArray(TDynamicArray&& Other) noexcept
		: Data(Other.Data), Len(Other.Len), Cap(Other.Cap)
	{
		Other.Data = nullptr;
		Other.Cap = 0;
		Other.Len = 0;
	}

	TDynamicArray(SizeType SourceLen, const TDynamicArray& Source)
	{
		TRNT_ASSERT(SourceLen >= 0);
		Len = SourceLen;
		Cap = SourceLen;
		ConstructData(SourceLen, Source.Data);
	}

	TDynamicArray(const TDynamicArray& Source, SizeType Position, SizeType SourceLen)
	{
		TRNT_ASSERT(Position >= 0 && SourceLen >= 0 && Position + SourceLen <= Source.Len);
		Len = SourceLen;
		Cap = SourceLen;
		ConstructData(Len, Source.Data + Position);
	}

	TDynamicArray(IteratorType First, IteratorType Last)
	{
		TRNT_ASSERT(Last >= First);
		Len = static_cast<SizeType>(Last - First);
		Cap = Len;
		ConstructData(Len, First.GetPointer());
	}

	TDynamicArray(ConstIteratorType First, ConstIteratorType Last)
	{
		TRNT_ASSERT(Last >= First);
		Len = static_cast<SizeType>(Last - First);
		Cap = Len;
		ConstructData(Len, First.GetPointer());
	}

	TDynamicArray(SizeType InitialCapacity)
		: Len(0)
	{
		TRNT_ASSERT(InitialCapacity >= 0);
		Cap = InitialCapacity;
		Data = static_cast<PointerType>(malloc(InitialCapacity * sizeof(ElementType)));
	}

	~TDynamicArray()
	{
		if (Data)
		{
			if constexpr (!TIsTriviallyDestructible<ElementType>::Value)
			{
				for (SizeType Index = 0; Index < Len; ++Index)
				{
					Data[Index].~ElementType();
				}
			}
			free(Data);
			Data = nullptr;
		}
	}

public:
	TRNT_FORCE_INLINE SizeType Length() const noexcept
	{
		return Len;
	}

	TRNT_FORCE_INLINE SizeType Capacity() const noexcept
	{
		return Cap;
	}

	TRNT_FORCE_INLINE SizeType RemainingCapacity() const noexcept
	{
		return Cap - Len;
	}

	TRNT_FORCE_INLINE PointerType GetData() noexcept
	{
		return Data;
	}

	TRNT_FORCE_INLINE ConstPointerType GetData() const noexcept
	{
		return Data;
	}

	TRNT_FORCE_INLINE TBool IsEmpty() const noexcept
	{
		return Len == 0;
	}

public:
	TRNT_FORCE_INLINE ReferenceType ElementAt(SizeType Index)
	{
		TRNT_ASSERT(Index >= 0 && Index < Len);
		return Data[Index];
	}

	TRNT_FORCE_INLINE ConstReferenceType ElementAt(SizeType Index) const
	{
		TRNT_ASSERT(Index >= 0 && Index < Len);
		return Data[Index];
	}

	TRNT_FORCE_INLINE ReferenceType operator[](SizeType Index)
	{
		TRNT_ASSERT(Index >= 0 && Index < Len);
		return Data[Index];
	}

	TRNT_FORCE_INLINE ConstReferenceType operator[](SizeType Index) const
	{
		TRNT_ASSERT(Index >= 0 && Index < Len);
		return Data[Index];
	}

	TRNT_FORCE_INLINE ReferenceType First()
	{
		TRNT_ASSERT(Len > 0);
		return Data[0];
	}

	TRNT_FORCE_INLINE ConstReferenceType First() const
	{
		TRNT_ASSERT(Len > 0);
		return Data[0];
	}

	TRNT_FORCE_INLINE ReferenceType Last()
	{
		TRNT_ASSERT(Len > 0);
		return Data[Len - 1];
	}

	TRNT_FORCE_INLINE ConstReferenceType Last() const
	{
		TRNT_ASSERT(Len > 0);
		return Data[Len - 1];
	}

private:
	TRNT_FORCE_INLINE void Assign(SizeType NewLen, ConstPointerType Ptr)
	{
		if (NewLen > Cap)
		{
			Reallocate(NewLen, Ptr, NewLen);
		}
		else
		{
			TBool B = (Len >= NewLen);

			if (B)
			{
				if constexpr (!TIsTriviallyDestructible<ElementType>::Value)
				{
					SizeType Count = Len - NewLen;
					PointerType Dest = Data + NewLen;

					while (Count)
					{
						Dest->~ElementType();
						++Dest;
						--Count;
					}
				}
			}

			SizeType Count = B ? NewLen : Len;
			if constexpr (TIsTriviallyCopyAssignable<ElementType>::Value)
			{
				memcpy(Data, Ptr, Count * sizeof(ElementType));
			}
			else
			{
				for (SizeType Index = 0; Index < Count; ++Index)
				{
					Data[Index] = Ptr[Index];
				}
			}

			if (!B)
			{
				PointerType Dest = Data + Count;
				ConstPointerType Source = Ptr + Count;
				Count = NewLen - Len;
				if constexpr (TIsTriviallyCopyConstructible<ElementType>::Value)
				{
					memcpy(Dest, Source, Count * sizeof(ElementType));
				}
				else
				{
					while (Count)
					{
						new (Dest) ElementType(*Source);
						++Dest;
						++Source;
						--Count;
					}
				}
			}
		}

		Len = NewLen;
	}

public:
	TDynamicArray& operator=(std::initializer_list<ElementType> ElementList)
	{
		Assign(ElementList.size(), ElementList.begin());
		return *this;
	}

	TDynamicArray& operator=(const TDynamicArray& Other)
	{
		if (this != &Other)
		{
			Assign(Other.Len, Other.Data);
		}

		return *this;
	}

	TDynamicArray& operator=(TDynamicArray&& Other) noexcept
	{
		if (this != &Other)
		{
			if constexpr (!TIsTriviallyDestructible<ElementType>::Value)
			{
				for (SizeType Index = 0; Index < Len; ++Index)
				{
					Data[Index].~ElementType();
				}
			}

			free(Data);
			Data = Other.Data;
			Len = Other.Len;
			Cap = Other.Cap;

			Other.Data = nullptr;
			Other.Len = 0;
			Other.Cap = 0;
		}

		return *this;
	}

public:
	TRNT_FORCE_INLINE IteratorType begin() noexcept
	{
		return IteratorType(Data);
	}

	TRNT_FORCE_INLINE ConstIteratorType begin() const noexcept
	{
		return ConstIteratorType(Data);
	}

	TRNT_FORCE_INLINE ConstIteratorType cbegin() const noexcept
	{
		return ConstIteratorType(Data);
	}

	TRNT_FORCE_INLINE IteratorType end() noexcept
	{
		return IteratorType(Data + Len);
	}

	TRNT_FORCE_INLINE ConstIteratorType end() const noexcept
	{
		return ConstIteratorType(Data + Len);
	}

	TRNT_FORCE_INLINE ConstIteratorType cend() const noexcept
	{
		return ConstIteratorType(Data + Len);
	}

public:
	TRNT_FORCE_INLINE void Clear()
	{
		if constexpr (!TIsTriviallyDestructible<ElementType>::Value)
		{
			for (SizeType Index = 0; Index < Len; ++Index)
			{
				Data[Index].~ElementType();
			}
		}
		Len = 0;
	}

	void Reserve(SizeType NewCap)
	{
		if (NewCap > Cap)
		{
			Reallocate(NewCap, Data, Len);
		}
	}

	void Shrink()
	{
		if (Cap != Len)
		{
			Reallocate(Len, Data, Len);
		}
	}

	void Resize(SizeType NewLen)
	{
		TRNT_ASSERT(NewLen >= 0);

		if (Len == NewLen)
		{
			return;
		}

		if (NewLen > Len)
		{
			if (NewLen > Cap)
			{
				Reallocate(NewLen, Data, Len);
			}
			SizeType Count = NewLen - Len;
			if constexpr (TIsZeroConstructType<ElementType>::Value)
			{
				memset(Data + Len, 0, Count * sizeof(ElementType));
			}
			else
			{
				PointerType Dest = Data + Len;
				while (Count)
				{
					new (Dest) ElementType();
					++Dest;
					--Count;
				}
			}
		}
		else
		{
			if constexpr (!TIsTriviallyDestructible<ElementType>::Value)
			{
				TMemory::DestructItems<ElementType, SizeType>(Data + NewLen, Len - NewLen);
			}
		}

		Len = NewLen;
	}

public:
	TRNT_NODISCARD SizeType FindElement(ConstReferenceType Item) const
	{
		for (const ElementType* TRNT_RESTRICT DataStart = Data, *TRNT_RESTRICT DataEnd = Data + Len; DataStart != DataEnd; ++DataStart)
		{
			if (*DataStart == Item)
			{
				return static_cast<SizeType>(DataStart - Data);
			}
		}

		return Npos;
	}

	template<typename PredicateType>
	TRNT_NODISCARD SizeType FindElementIf(PredicateType Predicate) const
	{
		for (const ElementType* TRNT_RESTRICT DataStart = Data, *TRNT_RESTRICT DataEnd = Data + Len; DataStart != DataEnd; ++DataStart)
		{
			if (Predicate(*DataStart))
			{
				return static_cast<SizeType>(DataStart - Data);
			}
		}

		return Npos;
	}

	TRNT_NODISCARD SizeType FindLastElement(ConstReferenceType Item) const
	{
		for (const ElementType* TRNT_RESTRICT DataStart = Data, *TRNT_RESTRICT DataEnd = Data + Len; DataStart != DataEnd;)
		{
			--DataEnd;
			if (*DataEnd == Item)
			{
				return static_cast<SizeType>(DataEnd - DataStart);
			}
		}
		return Npos;
	}

	template<typename PredicateType>
	TRNT_NODISCARD SizeType FindLastElementIf(PredicateType Predicate) const
	{
		for (const ElementType* TRNT_RESTRICT DataStart = Data, *TRNT_RESTRICT DataEnd = Data + Len; DataStart != DataEnd;)
		{
			--DataEnd;
			if (Predicate(*DataEnd))
			{
				return static_cast<SizeType>(DataEnd - DataStart);
			}
		}
		return Npos;
	}

	TRNT_NODISCARD TBool Contains(ConstReferenceType Item) const
	{
		for (const ElementType* TRNT_RESTRICT DataStart = Data, *TRNT_RESTRICT DataEnd = Data + Len; DataStart != DataEnd; ++DataStart)
		{
			if (*DataStart == Item)
			{
				return true;
			}
		}

		return false;
	}

	template<typename PredicateType>
	TRNT_NODISCARD TBool ContainsIf(PredicateType Predicate) const
	{
		for (const ElementType* TRNT_RESTRICT DataStart = Data, *TRNT_RESTRICT DataEnd = Data + Len; DataStart != DataEnd; ++DataStart)
		{
			if (Predicate(*DataStart))
			{
				return true;
			}
		}

		return false;
	}

	template<typename PredicateType>
	TRNT_NODISCARD TDynamicArray Filter(PredicateType Predicate) const
	{
		TDynamicArray Result;
		for (const ElementType* TRNT_RESTRICT DataStart = Data, *TRNT_RESTRICT DataEnd = Data + Len; DataStart != DataEnd; ++DataStart)
		{
			if (Predicate(*DataStart))
			{
				Result.EmplaceBack(*DataStart);
			}
		}

		return Result;
	}

public:
	TBool operator==(const TDynamicArray& Other) const
	{
		if (Len != Other.Len)
		{
			return false;
		}

		if constexpr (TOr<TIsArithmetic<ElementType>, TIsPointer<ElementType>, TIsEnum<ElementType>>::Value)
		{
			return !Len || !memcmp(Data, Other.Data, Len * sizeof(ElementType));
		}
		else
		{
			for (SizeType Index = 0; Index < Len; ++Index)
			{
				if (!(Data[Index] == Other.Data[Index]))
				{
					return false;
				}
			}
			return true;
		}
	}

	TBool operator!=(const TDynamicArray& Other) const
	{
		return !operator==(Other);
	}

public:
	template<typename ... Arguments>
	void EmplaceBack(Arguments&&... Args)
	{
		if (Len == Cap)
		{
			Cap = static_cast<SizeType>(Cap * GrowSize + 1);
			PointerType NewData = static_cast<PointerType>(malloc(Cap * sizeof(ElementType)));

			new (NewData + Len) ElementType(Forward<Arguments>(Args)...);

			if constexpr (TIsTriviallyMoveConstructible<ElementType>::Value || TIsTriviallyCopyConstructible<ElementType>::Value)
			{
				memcpy(NewData, Data, Len * sizeof(ElementType));
			}
			else
			{
				for (SizeType Index = 0; Index < Len; ++Index)
				{
					if constexpr (TIsMoveConstructible<ElementType>::Value)
					{
						new (NewData + Index) ElementType(Move(Data[Index]));
					}
					else
					{
						new (NewData + Index) ElementType(Data[Index]);
					}
				}
			}

			if constexpr (!TIsTriviallyDestructible<ElementType>::Value)
			{
				for (SizeType Index = 0; Index < Len; ++Index)
				{
					Data[Index].~ElementType();
				}
			}

			free(Data);
			Data = NewData;
		}
		else
		{
			new (Data + Len) ElementType(Forward<Arguments>(Args)...);
		}
		++Len;
	}

	template<typename ... Arguments>
	TRNT_FORCE_INLINE ReferenceType EmplaceBackGetRef(Arguments&&... Args)
	{
		EmplaceBack(Forward<Arguments>(Args)...);
		return Data[Len - 1];
	}

	TRNT_FORCE_INLINE void Add(ConstReferenceType Item)
	{
		EmplaceBack(Item);
	}

	TRNT_FORCE_INLINE void Add(ElementType&& Item)
	{
		EmplaceBack(Move(Item));
	}

	TRNT_FORCE_INLINE ReferenceType AddGetRef(ConstReferenceType Item)
	{
		EmplaceBack(Item);
		return Data[Len - 1];
	}

	TRNT_FORCE_INLINE ReferenceType AddGetRef(ElementType&& Item)
	{
		EmplaceBack(Move(Item));
		return Data[Len - 1];
	}

	TRNT_FORCE_INLINE void AddUnique(ConstReferenceType Item)
	{
		if (!Contains(Item))
		{
			EmplaceBack(Item);
		}
	}

	TRNT_FORCE_INLINE void Push(ConstReferenceType Item)
	{
		EmplaceBack(Item);
	}

	TRNT_FORCE_INLINE void Push(ElementType&& Item)
	{
		EmplaceBack(Move(Item));
	}

private:
	TRNT_FORCE_INLINE SizeType AddUninitialized(SizeType Count = 1)
	{
		if (Len + Count >= Cap)
		{
			Cap = static_cast<SizeType>(Cap * GrowSize + Count);
			PointerType NewData = static_cast<PointerType>(malloc(Cap * sizeof(ElementType)));

			if constexpr (TIsTriviallyMoveConstructible<ElementType>::Value || TIsTriviallyCopyConstructible<ElementType>::Value)
			{
				memcpy(NewData, Data, Len * sizeof(ElementType));
			}
			else
			{
				for (SizeType Index = 0; Index < Len; ++Index)
				{
					if constexpr (TIsMoveConstructible<ElementType>::Value)
					{
						new (NewData + Index) ElementType(Move(Data[Index]));
					}
					else
					{
						new (NewData + Index) ElementType(Data[Index]);
					}
				}
			}

			if constexpr (!TIsTriviallyDestructible<ElementType>::Value)
			{
				for (SizeType Index = 0; Index < Len; ++Index)
				{
					Data[Index].~ElementType();
				}
			}

			free(Data);
			Data = NewData;
		}
		SizeType Index = Len;
		Len += Count;
		return Index;
	}

public:
	void AddDefaulted(SizeType Count = 1)
	{
		TRNT_ASSERT(Count >= 0);
		SizeType Index = AddUninitialized(Count);
		if constexpr (TIsZeroConstructType<ElementType>::Value)
		{
			memset(Data + Index, 0, Count * sizeof(ElementType));
		}
		else
		{
			PointerType Dest = Data + Index;
			while (Count)
			{
				new (Dest) ElementType();
				++Dest;
				--Count;
			}
		}
	}

	TRNT_FORCE_INLINE ReferenceType AddDefaultedGetRef()
	{
		SizeType Index = AddUninitialized();
		PointerType Ptr = Data + Index;
		new (Ptr) ElementType();
		return *Ptr;
	}

public:
	template<typename Function>
	TDynamicArray ForEach(Function Func) const
	{
		TDynamicArray Array(Len, Data);
		for (SizeType Index = 0; Index < Len; ++Index)
		{
			Func(Array.Data[Index]);
		}
		return Array;
	}

	template<typename Function>
	void ForEachTInternal(Function Func)
	{
		for (SizeType Index = 0; Index < Len; ++Index)
		{
			Func(Data[Index]);
		}
	}

public:
	void RemoveAt(SizeType Index, SizeType Count = 1)
	{
		TRNT_ASSERT(Index >= 0 && Count >= 0 && Index + Count <= Len);

		Len -= Count;
		PointerType Dest = Data + Index;

		if constexpr (TIsTriviallyMoveAssignable<ElementType>::Value || TIsTriviallyCopyAssignable<ElementType>::Value)
		{
			memcpy(Dest, Dest + Count, (Len - Index) * sizeof(ElementType));
		}
		else
		{
			PointerType Source = Dest + Count;
			for (SizeType Idx = 0, Cnt = Len - Index; Idx < Cnt; ++Idx)
			{
				if constexpr (TIsMoveAssignable<ElementType>::Value)
				{
					Dest[Idx] = Move(Source[Idx]);
				}
				else
				{
					Dest[Idx] = Source[Idx];
				}
			}
		}

		if constexpr (!TIsTriviallyDestructible<ElementType>::Value)
		{
			Dest = Data + Len;

			for (SizeType Idx = 0; Idx < Count; ++Idx)
			{
				Dest[Idx].~ElementType();
			}
		}
	}

	void Remove(ConstReferenceType Item)
	{
		SizeType Index = this->FindElement(Item);

		if (Index == Npos)
		{
			return;
		}

		RemoveAt(Index);
	}

	void RemoveLast(ConstReferenceType Item)
	{
		SizeType Index = this->FindLastElement(Item);

		if (Index == Npos)
		{
			return;
		}

		RemoveAt(Index);
	}

	void RemoveAll(ConstReferenceType Item)
	{
		for (SizeType Index = Len - 1; Index >= 0; --Index)
		{
			if (Data[Index] == Item)
			{
				RemoveAt(Index);
				if (Len == 0)
				{
					break;
				}
			}
		}
	}

	template<typename PredicateType>
	void RemoveIf(PredicateType Predicate)
	{
		SizeType Index = this->FindElementIf(Predicate);

		if (Index == Npos)
		{
			return;
		}

		RemoveAt(Index);
	}

	template<typename PredicateType>
	void RemoveLastIf(PredicateType Predicate)
	{
		SizeType Index = this->FindLastElementIf(Predicate);

		if (Index == Npos)
		{
			return;
		}

		RemoveAt(Index);
	}

	template<typename PredicateType>
	void RemoveAllIf(PredicateType Predicate)
	{
		for (SizeType Index = Len - 1; Index >= 0; --Index)
		{
			if (Predicate(Data[Index]))
			{
				RemoveAt(Index);
				if (Len == 0)
				{
					break;
				}
			}
		}
	}

	TRNT_FORCE_INLINE ElementType Pop()
	{
		ElementType Item(Data[Len - 1]);
		RemoveAt(Len - 1, 1);
		return Item;
	}

public:
	void Append(SizeType PtrLen, ConstPointerType Ptr)
	{
		TRNT_ASSERT(PtrLen >= 0);

		if (Len + PtrLen >= Cap)
		{
			Cap = static_cast<SizeType>(Cap * GrowSize + PtrLen);
			PointerType NewData = static_cast<PointerType>(malloc(Cap * sizeof(ElementType)));

			if constexpr (TIsTriviallyMoveConstructible<ElementType>::Value || TIsTriviallyCopyConstructible<ElementType>::Value)
			{
				memcpy(NewData, Data, Len * sizeof(ElementType));
			}
			else
			{

				for (SizeType Index = 0; Index < Len; ++Index)
				{
					if constexpr (TIsMoveConstructible<ElementType>::Value)
					{
						new (NewData + Index) ElementType(Move(Data[Index]));
					}
					else
					{
						new (NewData + Index) ElementType(Data[Index]);
					}
				}
			}

			if constexpr (TIsTriviallyCopyConstructible<ElementType>::Value)
			{
				memcpy(NewData + Len, Ptr, PtrLen * sizeof(ElementType));
			}
			else
			{
				PointerType Dest = NewData + Len;
				for (SizeType Index = 0; Index < PtrLen; ++Index)
				{
					new (Dest + Index) ElementType(Ptr[Index]);
				}
			}

			if constexpr (!TIsTriviallyDestructible<ElementType>::Value)
			{
				for (SizeType Index = 0; Index < Len; ++Index)
				{
					Data[Index].~ElementType();
				}
			}

			free(Data);
			Data = NewData;
		}
		else
		{
			if constexpr (TIsTriviallyCopyConstructible<ElementType>::Value)
			{
				memcpy(Data + Len, Ptr, PtrLen * sizeof(ElementType));
			}
			else
			{
				PointerType Dest = Data + Len;
				for (SizeType Index = 0; Index < PtrLen; ++Index)
				{
					new (Dest + Index) ElementType(Ptr[Index]);
				}
			}
		}

		Len += PtrLen;
	}

	void Append(SizeType Count, ConstReferenceType Item)
	{
		TRNT_ASSERT(Count >= 0);

		if (Len + Count >= Cap)
		{
			Cap = static_cast<SizeType>(Cap * GrowSize + Count);
			PointerType NewData = static_cast<PointerType>(malloc(Cap * sizeof(ElementType)));

			if constexpr (TIsTriviallyMoveConstructible<ElementType>::Value || TIsTriviallyCopyConstructible<ElementType>::Value)
			{
				memcpy(NewData, Data, Len * sizeof(ElementType));
			}
			else
			{

				for (SizeType Index = 0; Index < Len; ++Index)
				{
					if constexpr (TIsMoveConstructible<ElementType>::Value)
					{
						new (NewData + Index) ElementType(Move(Data[Index]));
					}
					else
					{
						new (NewData + Index) ElementType(Data[Index]);
					}
				}
			}

			if constexpr (TIsScalar<ElementType>::Value)
			{
				memset(NewData + Len, Item, Count * sizeof(ElementType));
			}
			else
			{
				PointerType Dest = NewData + Len;
				for (SizeType Index = 0; Index < Count; ++Index)
				{
					new (Dest + Index) ElementType(Item);
				}
			}

			if constexpr (!TIsTriviallyDestructible<ElementType>::Value)
			{
				for (SizeType Index = 0; Index < Len; ++Index)
				{
					Data[Index].~ElementType();
				}
			}

			free(Data);
			Data = NewData;
		}
		else
		{
			if constexpr (TIsScalar<ElementType>::Value)
			{
				memset(Data + Len, Item, Count * sizeof(ElementType));
			}
			else
			{
				PointerType Dest = Data + Len;
				for (SizeType Index = 0; Index < Count; ++Index)
				{
					new (Dest + Index) ElementType(Item);
				}
			}
		}

		Len += Count;
	}

	TRNT_FORCE_INLINE void Append(std::initializer_list<ElementType> ElementList)
	{
		Append(ElementList.size(), ElementList.begin());
	}

	TRNT_FORCE_INLINE void Append(const TDynamicArray& Array)
	{
		Append(Array.Len, Array.Data);
	}

	TRNT_FORCE_INLINE void Append(IteratorType First, IteratorType Last)
	{
		const SizeType Offset = static_cast<SizeType>(Last - First);
		return Append(Offset, First.GetPointer());
	}

	TRNT_FORCE_INLINE void Append(ConstIteratorType First, ConstIteratorType Last)
	{
		const SizeType Offset = static_cast<SizeType>(Last - First);
		return Append(Offset, First.GetPointer());
	}

	TRNT_FORCE_INLINE TDynamicArray& operator+=(const TDynamicArray& Array)
	{
		Append(Array.Len, Array.Data);
		return *this;
	}

	TRNT_FORCE_INLINE TDynamicArray& operator+=(std::initializer_list<ElementType> ElementList)
	{
		Append(ElementList.size(), ElementList.begin());
		return *this;
	}

public:
	template<typename... Arguments>
	SizeType EmplaceAt(SizeType Index, Arguments&&... Args)
	{
		TRNT_ASSERT(Index >= 0 && Index <= Len);

		if (Index == Len)
		{
			EmplaceBack(Forward<Arguments>(Args)...);
			return Index;
		}

		if (Len == Cap)
		{
			Cap = static_cast<SizeType>(Cap * GrowSize + 1);
			PointerType NewData = static_cast<PointerType>(malloc(Cap * sizeof(ElementType)));

			new (NewData + Index) ElementType(Forward<Arguments>(Args)...);

			if constexpr (TIsTriviallyMoveConstructible<ElementType>::Value || TIsTriviallyCopyConstructible<ElementType>::Value)
			{
				memcpy(NewData, Data, Index * sizeof(ElementType));
				memcpy(NewData + Index + 1, Data + Index, (Len - Index) * sizeof(ElementType));
			}
			else
			{
				SizeType Count = Index;
				PointerType Dest = NewData;
				PointerType Source = Data;
				while (Count)
				{
					if constexpr (TIsMoveConstructible<ElementType>::Value)
					{
						new (Dest) ElementType(Move(*Source));
					}
					else
					{
						new (Dest) ElementType(*Source);
					}
					++Dest, ++Source;
					--Count;
				}

				++Dest;
				Count = Len - Index;
				while (Count)
				{
					if constexpr (TIsMoveConstructible<ElementType>::Value)
					{
						new (Dest) ElementType(Move(*Source));
					}
					else
					{
						new (Dest) ElementType(*Source);
					}
					++Dest, ++Source;
					--Count;
				}
			}

			if constexpr (!TIsTriviallyDestructible<ElementType>::Value)
			{
				for (SizeType Index = 0; Index < Len; ++Index)
				{
					Data[Index].~ElementType();
				}
			}

			free(Data);
			Data = NewData;
		}
		else
		{
			PointerType Dest = Data + Len;

			if constexpr (TIsMoveConstructible<ElementType>::Value)
			{
				new (Dest) ElementType(Move(*(Dest - 1)));
			}
			else
			{
				new (Dest) ElementType(*(Dest - 1));
			}

			Dest = Data + Index + 1;
			if constexpr (TIsTriviallyMoveAssignable<ElementType>::Value || TIsTriviallyCopyAssignable<ElementType>::Value)
			{
				memmove(Dest, Dest - 1, (Len - Index - 1) * sizeof(ElementType));
			}
			else
			{
				SizeType Count = Len - Index - 1;
				Dest += Count;
				PointerType Source = Dest - 1;

				while (Count)
				{
					if constexpr (TIsMoveAssignable<ElementType>::Value)
					{
						*--Dest = Move(*--Source);
					}
					else
					{
						*--Dest = *--Source;
					}
					--Count;
				}
			}

			Dest = Data + Index;
			Dest->~ElementType();
			new (Dest) ElementType(Forward<Arguments>(Args)...);
		}

		++Len;
		return Index;
	}

	template<typename... Arguments>
	TRNT_NODISCARD TRNT_FORCE_INLINE ReferenceType EmplaceAtGetRef(SizeType Index, Arguments&&... Args)
	{
		EmplaceAt(Index, Forward<Arguments>(Args)...);
		return Data[Index];
	}

	TRNT_FORCE_INLINE SizeType Insert(SizeType Index, ConstReferenceType Item)
	{
		return EmplaceAt(Index, Item);
	}

	TRNT_FORCE_INLINE SizeType Insert(SizeType Index, ElementType&& Item)
	{
		return EmplaceAt(Index, Move(Item));
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE ReferenceType InsertGetRef(SizeType Index, ConstReferenceType Item)
	{
		EmplaceAt(Index, Item);
		return Data[Index];
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE ReferenceType InsertGetRef(SizeType Index, ElementType&& Item)
	{
		EmplaceAt(Index, Move(Item));
		return Data[Index];
	}

	TRNT_FORCE_INLINE SizeType InsertDefaulted(SizeType Index)
	{
		return EmplaceAt(Index);
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE ReferenceType InsertDefaultedGetRef(SizeType Index)
	{
		EmplaceAt(Index);
		return Data[Index];
	}

	SizeType Insert(SizeType Index, ConstPointerType Ptr, SizeType PtrLen)
	{
		TRNT_ASSERT(Index >= 0 && Index <= Len && PtrLen >= 0);

		if (Len + PtrLen >= Cap)
		{
			Cap = static_cast<SizeType>(Cap * GrowSize + PtrLen);
			PointerType NewData = static_cast<PointerType>(malloc(Cap * sizeof(ElementType)));

			if constexpr (TIsTriviallyMoveConstructible<ElementType>::Value || TIsTriviallyCopyConstructible<ElementType>::Value)
			{
				memcpy(NewData, Data, Index * sizeof(ElementType));
				PointerType Dest = NewData + Index;
				memcpy(Dest, Ptr, PtrLen * sizeof(ElementType));
				memcpy(Dest + PtrLen, Data + Index, (Len - Index) * sizeof(ElementType));
			}
			else
			{
				PointerType Dest = NewData;
				PointerType Source = Data;

				SizeType Count = Index;
				while (Count)
				{
					if constexpr (TIsMoveConstructible<ElementType>::Value)
					{
						new (Dest) ElementType(Move(*Source));
					}
					else
					{
						new (Dest) ElementType(*Source);
					}
					++Dest, ++Source, --Count;
				}

				Count = PtrLen;
				while (Count)
				{
					new (Dest) ElementType(*Ptr);

					++Dest, ++Ptr, --Count;
				}

				Count = Len - Index;
				while (Count)
				{
					if constexpr (TIsMoveConstructible<ElementType>::Value)
					{
						new (Dest) ElementType(Move(*Source));
					}
					else
					{
						new (Dest) ElementType(*Source);
					}
					++Dest, ++Source, --Count;
				}
			}

			if constexpr (!TIsTriviallyDestructible<ElementType>::Value)
			{
				for (SizeType Index = 0; Index < Len; ++Index)
				{
					Data[Index].~ElementType();
				}
			}

			free(Data);
			Data = NewData;
		}
		else
		{
			if (Index + PtrLen >= Len)
			{
				if constexpr (TIsTriviallyMoveConstructible<ElementType>::Value || TIsTriviallyCopyConstructible<ElementType>::Value)
				{
					PointerType Source = Data + Index;
					memcpy(Source + PtrLen, Source, (Len - Index) * sizeof(ElementType));
				}
				else
				{
					PointerType Source = Data + Index;
					PointerType Dest = Source + PtrLen;
					SizeType Count = Len - Index;

					while (Count)
					{
						if constexpr (TIsMoveConstructible<ElementType>::Value)
						{
							new (Dest) ElementType(Move(*Source));
						}
						else
						{
							new (Dest) ElementType(*Source);
						}
						++Dest, ++Source, --Count;
					}
				}

				if constexpr (TIsTriviallyCopyAssignable<ElementType>::Value)
				{
					memcpy(Data + Index, Ptr, (Len - Index) * sizeof(ElementType));
				}
				else
				{
					ConstPointerType Source = Ptr;
					PointerType Dest = Data + Index;
					SizeType Count = Len - Index;

					while (Count)
					{
						*Dest = *Source;
						++Dest, ++Source, --Count;
					}
				}

				SizeType RemainingCount = Len - Index;
				if constexpr (TIsTriviallyCopyConstructible<ElementType>::Value)
				{
					memcpy(Data + Len, Ptr + RemainingCount, (PtrLen - RemainingCount) * sizeof(ElementType));
				}
				else
				{
					SizeType Count = PtrLen - RemainingCount;
					ConstPointerType Source = Ptr + RemainingCount;
					PointerType Dest = Data + Len;

					while (Count)
					{
						new (Dest) ElementType(*Source);
						++Dest, ++Source, --Count;
					}
				}
			}
			else
			{
				if constexpr (TIsTriviallyMoveConstructible<ElementType>::Value || TIsTriviallyCopyConstructible<ElementType>::Value)
				{
					PointerType Dest = Data + Len;
					memcpy(Dest, Dest - PtrLen, PtrLen * sizeof(ElementType));
				}
				else
				{
					PointerType Dest = Data + Len;
					PointerType Source = Dest - PtrLen;
					SizeType Count = PtrLen;

					while (Count)
					{
						if constexpr (TIsMoveConstructible<ElementType>::Value)
						{
							new (Dest) ElementType(Move(*Source));
						}
						else
						{
							new (Dest) ElementType(*Source);
						}
						++Dest, ++Source, --Count;
					}
				}

				if constexpr (TIsTriviallyMoveAssignable<ElementType>::Value || TIsTriviallyCopyAssignable<ElementType>::Value)
				{
					PointerType Source = Data + Index;
					memmove(Source + PtrLen, Source, (Len - Index - PtrLen) * sizeof(ElementType));
				}
				else
				{
					SizeType Count = Len - Index - PtrLen;
					PointerType Source = Data + Index + Count;
					PointerType Dest = Source + PtrLen;

					while (Count)
					{
						if constexpr (TIsMoveAssignable<ElementType>::Value)
						{
							*--Dest = Move(*--Source);
						}
						else
						{
							*--Dest = *--Source;
						}
						--Count;
					}
				}

				if constexpr (TIsTriviallyCopyAssignable<ElementType>::Value)
				{
					memcpy(Data + Index, Ptr, PtrLen * sizeof(ElementType));
				}
				else
				{
					PointerType Dest = Data + Index;
					SizeType Count = PtrLen;

					while (Count)
					{
						*Dest = *Ptr;
						++Dest, ++Ptr, --Count;
					}
				}
			}
		}

		Len += PtrLen;
		return Index;
	}

	TRNT_FORCE_INLINE SizeType Insert(SizeType Index, const TDynamicArray& Array)
	{
		return Insert(Index, Array.Data, Array.Len);
	}

	TRNT_FORCE_INLINE SizeType Insert(SizeType Index, std::initializer_list<ElementType> ElementList)
	{
		return Insert(Index, ElementList.begin(), ElementList.size());
	}

	TRNT_FORCE_INLINE SizeType Insert(SizeType Index, IteratorType First, IteratorType Last)
	{
		const SizeType Offset = static_cast<SizeType>(Last - First);
		return Insert(Index, First.GetPointer(), Offset);
	}

	TRNT_FORCE_INLINE SizeType Insert(SizeType Index, ConstIteratorType First, ConstIteratorType Last)
	{
		const SizeType Offset = static_cast<SizeType>(Last - First);
		return Insert(Index, First.GetPointer(), Offset);
	}

private:
	PointerType Data;
	SizeType Len;
	SizeType Cap;
};