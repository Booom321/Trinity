#pragma once

#include "CString.h"
#include "CharUtils.h"
#include "Trinity/Core/Containers/DynamicArray.h"
#include "Trinity/Core/Memory/Memory.h"
#include "Trinity/Core/TypeTraits/IsStandardLayout.h"
#include "Trinity/Core/TypeTraits/Logical.h"
#include "Trinity/Core/TypeTraits/PrimaryTypes.h"
#include "Trinity/Core/TypeTraits/Trivial.h"
#include "Trinity/Core/Types/Iterators.h"

#include <type_traits>

enum class TStringSearchCase : TInt8
{
	EIgnoreCase,
	ECaseSensitive
};

#pragma warning(push)
#pragma warning(disable : 6011)
#pragma warning(disable : 6387)

template<typename T>
class TStringBase
{
public:
	static_assert(!TIsArray<T>::Value && TIsTrivial<T>::Value && TIsStandardLayout<T>::Value);
	static_assert(TIsCharTypeSupported<T>::Value, "TStringBase<T> is not implemented for this char type.");

	using SizeType = TInt64;
	using ElementType = T;
	using ConstReferenceType = const T&;
	using ReferenceType = T&;
	using ConstPointerType = const T*;
	using PointerType = T*;

	using IteratorType = TContiguousIterator<ElementType, SizeType>;
	using ConstIteratorType = TContiguousIterator<const ElementType, SizeType>;

	using CStringHelper = TCString<ElementType>;
	using CharUtils = TCharUtils<ElementType>;

	static TRNT_CONSTEXPR SizeType Npos = -1;
	static TRNT_CONSTEXPR TFloat GrowSize = 1.5f;

	static TRNT_CONSTEXPR ElementType NullChar = static_cast<ElementType>(0);

private:
	TRNT_FORCE_INLINE void ConstructData(SizeType StrLen, ConstPointerType Str)
	{
		Data = static_cast<PointerType>(malloc((Cap + 1) * sizeof(ElementType)));

		if (StrLen > 0 && Str)
		{
			memcpy(Data, Str, StrLen * sizeof(ElementType));
		}

		Data[StrLen] = NullChar;
	}

	TRNT_FORCE_INLINE void ConstructData(SizeType Count, const ElementType Chr)
	{
		Data = static_cast<PointerType>(malloc((Count + 1) * sizeof(ElementType)));

		if (Count > 0)
		{
			CStringHelper::Memset(Data, Chr, Count);
		}

		Data[Count] = NullChar;
	}

public:
	TStringBase(TNullPtr) = delete;

	TStringBase()
		: Data(static_cast<PointerType>(malloc(sizeof(ElementType)))), Len(0), Cap(0)
	{
		*Data = NullChar;
	}

	TStringBase(const TStringBase& Str)
		: Len(Str.Len), Cap(Str.Len)
	{
		ConstructData(Len, Str.Data);
	}

	TStringBase(const TStringBase& Str, SizeType StartPos, SizeType StrLen)
	{
		TRNT_ASSERT(StartPos >= 0 && StrLen >= 0 && StartPos + StrLen <= Str.Len);
		Len = StrLen;
		Cap = StrLen;
		ConstructData(Len, Str.Data + StartPos);
	}

	TStringBase(TStringBase&& Str) noexcept
		: Data(Move(Str.Data)), Len(Str.Len), Cap(Str.Cap)
	{
		Str.Data = static_cast<PointerType>(malloc(sizeof(ElementType)));
		Str.Len = 0;
		Str.Cap = 0;
		*Str.Data = NullChar;
	}

	TStringBase(ConstPointerType Str)
		: Len(CStringHelper::Strlen(Str)), Cap(Len)
	{
		ConstructData(Len, Str);
	}

	TStringBase(SizeType StrLen, ConstPointerType Str)
	{
		TRNT_ASSERT(StrLen >= 0);
		Len = StrLen;
		Cap = StrLen;
		ConstructData(Len, Str);
	}

	TStringBase(SizeType Count, const ElementType Chr)
	{
		TRNT_ASSERT(Count >= 0);
		Len = Count;
		Cap = Count;
		ConstructData(Len, Chr);
	}

	~TStringBase()
	{
		free(Data);
		Data = nullptr;
	}

private:
	TStringBase(SizeType Len)
		: Data(static_cast<PointerType>(malloc((Len + 1) * sizeof(ElementType)))), Len(Len), Cap(Len)
	{
		Data[Len] = NullChar;
	}

public:
	TStringBase& operator=(TNullPtr) = delete;

	TStringBase& operator=(const TStringBase& Other)
	{
		if (this != &Other)
		{
			if (Other.Len > Cap)
			{
				free(Data);
				Data = static_cast<PointerType>(malloc((Other.Len + 1) * sizeof(ElementType)));
				memcpy(Data, Other.Data, Other.Len * sizeof(ElementType));
				Cap = Other.Len;
			}
			else
			{
				memcpy(Data, Other.Data, Other.Len * sizeof(ElementType));
			}
			Data[Other.Len] = NullChar;
			Len = Other.Len;
		}

		return *this;
	}

	TStringBase& operator=(ConstPointerType Other)
	{
		if (Data != Other)
		{
			Len = CStringHelper::Strlen(Other);

			if (Len > Cap)
			{
				free(Data);
				Data = static_cast<PointerType>(malloc((Len + 1) * sizeof(ElementType)));
				memcpy(Data, Other, Len * sizeof(ElementType));
				Cap = Len;
			}
			else
			{
				memcpy(Data, Other, Len * sizeof(ElementType));
			}
			Data[Len] = NullChar;
		}

		return *this;
	}

	TStringBase& operator=(TStringBase&& Other) noexcept
	{
		if (this != &Other)
		{
			PointerType TmpData = Data;
			Data = Other.Data;
			Other.Data = TmpData;

			SizeType TmpCap = Cap;
			Cap = Other.Cap;
			Other.Cap = TmpCap;

			Len = Other.Len;

			Other.Len = 0;
			*Other.Data = NullChar;
		}
		return *this;
	}

public:
	TRNT_FORCE_INLINE PointerType GetData() noexcept
	{
		return Data;
	}

	TRNT_FORCE_INLINE ConstPointerType GetData() const noexcept
	{
		return Data;
	}

	TRNT_FORCE_INLINE SizeType GetElementCount() const noexcept
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

	TRNT_FORCE_INLINE ReferenceType CharAt(SizeType Index)
	{
		TRNT_ASSERT(Index >= 0 && Index < Len);
		return Data[Index];
	}

	TRNT_FORCE_INLINE ConstReferenceType CharAt(SizeType Index) const
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

	TRNT_FORCE_INLINE TBool IsEmpty() const
	{
		return Len == 0;
	}

public:
	TRNT_NODISCARD TRNT_FORCE_INLINE TBool IsEquals(ConstPointerType Other, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive) const
	{
		if (SearchCase == TStringSearchCase::ECaseSensitive)
		{
			return CStringHelper::Strcmp(Data, Other) == 0;
		}
		return CStringHelper::Stricmp(Data, Other) == 0;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE TBool IsEquals(const TStringBase& Other, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive) const
	{
		if (Len == Other.Len)
		{
			if (SearchCase == TStringSearchCase::ECaseSensitive)
			{
				return CStringHelper::Strcmp(Data, Other.Data) == 0;
			}
			return CStringHelper::Stricmp(Data, Other.Data) == 0;
		}
		return false;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE TInt32 Compare(ConstPointerType Other, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive) const
	{
		if (SearchCase == TStringSearchCase::ECaseSensitive)
		{
			return CStringHelper::Strcmp(Data, Other);
		}
		return CStringHelper::Stricmp(Data, Other);
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE TInt32 Compare(const TStringBase& Other, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive) const
	{
		if (SearchCase == TStringSearchCase::ECaseSensitive)
		{
			return CStringHelper::Strcmp(Data, Other.Data);
		}
		return CStringHelper::Stricmp(Data, Other.Data);
	}

	TRNT_NODISCARD TInt32 Compare(
		SizeType StartPos1,
		SizeType StrLen1,
		const TStringBase& Other,
		SizeType StartPos2,
		SizeType StrLen2,
		TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive) const
	{
		TRNT_ASSERT((StartPos1 >= 0 && StartPos1 + StrLen1 <= Len) && (StartPos2 >= 0 && StartPos2 + StrLen2 <= Other.Len));
		PointerType AFirst = Data + StartPos1;
		PointerType BFirst = Other.Data + StartPos2;
		if (SearchCase == TStringSearchCase::ECaseSensitive)
		{
			return CStringHelper::StrcmpRanges(AFirst, AFirst + StrLen1, BFirst, BFirst + StrLen2);
		}
		return CStringHelper::StricmpRanges(AFirst, AFirst + StrLen1, BFirst, BFirst + StrLen2);
	}

public:
	TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator==(const TStringBase& Rhs) const
	{
		return CStringHelper::Strcmp(Data, Rhs.Data) == 0;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator==(ConstPointerType Rhs) const
	{
		return CStringHelper::Strcmp(Data, Rhs) == 0;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE friend TBool operator==(ConstPointerType Lhs, const TStringBase& Rhs)
	{
		return CStringHelper::Strcmp(Lhs, Rhs.Data) == 0;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator!=(const TStringBase& Rhs) const
	{
		return CStringHelper::Strcmp(Data, Rhs.Data) != 0;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator!=(ConstPointerType Rhs) const
	{
		return CStringHelper::Strcmp(Data, Rhs) != 0;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE friend TBool operator!=(ConstPointerType Lhs, const TStringBase& Rhs)
	{
		return CStringHelper::Strcmp(Lhs, Rhs.Data) != 0;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator>(const TStringBase& Rhs) const
	{
		return CStringHelper::Strcmp(Data, Rhs.Data) > 0;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator>(ConstPointerType Rhs) const
	{
		return CStringHelper::Strcmp(Data, Rhs) > 0;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE friend TBool operator>(ConstPointerType Lhs, const TStringBase& Rhs)
	{
		return CStringHelper::Strcmp(Lhs, Rhs.Data) > 0;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator>=(const TStringBase& Rhs) const
	{
		return CStringHelper::Strcmp(Data, Rhs.Data) >= 0;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator>=(ConstPointerType Rhs) const
	{
		return CStringHelper::Strcmp(Data, Rhs) >= 0;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE friend TBool operator>=(ConstPointerType Lhs, const TStringBase& Rhs)
	{
		return CStringHelper::Strcmp(Lhs, Rhs.Data) >= 0;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator<(const TStringBase& Rhs) const
	{
		return CStringHelper::Strcmp(Data, Rhs.Data) < 0;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator<(ConstPointerType Rhs) const
	{
		return CStringHelper::Strcmp(Data, Rhs) < 0;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE friend TBool operator<(ConstPointerType Lhs, const TStringBase& Rhs)
	{
		return CStringHelper::Strcmp(Lhs, Rhs.Data) < 0;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator<=(const TStringBase& Rhs) const
	{
		return CStringHelper::Strcmp(Data, Rhs.Data) <= 0;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator<=(ConstPointerType Rhs) const
	{
		return CStringHelper::Strcmp(Data, Rhs) <= 0;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE friend TBool operator<=(ConstPointerType Lhs, const TStringBase& Rhs)
	{
		return CStringHelper::Strcmp(Lhs, Rhs.Data) <= 0;
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
	TRNT_NODISCARD SizeType Find(SizeType SubstrLen, ConstPointerType Substr, SizeType StartPos = 0, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive) const
	{
		if (StartPos < 0)
		{
			StartPos = 0;
		}

		if (SubstrLen > Len || StartPos > Len - SubstrLen)
		{
			return Npos;
		}

		if (*Substr == NullChar || SubstrLen == 0)
		{
			return StartPos;
		}

		ConstPointerType EndStringPos = Data + (Len - SubstrLen) + 1;

		if (SearchCase == TStringSearchCase::ECaseSensitive)
		{
			for (ConstPointerType Tmp = Data + StartPos; Tmp < EndStringPos; ++Tmp)
			{
				if (*Tmp == *Substr && CStringHelper::Strncmp(Tmp, Substr, SubstrLen) == 0)
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
				if (CharUtils::ToUpperCase(*Tmp) == FirstUpperSubstrChar && CStringHelper::Strnicmp(Tmp, Substr, SubstrLen) == 0)
				{
					return static_cast<SizeType>(Tmp - Data);
				}
			}
		}

		return Npos;
	}

	TRNT_NODISCARD SizeType FindLast(SizeType SubstrLen, ConstPointerType Substr, SizeType StartPos = Npos, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive) const
	{
		if (StartPos == Npos || StartPos > Len)
		{
			StartPos = Len;
		}

		if (SubstrLen > Len)
		{
			return Npos;
		}

		if (*Substr == NullChar || SubstrLen == 0)
		{
			return StartPos;
		}

		if (SearchCase == TStringSearchCase::ECaseSensitive)
		{
			for (ConstPointerType Tmp = Data + (TRNT_MIN(StartPos, Len - SubstrLen));; --Tmp)
			{
				if (*Tmp == *Substr && CStringHelper::Strncmp(Tmp, Substr, SubstrLen) == 0)
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

			for (ConstPointerType Tmp = Data + (TRNT_MIN(StartPos, Len - SubstrLen));; --Tmp)
			{
				if (CharUtils::ToUpperCase(*Tmp) == FirstUpperSubstrChar && CStringHelper::Strnicmp(Tmp, Substr, SubstrLen) == 0)
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

	TRNT_NODISCARD TRNT_FORCE_INLINE SizeType Find(const TStringBase& Substr, SizeType StartPos = 0, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive) const
	{
		return Find(Substr.Len, Substr.Data, StartPos, SearchCase);
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE SizeType Find(ConstPointerType Substr, SizeType StartPos = 0, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive) const
	{
		return Find(CStringHelper::Strlen(Substr), Substr, StartPos, SearchCase);
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE SizeType FindLast(const TStringBase& Substr, SizeType StartPos = Npos, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive) const
	{
		return FindLast(Substr.Len, Substr.Data, StartPos, SearchCase);
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE SizeType FindLast(ConstPointerType Substr, SizeType StartPos = Npos, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive) const
	{
		return FindLast(CStringHelper::Strlen(Substr), Substr, StartPos, SearchCase);
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE SizeType FindChar(ElementType Chr, SizeType StartPos = 0, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive) const
	{
		if (StartPos < 0)
		{
			StartPos = 0;
		}

		if (SearchCase == TStringSearchCase::ECaseSensitive)
		{
			ConstPointerType Found = CStringHelper::Strchr(Data + StartPos, Chr);
			return Found ? static_cast<SizeType>(Found - Data) : Npos;
		}
		else
		{
			ConstPointerType Start = Data + StartPos;
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

	TRNT_NODISCARD SizeType FindLastChar(ElementType Chr, SizeType StartPos = Npos, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive) const
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

	TRNT_NODISCARD TBool Contains(ConstPointerType Substr, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive) const
	{
		if (SearchCase == TStringSearchCase::ECaseSensitive)
		{
			return CStringHelper::Strstr(Data, Substr) != nullptr;
		}
		return CStringHelper::Stristr(Data, Substr) != nullptr;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE TBool Contains(const TStringBase& Substr, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive) const
	{
		return Find(Substr.Len, Substr.Data, 0, SearchCase) != Npos;
	}

public:
	TRNT_NODISCARD TRNT_FORCE_INLINE TStringBase Substring(SizeType StartPos) const
	{
		TRNT_ASSERT(StartPos >= 0 && StartPos < Len);
		return TStringBase(Len - StartPos, Data + StartPos);
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE TStringBase Substring(SizeType StartPos, SizeType StringLen) const
	{
		TRNT_ASSERT(StartPos >= 0 && StartPos + StringLen <= Len);
		return TStringBase(StringLen, Data + StartPos);
	}

	void ToUpperInternal()
	{
		for (SizeType i = 0; i < Len; ++i)
		{
			Data[i] = CharUtils::ToUpperCase(Data[i]);
		}
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE TStringBase ToUpper() const
	{
		TStringBase Str(Len, Data);
		Str.ToUpperInternal();
		return Str;
	}

	void ToLowerInternal()
	{
		for (SizeType i = 0; i < Len; ++i)
		{
			Data[i] = CharUtils::ToLowerCase(Data[i]);
		}
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE TStringBase ToLower() const
	{
		TStringBase Str(Len, Data);
		Str.ToLowerInternal();
		return Str;
	}

private:
	TRNT_NODISCARD static TStringBase Concat(ConstPointerType Lhs, SizeType LhsLen, ConstPointerType Rhs, SizeType RhsLen)
	{
		TRNT_ASSERT(LhsLen >= 0 && RhsLen >= 0);

		TStringBase Result(LhsLen + RhsLen);

		memcpy(Result.Data, Lhs, LhsLen * sizeof(ElementType));
		memcpy(Result.Data + LhsLen, Rhs, RhsLen * sizeof(ElementType));

		return Result;
	}

	TRNT_NODISCARD static TStringBase Concat(TStringBase&& Lhs, TStringBase&& Rhs)
	{
		TStringBase Result(Lhs.Len + Rhs.Len);
		memcpy(Result.Data, Lhs.Data, Lhs.Len * sizeof(ElementType));
		memcpy(Result.Data + Lhs.Len, Rhs.Data, Rhs.Len * sizeof(ElementType));

		Lhs.Len = 0;
		*Lhs.Data = NullChar;
		Rhs.Len = 0;
		*Rhs.Data = NullChar;

		return Result;
	}

	TRNT_NODISCARD static TStringBase Concat(TStringBase&& Lhs, ConstPointerType Rhs, SizeType RhsLen)
	{
		TStringBase Result(Lhs.Len + RhsLen);

		memcpy(Result.Data, Lhs.Data, Lhs.Len * sizeof(ElementType));
		memcpy(Result.Data + Lhs.Len, Rhs, RhsLen * sizeof(ElementType));

		Lhs.Len = 0;
		*Lhs.Data = NullChar;

		return Result;
	}

	TRNT_NODISCARD static TStringBase Concat(ConstPointerType Lhs, SizeType LhsLen, TStringBase&& Rhs)
	{
		TStringBase Result(LhsLen + Rhs.Len);

		memcpy(Result.Data, Lhs, LhsLen * sizeof(ElementType));
		memcpy(Result.Data + LhsLen, Rhs.Data, Rhs.Len * sizeof(ElementType));

		Rhs.Len = 0;
		*Rhs.Data = NullChar;

		return Result;
	}

public:
	TRNT_NODISCARD TRNT_FORCE_INLINE friend TStringBase operator+(const TStringBase& Lhs, const TStringBase& Rhs)
	{
		return Concat(Lhs.Data, Lhs.Len, Rhs.Data, Rhs.Len);
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE friend TStringBase operator+(const TStringBase& Lhs, ConstPointerType Rhs)
	{
		return Concat(Lhs.Data, Lhs.Len, Rhs, CStringHelper::Strlen(Rhs));
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE friend TStringBase operator+(const TStringBase& Lhs, TStringBase&& Rhs)
	{
		return Concat(Lhs.Data, Lhs.Len, Move(Rhs));
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE friend TStringBase operator+(ConstPointerType Lhs, const TStringBase& Rhs)
	{
		return Concat(Lhs, CStringHelper::Strlen(Lhs), Rhs.Data, Rhs.Len);
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE friend TStringBase operator+(ConstPointerType Lhs, TStringBase&& Rhs)
	{
		return Concat(Lhs, CStringHelper::Strlen(Lhs), Move(Rhs));
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE friend TStringBase operator+(TStringBase&& Lhs, const TStringBase& Rhs)
	{
		return Concat(Move(Lhs), Rhs.Data, Rhs.Len);
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE friend TStringBase operator+(TStringBase&& Lhs, TStringBase&& Rhs)
	{
		return Concat(Move(Lhs), Move(Rhs));
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE friend TStringBase operator+(TStringBase&& Lhs, ConstPointerType Rhs)
	{
		return Concat(Move(Lhs), Rhs, CStringHelper::Strlen(Rhs));
	}

public:
	TRNT_FORCE_INLINE void Clear() noexcept
	{
		Len = 0;
		*Data = NullChar;
	}

	void Resize(SizeType NewLength, ElementType Chr = NullChar)
	{
		TRNT_ASSERT(NewLength >= 0);

		if (NewLength == Len)
		{
			return;
		}

		if (NewLength > Cap)
		{
			PointerType NewData = static_cast<PointerType>(malloc((NewLength + 1) * sizeof(ElementType)));
			memcpy(NewData, Data, Len * sizeof(ElementType));
			free(Data);
			Data = NewData;
			Cap = NewLength;
		}

		if (NewLength > Len)
		{
			CStringHelper::Memset(Data + Len, Chr, NewLength - Len);
		}

		Data[NewLength] = NullChar;
		Len = NewLength;
	}

	void Reserve(SizeType NewCapacity)
	{
		if (NewCapacity > Cap)
		{
			PointerType NewData = static_cast<PointerType>(malloc((NewCapacity + 1) * sizeof(ElementType)));
			memcpy(NewData, Data, Len * sizeof(ElementType));
			free(Data);
			Data = NewData;
			Cap = NewCapacity;
			Data[Len] = NullChar;
		}
	}

public:
	TRNT_NODISCARD TBool StartsWith(ConstPointerType PrefixStr, SizeType PrefixLen, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive) const
	{
		if (SearchCase == TStringSearchCase::ECaseSensitive)
		{
			return PrefixLen >= 0 && CStringHelper::Strncmp(Data, PrefixStr, PrefixLen) == 0;
		}
		return PrefixLen >= 0 && CStringHelper::Strnicmp(Data, PrefixStr, PrefixLen) == 0;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE TBool StartsWith(ConstPointerType PrefixStr, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive) const
	{
		return StartsWith(PrefixStr, CStringHelper::Strlen(PrefixStr), SearchCase);
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE TBool StartsWith(const TStringBase& PrefixStr, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive) const
	{
		return StartsWith(PrefixStr.Data, PrefixStr.Len, SearchCase);
	}

	//////////////////////////////////////////////////////////////////////////////////////
	TRNT_NODISCARD TBool EndsWith(ConstPointerType SuffixStr, SizeType SuffixLen, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive) const
	{
		if (SearchCase == TStringSearchCase::ECaseSensitive)
		{
			return SuffixLen >= 0 && Len >= SuffixLen && CStringHelper::Strncmp(Data + (Len - SuffixLen), SuffixStr, SuffixLen) == 0;
		}
		return SuffixLen >= 0 && Len >= SuffixLen && CStringHelper::Strnicmp(Data + (Len - SuffixLen), SuffixStr, SuffixLen) == 0;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE TBool EndsWith(ConstPointerType SuffixStr, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive) const
	{
		return EndsWith(SuffixStr, CStringHelper::Strlen(SuffixStr), SearchCase);
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE TBool EndsWith(const TStringBase& SuffixStr, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive) const
	{
		return EndsWith(SuffixStr.Data, SuffixStr.Len, SearchCase);
	}

public:
	void ReverseInternal()
	{
		if (Len > 1)
		{
			PointerType Start = Data;
			PointerType End = Data + (Len - 1);
			ElementType TmpChar;
			do
			{
				TmpChar = *Start;
				*Start = *End;
				*End = TmpChar;

				Start++, --End;
			} while (Start < End);
		}
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE TStringBase Reverse() const
	{
		TStringBase Result(Len, Data);
		Result.ReverseInternal();
		return Result;
	}

public:
	TStringBase& Append(ConstPointerType Str, SizeType StrLen)
	{
		TRNT_ASSERT(Str != nullptr && StrLen >= 0);

		if (Len + StrLen > Cap)
		{
			Cap = static_cast<SizeType>(Cap * GrowSize + StrLen);
			PointerType NewData = static_cast<PointerType>(malloc((Cap + 1) * sizeof(ElementType)));
			memcpy(NewData, Data, Len * sizeof(ElementType));
			memcpy(NewData + Len, Str, StrLen * sizeof(ElementType));
			free(Data);
			Data = NewData;
		}
		else
		{
			memcpy(Data + Len, Str, StrLen * sizeof(ElementType));
		}

		Len += StrLen;
		Data[Len] = NullChar;
		return *this;
	}

	TStringBase& Append(SizeType Count, ElementType Chr = NullChar)
	{
		TRNT_ASSERT(Count >= 0);

		if (Len + Count > Cap)
		{
			Cap = static_cast<SizeType>(Cap * GrowSize + Count);
			PointerType NewData = static_cast<PointerType>(malloc((Cap + 1) * sizeof(ElementType)));
			memcpy(NewData, Data, Len * sizeof(ElementType));
			CStringHelper::Memset(NewData + Len, static_cast<int>(Chr), Count);
			free(Data);
			Data = NewData;
		}
		else
		{
			CStringHelper::Memset(Data + Len, static_cast<int>(Chr), Count);
		}

		Len += Count;
		Data[Len] = NullChar;
		return *this;
	}

	TRNT_FORCE_INLINE TStringBase& Append(const TStringBase& Str)
	{
		return Append(Str.Data, Str.Len);
	}

	TRNT_FORCE_INLINE TStringBase& Append(ConstPointerType Str)
	{
		return Append(Str, CStringHelper::Strlen(Str));
	}

	TRNT_FORCE_INLINE TStringBase& Append(const TStringBase& Str, SizeType StartPos, SizeType StrLen)
	{
		TRNT_ASSERT(StartPos >= 0 && StartPos + StrLen <= Str.Len);
		return Append(Str.Data + StartPos, StrLen);
	}

	TRNT_FORCE_INLINE TStringBase& operator+=(const TStringBase& Str)
	{
		return Append(Str.Data, Str.Len);
	}

	TRNT_FORCE_INLINE TStringBase& operator+=(ConstPointerType Str)
	{
		return Append(Str, CStringHelper::Strlen(Str));
	}

	TRNT_FORCE_INLINE TStringBase& operator+=(ElementType Chr)
	{
		return Append(1, Chr);
	}

	void RemoveAt(SizeType StartPos, SizeType StrLen)
	{
		TRNT_ASSERT(StartPos >= 0 && StartPos + StrLen <= Len && StrLen >= 0);

		if (StartPos == 0 && StrLen == Len)
		{
			Clear();
			return;
		}

		Len -= StrLen;

		PointerType Dest = Data + StartPos;
		memcpy(Dest, Dest + StrLen, (Len - StartPos) * sizeof(ElementType));
		Data[Len] = NullChar;
	}

public:
	void Insert(SizeType Index, ConstPointerType Str, SizeType StrLen)
	{
		TRNT_ASSERT(Index >= 0 && Index <= Len && StrLen >= 0 && Str != nullptr);

		if (Len + StrLen > Cap)
		{
			Cap = static_cast<SizeType>(Cap * GrowSize + StrLen);
			PointerType NewData = static_cast<PointerType>(malloc((Cap + 1) * sizeof(ElementType)));
			PointerType Dest = NewData + Index;
			memcpy(NewData, Data, Index * sizeof(ElementType));
			memcpy(Dest, Str, StrLen * sizeof(ElementType));
			memcpy(Dest + StrLen, Data + Index, (Len - Index) * sizeof(ElementType));
			free(Data);
			Data = NewData;
		}
		else
		{
			PointerType Dest = Data + Index;
			memmove(Dest + StrLen, Dest, (Len - Index) * sizeof(ElementType));
			memcpy(Dest, Str, StrLen * sizeof(ElementType));
		}

		Len += StrLen;
		Data[Len] = NullChar;
	}

	void Insert(SizeType Index, SizeType Count, ElementType Chr = NullChar)
	{
		TRNT_ASSERT(Index >= 0 && Index <= Len && Count >= 0);

		if (Len + Count > Cap)
		{
			Cap = static_cast<SizeType>(Cap * GrowSize + Count);
			PointerType NewData = static_cast<PointerType>(malloc((Cap + 1) * sizeof(ElementType)));
			PointerType Dest = NewData + Index;
			memcpy(NewData, Data, Index * sizeof(ElementType));
			CStringHelper::Memset(Dest, Chr, Count);
			memcpy(Dest + Count, Data + Index, (Len - Index) * sizeof(ElementType));
			free(Data);
			Data = NewData;
		}
		else
		{
			PointerType Dest = Data + Index;
			memmove(Dest + Count, Dest, (Len - Index) * sizeof(ElementType));
			CStringHelper::Memset(Dest, Chr, Count);
		}

		Len += Count;
		Data[Len] = NullChar;
	}

	TRNT_FORCE_INLINE void Insert(SizeType Index, ConstPointerType Str)
	{
		Insert(Index, Str, CStringHelper::Strlen(Str));
	}

	TRNT_FORCE_INLINE void Insert(SizeType Index, const TStringBase& Str)
	{
		Insert(Index, Str.Data, Str.Len);
	}

	TRNT_FORCE_INLINE void Insert(SizeType Index, const TStringBase& Str, SizeType StartPos, SizeType StrLen)
	{
		TRNT_ASSERT(StartPos >= 0 && StartPos + StrLen <= Str.Len);
		Insert(Index, Str.Data + StartPos, StrLen);
	}

public:
	TBool SplitByString(TDynamicArray<TStringBase>& Result, ConstPointerType Substring)
	{
		SizeType LastOffset = 0, Offset;
		PointerType Found = CStringHelper::Strstr(Data, Substring);

		while (Found != nullptr)
		{
			Offset = static_cast<SizeType>(Found - Data);
			SizeType Count = Offset - LastOffset;
			if (Count > 0)
			{
				Result.EmplaceBack(*this, LastOffset, Count);
			}
			LastOffset = Offset + 1;
			Found = CStringHelper::Strstr(Data + LastOffset, Substring);
		}

		Offset = Len - LastOffset;

		if (Offset > 0)
		{
			Result.EmplaceBack(*this, LastOffset, Offset);
		}

		return true;
	}

	TBool SplitBySeparators(TDynamicArray<TStringBase>& Result, ConstPointerType Separators)
	{
		SizeType LastOffset = 0, Offset;
		PointerType Found = CStringHelper::Strpbrk(Data, Separators);

		while (Found != nullptr)
		{
			Offset = static_cast<SizeType>(Found - Data);
			SizeType Count = Offset - LastOffset;
			if (Count > 0)
			{
				Result.EmplaceBack(*this, LastOffset, Count);
			}
			LastOffset = Offset + 1;
			Found = CStringHelper::Strpbrk(Data + LastOffset, Separators);
		}

		Offset = Len - LastOffset;

		if (Offset > 0)
		{
			Result.EmplaceBack(*this, LastOffset, Offset);
		}

		return true;
	}

public:
	void TrimStartInternal()
	{
		SizeType Position = 0;
		while (Position < Len)
		{
			if constexpr (TAreTheSameType<ElementType, TChar>::Value)
			{
				if (!::isspace(Data[Position]))
				{
					break;
				}
			}
			else // WChar
			{
				if (!::iswspace(Data[Position]))
				{
					break;
				}
			}
			++Position;
		}

		RemoveAt(0, Position);
	}

	void TrimEndInternal()
	{
		SizeType End = Len;
		while (End > 0)
		{
			if constexpr (TAreTheSameType<ElementType, TChar>::Value)
			{
				if (!::isspace(Data[End - 1]))
				{
					break;
				}
			}
			else // WChar
			{
				if (!::iswspace(Data[End - 1]))
				{
					break;
				}
			}
			--End;
		}

		RemoveAt(End, Len - End);
	}

	void TrimStartAndEndInternal()
	{
		TrimStartInternal();
		TrimEndInternal();
	}

public:
	void AppendPath(ConstPointerType PathString, SizeType PathLen)
	{
		if (!PathString || PathLen < 0)
		{
			return;
		}
		// 47 => '/'
		// 92 => '\\'
		if (Len > 0 && Data[Len - 1] != 47 && Data[Len - 1] != 92 && (PathLen == 0 || (*PathString != 47 && *PathString != 92)))
		{
			Append(1, 47);
		}
		else
		{
			if ((Len > 0 && (Data[Len - 1] == 47 || Data[Len - 1] == 92)) && (PathLen > 0 && (*PathString == 47 || *PathString == 92)))
			{
				++PathString;
				--PathLen;
			}
		}
		Append(PathString, PathLen);
	}

	TRNT_FORCE_INLINE void AppendPath(ConstPointerType PathString)
	{
		return AppendPath(PathString, CStringHelper::Strlen(PathString));
	}

	TRNT_FORCE_INLINE TStringBase& operator/=(ConstPointerType PathString)
	{
		AppendPath(PathString, CStringHelper::Strlen(PathString));
		return *this;
	}

	TRNT_FORCE_INLINE TStringBase& operator/=(const TStringBase& PathString)
	{
		AppendPath(PathString.Data, PathString.Len);
		return *this;
	}

public:
	TRNT_NODISCARD TRNT_FORCE_INLINE friend TStringBase operator/(const TStringBase& PathA, const TStringBase& PathB)
	{
		TStringBase Result(PathA);
		Result.AppendPath(PathB.Data, PathB.Len);
		return Result;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE friend TStringBase operator/(const TStringBase& PathA, ConstPointerType PathB)
	{
		TStringBase Result(PathA);
		Result.AppendPath(PathB, CStringHelper::Strlen(PathB));
		return Result;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE friend TStringBase operator/(TStringBase&& PathA, const TStringBase& PathB)
	{
		TStringBase Result(Move(PathA));
		Result.AppendPath(PathB.Data, PathB.Len);
		return Result;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE friend TStringBase operator/(TStringBase&& PathA, ConstPointerType PathB)
	{
		TStringBase Result(Move(PathA));
		Result.AppendPath(PathB, CStringHelper::Strlen(PathB));
		return Result;
	}

private:
	friend struct std::hash<TStringBase<T>>;

	PointerType Data;
	SizeType Len;
	SizeType Cap;
};

#pragma warning(pop)

using TString = TStringBase<TChar>;
using TWString = TStringBase<TWChar>;