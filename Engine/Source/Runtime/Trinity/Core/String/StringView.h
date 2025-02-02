#pragma once

#include "Trinity/Core/Defines.h"
#include "Trinity/Core/String/String.h"
#include "Trinity/Core/Types/DataTypes.h"

template<typename T>
class TStringViewBase
{
public:
	static_assert(TIsCharTypeSupported<T>::Value, "TStringViewBase<T> is not implemented for this char type.");

	using SizeType = TInt64;
	using ElementType = T;
	using ConstReferenceType = const T&;
	using ConstPointerType = const T*;
	using CharUtils = TCharUtils<ElementType>;

	static TRNT_CONSTEXPR SizeType Npos = -1;
	static TRNT_CONSTEXPR TBool IsCharType = TAreTheSameType<T, TChar>::Value;
	static TRNT_CONSTEXPR TBool IsWCharType = TAreTheSameType<T, TWChar>::Value;

public:
	TRNT_CONSTEXPR TRNT_INLINE TStringViewBase() noexcept
		: Data(nullptr), Len(0)
	{}

	TRNT_CONSTEXPR TRNT_INLINE TStringViewBase(ConstPointerType Str, SizeType Len) noexcept
		: Data(Str), Len(Len)
	{}

	TRNT_CONSTEXPR TRNT_INLINE TStringViewBase(ConstPointerType Str) noexcept
		: Data(Str), Len(Private_GetNumChar(Str))
	{}

	template<TSize_T NumChar>
	TRNT_CONSTEXPR TRNT_INLINE TStringViewBase(const ElementType (&Str)[NumChar])
		: Data(Str), Len(NumChar - 1)
	{}

	TRNT_CONSTEXPR TStringViewBase(const TStringViewBase& Other) = default;
	TRNT_CONSTEXPR TStringViewBase& operator=(const TStringViewBase& Other) = default;

public:
	TRNT_CONSTEXPR TRNT_FORCE_INLINE SizeType GetElementCount() const noexcept
	{
		return Len;
	}

	TRNT_CONSTEXPR TRNT_FORCE_INLINE ConstPointerType GetData() const noexcept
	{
		return Data;
	}

	TRNT_CONSTEXPR TRNT_FORCE_INLINE ConstReferenceType operator[](SizeType Index) const noexcept
	{
		return Data[Index];
	}

	TRNT_CONSTEXPR TRNT_FORCE_INLINE ConstReferenceType CharAt(SizeType Index) const noexcept
	{
		return Data[Index];
	}

	TRNT_CONSTEXPR TRNT_FORCE_INLINE ConstReferenceType First() const
	{
		return Data[0];
	}

	TRNT_CONSTEXPR TRNT_FORCE_INLINE ConstReferenceType Last() const
	{
		return Data[Len - 1];
	}

	TRNT_CONSTEXPR TRNT_FORCE_INLINE TBool IsEmpty() const
	{
		return Len == 0;
	}

public:
	TRNT_CONSTEXPR TRNT_FORCE_INLINE TStringViewBase Substring(SizeType Position = 0, SizeType Count = Npos) const
	{
		return TStringViewBase{ Data + Position, (Count == Npos) ? Len - Position : Count };
	}

	TRNT_CONSTEXPR TRNT_FORCE_INLINE TInt32 Compare(TStringViewBase StrView, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive) const
	{
		return Private_Compare(Data, StrView.Data, Len, SearchCase);
	}

	TRNT_CONSTEXPR TRNT_FORCE_INLINE TBool StartsWith(TStringViewBase StrView, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive) const noexcept
	{
		return Len >= StrView.Len && Private_Compare(Data, StrView.Data, StrView.Len, SearchCase) == 0;
	}

	TRNT_CONSTEXPR TRNT_FORCE_INLINE TBool StartsWith(ConstPointerType Str, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive) const
	{
		return StartsWith(TStringViewBase(Str), SearchCase);
	}

	TRNT_CONSTEXPR TRNT_FORCE_INLINE TBool StartsWith(ElementType C, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive) const noexcept
	{
		return Len > 0 && (SearchCase == TStringSearchCase::ECaseSensitive) ? *Data == C : CharUtils::ToUpperCase(*Data) == CharUtils::ToUpperCase(C);
	}

	TRNT_CONSTEXPR TRNT_FORCE_INLINE TBool EndsWith(TStringViewBase StrView, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive) const
	{
		return Len >= StrView.Len && Private_Compare(Data + (Len - StrView.Len), StrView.Data, StrView.Len, SearchCase) == 0;
	}

	TRNT_CONSTEXPR TRNT_FORCE_INLINE TBool EndsWith(ConstPointerType SuffixStr, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive) const
	{
		return EndsWith(TStringViewBase(SuffixStr), SearchCase);
	}

	TRNT_CONSTEXPR TRNT_FORCE_INLINE TBool EndsWith(ElementType C, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive) const
	{
		return Len > 0 && (SearchCase == TStringSearchCase::ECaseSensitive) ? *(Data + Len - 1) == C : CharUtils::ToUpperCase(*(Data + Len - 1)) == CharUtils::ToUpperCase(C);
	}

public:
	TRNT_CONSTEXPR TRNT_FORCE_INLINE TBool operator==(TStringViewBase Rhs) const
	{
		return Len == Rhs.Len && Private_Compare(Data, Rhs.Data, Len) == 0;
	}

	TRNT_CONSTEXPR TRNT_FORCE_INLINE TBool operator!=(TStringViewBase Rhs) const
	{
		return !(Len == Rhs.Len && Private_Compare(Data, Rhs.Data, Len) == 0);
	}

	TRNT_CONSTEXPR TRNT_FORCE_INLINE TBool operator>(TStringViewBase Rhs) const
	{
		return Private_Compare(Data, Rhs.Data, Len) > 0;
	}

	TRNT_CONSTEXPR TRNT_FORCE_INLINE TBool operator>=(TStringViewBase Rhs) const
	{
		return Private_Compare(Data, Rhs.Data, Len) >= 0;
	}

	TRNT_CONSTEXPR TRNT_FORCE_INLINE TBool operator<(TStringViewBase Rhs) const
	{
		return Private_Compare(Data, Rhs.Data, Len) < 0;
	}

	TRNT_CONSTEXPR TRNT_FORCE_INLINE TBool operator<=(TStringViewBase Rhs) const
	{
		return Private_Compare(Data, Rhs.Data, Len) <= 0;
	}

public:
	TRNT_CONSTEXPR TRNT_FORCE_INLINE SizeType Find(TStringViewBase StrView, SizeType StartPos = 0, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive) const
	{
		return Private_Find(StrView.Len, StrView.Data, StartPos, SearchCase);
	}

	TRNT_CONSTEXPR TRNT_FORCE_INLINE SizeType FindLast(TStringViewBase StrView, SizeType StartPos = Npos, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive) const
	{
		return Private_FindLast(StrView.Len, StrView.Data, StartPos, SearchCase);
	}

	TRNT_CONSTEXPR SizeType FindChar(ElementType Chr, SizeType StartPos = 0, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive) const
	{
		if (StartPos < 0)
		{
			StartPos = 0;
		}

		ConstPointerType Start = Data + StartPos;

		if (SearchCase == TStringSearchCase::ECaseSensitive)
		{
			while (*Start)
			{
				if (*Start == Chr)
				{
					return static_cast<SizeType>(Start - Data);
				}
				++Start;
			}
		}
		else
		{
			ElementType UpperChr = CharUtils::ToUpperCase(Chr);
			while (*Start)
			{
				if (CharUtils::ToUpperCase(*Start) == UpperChr)
				{
					return static_cast<SizeType>(Start - Data);
				}
				++Start;
			}
		}

		return Npos;
	}

	TRNT_CONSTEXPR SizeType FindLastChar(ElementType Chr, SizeType StartPos = Npos, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive) const
	{
		ConstPointerType End = Data + ((StartPos < 0) ? Len : StartPos);

		if (SearchCase == TStringSearchCase::ECaseSensitive)
		{
			for (;; --End)
			{
				if (*End == Chr)
				{
					return static_cast<SizeType>(End - Data);
				}

				if (End == Data)
				{
					break;
				}
			}
		}
		else
		{
			ElementType UpperChr = CharUtils::ToUpperCase(Chr);
			for (;; --End)
			{
				if (CharUtils::ToUpperCase(*End) == UpperChr)
				{
					return static_cast<SizeType>(End - Data);
				}

				if (End == Data)
				{
					break;
				}
			}
		}

		return Npos;
	}

public:
	TRNT_CONSTEXPR TRNT_FORCE_INLINE ConstPointerType begin() const noexcept
	{
		return Data;
	}

	TRNT_CONSTEXPR TRNT_FORCE_INLINE ConstPointerType end() const noexcept
	{
		return Data + Len;
	}

private:
	TRNT_CONSTEXPR TRNT_FORCE_INLINE SizeType Private_GetNumChar(ConstPointerType Ptr)
	{
		ConstPointerType BeginPtr = Ptr;
		while (*++Ptr)
		{
		}
		return static_cast<SizeType>(Ptr - BeginPtr);
	}

	TRNT_CONSTEXPR TInt32 Private_Compare(ConstPointerType Lhs, ConstPointerType Rhs, SizeType Count, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive) const
	{
		if (SearchCase == TStringSearchCase::ECaseSensitive)
		{
			for (; Count != 0; ++Lhs, ++Rhs, --Count)
			{
				if (*Lhs < *Rhs)
				{
					return -1;
				}
				if (*Lhs > *Rhs)
				{
					return 1;
				}
			}
			return 0;
		}

		for (; Count != 0; ++Lhs, ++Rhs, --Count)
		{
			ElementType Lhsc = CharUtils::ToUpperCase(*Lhs);
			ElementType Rhsc = CharUtils::ToUpperCase(*Rhs);
			if (Lhsc < Rhsc)
			{
				return -1;
			}
			if (Lhsc > Rhsc)
			{
				return 1;
			}
		}
		return 0;
	}

	TRNT_CONSTEXPR SizeType Private_Find(SizeType SubstrLen, ConstPointerType Substr, SizeType StartPos = 0, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive) const
	{
		if (StartPos < 0)
		{
			StartPos = 0;
		}

		if (SubstrLen > Len || StartPos > Len - SubstrLen)
		{
			return Npos;
		}

		if (*Substr == (ElementType)0 || SubstrLen == 0)
		{
			return StartPos;
		}

		ConstPointerType EndStringPos = Data + (Len - SubstrLen) + 1;

		if (SearchCase == TStringSearchCase::ECaseSensitive)
		{
			for (ConstPointerType Tmp = Data + StartPos; Tmp < EndStringPos; ++Tmp)
			{
				if (*Tmp == *Substr && Private_Compare(Tmp, Substr, SubstrLen, SearchCase) == 0)
				{
					return static_cast<SizeType>(Tmp - Data);
				}
			}
		}
		else
		{
			const ElementType FirstUpperSubstrChar = CharUtils::ToUpperCase(*Substr);
			for (ConstPointerType Tmp = Data + StartPos; Tmp < EndStringPos; ++Tmp)
			{
				if (CharUtils::ToUpperCase(*Tmp) == FirstUpperSubstrChar && Private_Compare(Tmp, Substr, SubstrLen, SearchCase) == 0)
				{
					return static_cast<SizeType>(Tmp - Data);
				}
			}
		}

		return Npos;
	}

	TRNT_CONSTEXPR SizeType Private_FindLast(SizeType SubstrLen, ConstPointerType Substr, SizeType StartPos = Npos, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive) const
	{
		if (StartPos == Npos || StartPos > Len)
		{
			StartPos = Len;
		}

		if (SubstrLen > Len)
		{
			return Npos;
		}

		if (*Substr == (ElementType)0 || SubstrLen == 0)
		{
			return StartPos;
		}

		if (SearchCase == TStringSearchCase::ECaseSensitive)
		{
			for (ConstPointerType Tmp = Data + (TRNT_MIN(StartPos, Len - SubstrLen));; --Tmp)
			{
				if (*Tmp == *Substr && Private_Compare(Tmp, Substr, SubstrLen, SearchCase) == 0)
				{
					return static_cast<SizeType>(Tmp - Data);
				}

				if (Tmp == Data)
				{
					break;
				}
			}
		}
		else
		{
			const ElementType FirstUpperSubstrChar = CharUtils::ToUpperCase(*Substr);

			for (ConstPointerType Tmp = Data + (TRNT_MIN(StartPos, Len - SubstrLen)); Tmp != Data; --Tmp)
			{
				if (CharUtils::ToUpperCase(*Tmp) == FirstUpperSubstrChar && Private_Compare(Tmp, Substr, SubstrLen, SearchCase) == 0)
				{
					return static_cast<SizeType>(Tmp - Data);
				}

				if (Tmp == Data)
				{
					break;
				}
			}
		}

		return Npos;
	}

	ConstPointerType Data;
	SizeType Len;
};

using TStringView = TStringViewBase<char>;
using TWStringView = TStringViewBase<wchar_t>;