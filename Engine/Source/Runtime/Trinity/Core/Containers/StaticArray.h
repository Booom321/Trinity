#pragma once

#include "Trinity/Core/Defines.h"
#include "Trinity/Core/Assert/AssertionMacros.h"
#include "Trinity/Core/Types/DataTypes.h"
#include "Trinity/Core/Types/Iterators.h"
#include "Trinity/Core/TypeTraits/Logical.h"
#include "Trinity/Core/TypeTraits/PrimaryTypes.h"

template<typename Type, TSize_T Count>
class TStaticArray
{
public:
	using ElementType = Type;

	using SizeType = TSize_T;

	using ReferenceType = Type&;
	using ConstReferenceType = const Type&;

	using PointerType = Type*;
	using ConstPointerType = const Type*;

	using IteratorType = TContiguousIterator<ElementType, SizeType>;
	using ConstIteratorType = TContiguousIterator<const ElementType, SizeType>;

	TStaticArray() = default;

	TStaticArray(TStaticArray&&) = default;
	TStaticArray(const TStaticArray&) = default;
	TStaticArray& operator=(TStaticArray&&) = default;
	TStaticArray& operator=(const TStaticArray&) = default;

	~TStaticArray() = default;

	constexpr TRNT_FORCE_INLINE PointerType GetData() noexcept
	{
		return Elements;
	}

	constexpr TRNT_FORCE_INLINE ConstPointerType GetData() const noexcept
	{
		return Elements;
	}

	constexpr TRNT_FORCE_INLINE ReferenceType operator[](SizeType Index)
	{
		TRNT_ASSERT(Index < Count);
		return Elements[Index];
	}

	constexpr TRNT_FORCE_INLINE ConstReferenceType operator[](SizeType Index) const
	{
		TRNT_ASSERT(Index < Count);
		return Elements[Index];
	}

	constexpr TRNT_FORCE_INLINE TBool IsEmpty() const noexcept { return Count == 0; }

	constexpr TRNT_FORCE_INLINE SizeType GetElementCount() const noexcept { return Count; }

	constexpr IteratorType begin() { return IteratorType(Elements); }
	constexpr ConstIteratorType begin() const { return ConstIteratorType(Elements); }
	constexpr ConstIteratorType cbegin() const { return ConstIteratorType(Elements); }

	constexpr IteratorType end() { return IteratorType(Elements + Count); }
	constexpr ConstIteratorType end() const { return ConstIteratorType(Elements + Count); }
	constexpr ConstIteratorType cend() const { return ConstIteratorType(Elements + Count); }

	TBool operator==(const TStaticArray& B) const
	{
		if constexpr (TOr<TIsEnum<Type>, TIsArithmetic<Type>, TIsPointer<Type>>::Value)
		{
			return memcmp(Elements, B.Elements, Count * sizeof(ElementType)) == 0;
		}
		else
		{
			for (SizeType Index = 0; Index < Count; ++Index)
			{
				if (!(Elements[Index] == B.Elements[Index]))
				{
					return false;
				}
			}
			return true;
		}
	}

	TBool operator!=(const TStaticArray& B) const
	{
		if constexpr (TOr<TIsEnum<Type>, TIsArithmetic<Type>, TIsPointer<Type>>::Value)
		{
			return memcmp(Elements, B.Elements, Count * sizeof(ElementType)) != 0;
		}
		else
		{
			for (SizeType Index = 0; Index < Count; ++Index)
			{
				if (!(Elements[Index] == B.Elements[Index]))
				{
					return true;
				}
			}
			return false;
		}
	}

private:
	ElementType Elements[Count];
};