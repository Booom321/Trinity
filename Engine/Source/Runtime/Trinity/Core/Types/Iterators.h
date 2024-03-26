#pragma once

#include <type_traits>

#include "Trinity/Core/Defines.h"

template<typename Type, typename SizeType>
class TContiguousIterator
{
public:
	using ElementType = Type;
	using PointerType = Type*;

	TContiguousIterator(PointerType Ptr)
		: Ptr(Ptr)
	{}

	TContiguousIterator(const TContiguousIterator& Iter)
		: Ptr(Iter.Ptr)
	{}

	TRNT_FORCE_INLINE bool operator==(const TContiguousIterator& Rhs) const { return Ptr == Rhs.Ptr; };
	TRNT_FORCE_INLINE bool operator!=(const TContiguousIterator& Rhs) const { return Ptr != Rhs.Ptr; };
	TRNT_FORCE_INLINE bool operator<(const TContiguousIterator& Rhs) const { return Ptr < Rhs.Ptr; };
	TRNT_FORCE_INLINE bool operator<=(const TContiguousIterator& Rhs) const { return Ptr <= Rhs.Ptr; };
	TRNT_FORCE_INLINE bool operator>(const TContiguousIterator& Rhs) const { return Ptr > Rhs.Ptr; };
	TRNT_FORCE_INLINE bool operator>=(const TContiguousIterator& Rhs) const { return Ptr >= Rhs.Ptr; };

	TRNT_FORCE_INLINE ElementType& operator*() const
	{
		return *Ptr;
	}

	TRNT_FORCE_INLINE PointerType operator->() const
	{
		return Ptr;
	}

	TRNT_FORCE_INLINE PointerType GetPointer() const
	{
		return Ptr;
	}

public:
	TContiguousIterator& operator++()
	{
		++Ptr;
		return *this;
	}

	TContiguousIterator operator++(int)
	{
		TContiguousIterator Tmp(*this);
		++Ptr;
		return Tmp;
	}

	TContiguousIterator& operator--()
	{
		--Ptr;
		return *this;
	}

	TContiguousIterator operator--(int)
	{
		TContiguousIterator Tmp(*this);
		--Ptr;
		return Tmp;
	}

	TContiguousIterator& operator+=(SizeType Offset)
	{
		Ptr += Offset;
		return *this;
	}

	TContiguousIterator operator+(SizeType Offset) const
	{
		TContiguousIterator Tmp(*this);
		return Tmp += Offset;
	}

	TContiguousIterator& operator-=(SizeType Offset)
	{
		return *this += -Offset;
	}

	TContiguousIterator operator-(SizeType Offset) const
	{
		TContiguousIterator Tmp(*this);
		return Tmp -= Offset;
	}

	friend TRNT_FORCE_INLINE TContiguousIterator operator+(SizeType Lhs, TContiguousIterator Rhs)
	{
		return Rhs += Lhs;
	}

	friend TRNT_FORCE_INLINE SizeType operator-(TContiguousIterator Lhs, TContiguousIterator Rhs)
	{
		return Lhs.Ptr - Rhs.Ptr;
	}

private:
	PointerType Ptr;
};