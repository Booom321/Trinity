#pragma once

#include "Trinity/Core/Defines.h"
#include "Trinity/Core/Types/DataTypes.h"
#include "Trinity/Core/Assert/AssertionMacros.h"

#include "CharUtils.h"

#include "Trinity/Core/TypeTraits/Logical.h"
#include "Trinity/Core/TypeTraits/RemoveCV.h"
#include "Trinity/Core/TypeTraits/TypeRelationships.h"

template<typename T>
class TCString
{
private:
	static constexpr TBool IsCharType = TAreTheSameType<typename TRemoveCV<T>::Type, TChar>::Value;
	static constexpr TBool IsWCharType = TAreTheSameType<typename TRemoveCV<T>::Type, TWChar>::Value;

public:
	using CharType = T;
	using SizeType = TSize_T;

	static_assert(IsCharType || IsWCharType, "TCString<T> only supports Char and WChar.");

public:
	static TRNT_FORCE_INLINE SizeType Strlen(const CharType* Str)
	{
		if constexpr (IsCharType)
		{
			return strlen(Str);
		}
		else
		{
			return wcslen(Str);
		}
	}

	static TRNT_FORCE_INLINE TInt32 Strcmp(const CharType* A, const CharType* B)
	{
		if constexpr (IsCharType)
		{
			return strcmp(A, B);
		}
		else
		{
			return wcscmp(A, B);
		}
	}

	static TRNT_FORCE_INLINE TInt32 Stricmp(const CharType* A, const CharType* B)
	{
		TInt32 Diff;
		while ((Diff = TCharUtils<CharType>::ToUpperCase(*A) - TCharUtils<CharType>::ToUpperCase(*B)) == 0)
		{
			if (*A == static_cast<CharType>(0)) 
			{
				return 0;
			}
			++A;
			++B;
		}
		return Diff > 0 ? 1 : -1;
	}

	static TRNT_FORCE_INLINE TInt32 Strncmp(const CharType* A, const CharType* B, SizeType Count)
	{
		if constexpr (IsCharType)
		{
			return strncmp(A, B, Count);
		}
		else
		{
			return wcsncmp(A, B, Count);
		}
	}

	static TRNT_FORCE_INLINE TInt32 Strnicmp(const CharType* A, const CharType* B, SizeType Count)
	{
		if (Count == 0)
			return 0;

		TInt32 Diff;
		for (;	Count > 0; --Count)
		{
			if (Diff = TCharUtils<CharType>::ToUpperCase(*A) - TCharUtils<CharType>::ToUpperCase(*B))
			{
				return Diff > 0 ? 1 : -1;
			}

			if (*A == static_cast<CharType>(0))
			{
				return 0;
			}

			++A;
			++B;
		}

		return 0;
	}

	static TRNT_FORCE_INLINE TInt32 StrcmpRanges(const CharType* AFirst, const CharType* ALast, const CharType* BFirst, const CharType* BLast)
	{
		SizeType ALen = static_cast<SizeType>(ALast - AFirst);
		SizeType BLen = static_cast<SizeType>(BLast - BFirst);
		TInt32 Result;

		if constexpr (IsCharType)
		{
			Result = strncmp(AFirst, BFirst, TRNT_MIN(ALen, BLen));
		}
		else
		{
			Result = wcsncmp(AFirst, BFirst, TRNT_MIN(ALen, BLen));
		}

		if (Result != 0)
		{
			return Result;
		}

		if (ALen < BLen) 
		{
			return -1;
		}

		if (ALen > BLen) 
		{
			return 1;
		}

		return 0;
	}

	static TRNT_FORCE_INLINE TInt32 StricmpRanges(const CharType* AFirst, const CharType* ALast, const CharType* BFirst, const CharType* BLast)
	{
		SizeType ALen = static_cast<SizeType>(ALast - AFirst);
		SizeType BLen = static_cast<SizeType>(BLast - BFirst);

		TInt32 Result = Strnicmp(AFirst, BFirst, TRNT_MIN(ALen, BLen));

		if (Result != 0)
		{
			return Result;
		}

		if (ALen < BLen)
		{
			return -1;
		}

		if (ALen > BLen)
		{
			return 1;
		}

		return 0;
	}

	static TRNT_FORCE_INLINE const CharType* Strchr(const CharType* Str, CharType Chr)
	{
		if constexpr (IsCharType)
		{
			return strchr(Str, Chr);
		}
		else
		{
			return wcschr(Str, Chr);
		}
	}

	static TRNT_FORCE_INLINE CharType* Strchr(CharType* Str, CharType Chr)
	{
		if constexpr (IsCharType)
		{
			return strchr(Str, Chr);
		}
		else
		{
			return wcschr(Str, Chr);
		}
	}

	static TRNT_FORCE_INLINE const CharType* Strrchr(const CharType* Str, CharType Chr)
	{
		if constexpr (IsCharType)
		{
			return strrchr(Str, Chr);
		}
		else
		{
			return wcsrchr(Str, Chr);
		}
	}

	static TRNT_FORCE_INLINE CharType* Strrchr(CharType* Str, CharType Chr)
	{
		if constexpr (IsCharType)
		{
			return strrchr(Str, Chr);
		}
		else
		{
			return wcsrchr(Str, Chr);
		}
	}

	static TRNT_FORCE_INLINE const CharType* Strstr(const CharType* Str, const CharType* Substr)
	{
		if constexpr (IsCharType)
		{
			return strstr(Str, Substr);
		}
		else
		{
			return wcsstr(Str, Substr);
		}
	}

	static TRNT_FORCE_INLINE CharType* Strstr(CharType* Str, const CharType* Substr)
	{
		if constexpr (IsCharType)
		{
			return strstr(Str, Substr);
		}
		else
		{
			return wcsstr(Str, Substr);
		}
	}

	static TRNT_FORCE_INLINE const CharType* Stristr(const CharType* Str, const CharType* Substr)
	{
		if (Str == nullptr || Substr == nullptr)
		{
			return nullptr;
		}

		if (*Substr == static_cast<CharType>(0))
		{
			return Str;
		}

		SizeType SubstrLen = Strlen(Substr);
		const CharType FirstUpperSubstrChar = TCharUtils<CharType>::ToUpperCase(*Substr);

		CharType StrChar = TCharUtils<CharType>::ToUpperCase(*Str);
		while (StrChar)
		{
			if (StrChar == FirstUpperSubstrChar && Strnicmp(Str, Substr, SubstrLen) == 0)
			{
				return Str;
			}

			StrChar = TCharUtils<CharType>::ToUpperCase(*(++Str));
		}
		return nullptr;
	}

	static TRNT_FORCE_INLINE CharType* Stristr(CharType* Str, const CharType* Substr)
	{
		return (CharType*)Stristr((const CharType*)Str, Substr);
	}

	static TRNT_FORCE_INLINE const CharType* Strrstr(const CharType* Str, const CharType* Substr)
	{
		if (Str == nullptr || Substr == nullptr)
		{
			return nullptr;
		}

		if (*Substr == static_cast<CharType>(0))
		{
			return Str + Strlen(Str);
		}

		SizeType StrLen = Strlen(Str);
		SizeType SubstrLen = Strlen(Substr);
	
		if (StrLen >= SubstrLen)
		{
			for (const CharType* Tmp = Str + (StrLen - SubstrLen); Tmp >= Str; --Tmp)
			{
				if (*Tmp == *Substr && Strncmp(Tmp, Substr, SubstrLen) == 0)
				{
					return Tmp;
				}
			}
		}

		return nullptr;
	}

	static TRNT_FORCE_INLINE CharType* Strrstr(CharType* Str, const CharType* Substr)
	{
		return (CharType*)Strrstr((const CharType*)Str, Substr);
	}

	static TRNT_FORCE_INLINE const CharType* Strristr(const CharType* Str, const CharType* Substr)
	{
		if (Str == nullptr || Substr == nullptr)
		{
			return nullptr;
		}

		if (*Substr == static_cast<CharType>(0))
		{
			return Str + Strlen(Str);
		}

		SizeType StrLen = Strlen(Str);
		SizeType SubstrLen = Strlen(Substr);

		if (StrLen >= SubstrLen)
		{
			const CharType FirstUpperSubstrChar = TCharUtils<CharType>::ToUpperCase(*Substr);
			for (const CharType* Tmp = Str + (StrLen - SubstrLen); Tmp >= Str; --Tmp)
			{
				if (TCharUtils<CharType>::ToUpperCase(*Tmp) == FirstUpperSubstrChar && Strnicmp(Tmp, Substr, SubstrLen) == 0)
				{
					return Tmp;
				}
			}
		}

		return nullptr;
	}

	static TRNT_FORCE_INLINE CharType* Strristr(CharType* Str, const CharType* Substr)
	{
		return (CharType*)Strristr((const CharType*)Str, Substr);
	}

	static TRNT_FORCE_INLINE CharType* Strupr(CharType* Dest)
	{
		CharType* Str = Dest;
		while (*Str)
		{
			*Str = TCharUtils<CharType>::ToUpperCase(*Str);
			++Str;
		}
		return Dest;
	}

	static TRNT_FORCE_INLINE CharType* Strupr(CharType* Dest, SizeType Count)
	{
		CharType* Str = Dest;
		while (*Str && Count)
		{
			*Str = TCharUtils<CharType>::ToUpperCase(*Str);
			++Str;
			--Count;
		}
		return Dest;
	}

	static TRNT_FORCE_INLINE CharType* Strlwr(CharType* Dest)
	{
		CharType* Str = Dest;
		while (*Str)
		{
			*Str = TCharUtils<CharType>::ToLowerCase(*Str);
			++Str;
		}
		return Dest;
	}

	static TRNT_FORCE_INLINE CharType* Strlwr(CharType* Dest, SizeType Count)
	{
		CharType* Str = Dest;
		while (*Str && Count)
		{
			*Str = TCharUtils<CharType>::ToLowerCase(*Str);
			++Str;
			--Count;
		}
		return Dest;
	}

	static TRNT_FORCE_INLINE const CharType* Strpbrk(const CharType* Str1, const CharType* Str2)
	{
		if constexpr (IsCharType)
		{
			return strpbrk(Str1, Str2);
		}
		else
		{
			return wcspbrk(Str1, Str2);
		}
	}

	static TRNT_FORCE_INLINE CharType* Strpbrk(CharType* Str1, const CharType* Str2)
	{
		if constexpr (IsCharType)
		{
			return strpbrk(Str1, Str2);
		}
		else
		{
			return wcspbrk(Str1, Str2);
		}
	}

	static TRNT_FORCE_INLINE const CharType* Strrpbrk(const CharType* Str1Begin, const CharType* Str1End, const CharType* Str2)
	{
		if (Str1Begin == nullptr || Str1End == nullptr || Str2 == nullptr)
		{
			return nullptr;
		}

		const CharType* Found = nullptr;

		for (--Str1End; Str1End != Str1Begin; --Str1End)
		{
			if constexpr (IsCharType)
			{
				Found = strchr(Str2, *Str1End);
			}
			else
			{
				Found = wcschr(Str2, *Str1End);
			}

			if (Found)
			{
				return Str1End;
			}
		}

		return nullptr;
	}

	static TRNT_FORCE_INLINE CharType* Strrpbrk(CharType* Str1, const CharType* Str2)
	{
		return (CharType*)Strrpbrk((const CharType*)Str1, Str2);
	}
};