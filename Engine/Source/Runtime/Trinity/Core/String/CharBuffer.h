#pragma once

#include "Trinity/Core/Defines.h"
#include "Trinity/Core/String/String.h"
#include "Trinity/Core/TypeTraits/EnableIf.h"
#include "Trinity/Core/TypeTraits/PrimaryTypes.h"
#include "Trinity/Core/TypeTraits/TypeRelationships.h"
#include "Trinity/Core/Types/DataTypes.h"

#include <cstring>

template<typename T, TInt64 Num>
class TCharBuffer
{
public:
	static_assert(TIsCharTypeSupported<T>::Value, "TCharBuffer<T> is not implemented for this char type.");
	static_assert(Num > 0, "`NumChars` must be greater than zero.");

	using ElementType = T;
	using SizeType = TInt64;

	static TRNT_CONSTEXPR SizeType NumChars = Num;

public:
	TRNT_FORCE_INLINE TCharBuffer() = default;

	TCharBuffer(const TCharBuffer& Other) = default;
	TCharBuffer& operator=(const TCharBuffer& Other) = default;
	TCharBuffer(TCharBuffer&& Other) noexcept = default;
	TCharBuffer& operator=(TCharBuffer&& Other) noexcept = default;

public:
	TRNT_FORCE_INLINE SizeType GetElementCount() const
	{
		return NumChars;
	}

	TRNT_FORCE_INLINE ElementType* GetData()
	{
		return Buffer;
	}

	TRNT_FORCE_INLINE const ElementType* GetData() const
	{
		return Buffer;
	}

	TRNT_FORCE_INLINE ElementType& CharAt(SizeType Index) noexcept
	{
		return Buffer[Index];
	}

	TRNT_FORCE_INLINE const ElementType& CharAt(SizeType Index) const noexcept
	{
		return Buffer[Index];
	}

	TRNT_FORCE_INLINE ElementType& operator[](SizeType Idx)
	{
		return Buffer[Idx];
	}

	TRNT_FORCE_INLINE const ElementType& operator[](SizeType Idx) const
	{
		return Buffer[Idx];
	}

	TRNT_FORCE_INLINE TBool IsEmpty() const
	{
		return NumChars == 0;
	}

public:
	TRNT_FORCE_INLINE void Copy(SizeType Count, const ElementType* Str, SizeType StartPos = 0)
	{
		if (Count < 0 || Str == nullptr)
		{
			return;
		}

		if (StartPos < 0)
		{
			StartPos = 0;
		}

		const SizeType Len = NumChars - StartPos;
		memcpy(Buffer + StartPos, Str, TRNT_MIN(Len, Count) * sizeof(ElementType));
	}

public:
	TRNT_FORCE_INLINE void Fill(SizeType Count, ElementType Chr, SizeType StartPos = 0)
	{
		if (Count < 0)
		{
			return;
		}

		if (StartPos < 0)
		{
			StartPos = 0;
		}

		const SizeType Len = NumChars - StartPos;

		if constexpr (TAreTheSameType<ElementType, char>::Value)
		{
			memset(Buffer + StartPos, Chr, TRNT_MIN(Len, Count) * sizeof(ElementType));
		}
		else if constexpr (TAreTheSameType<ElementType, wchar_t>::Value)
		{
			wmemset(Buffer + StartPos, Chr, TRNT_MIN(Len, Count));
		}
	}

public:
	TRNT_CONSTEXPR TRNT_FORCE_INLINE ElementType* begin() noexcept
	{
		return Buffer;
	}

	TRNT_CONSTEXPR TRNT_FORCE_INLINE ElementType* end() noexcept
	{
		return Buffer + NumChars;
	}

	TRNT_CONSTEXPR TRNT_FORCE_INLINE const ElementType* cbegin() const noexcept
	{
		return Buffer;
	}

	TRNT_CONSTEXPR TRNT_FORCE_INLINE const ElementType* cend() const noexcept
	{
		return Buffer + NumChars;
	}

	TRNT_CONSTEXPR TRNT_FORCE_INLINE const ElementType* begin() const noexcept
	{
		return Buffer;
	}

	TRNT_CONSTEXPR TRNT_FORCE_INLINE const ElementType* end() const noexcept
	{
		return Buffer + NumChars;
	}

public:
	ElementType Buffer[NumChars];
};