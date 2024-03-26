#pragma once

#include <ctype.h>
#include <wctype.h>

#include "Trinity/Core/Types/DataTypes.h"
#include "Trinity/Core/Defines.h"

#include "Trinity/Core/TypeTraits/TypeRelationships.h"
#include "Trinity/Core/TypeTraits/Logical.h"
#include "Trinity/Core/TypeTraits/RemoveCV.h"

template<typename T>
class TCharUtils
{
public:
	using CharType = T;

private:
	template <TInt32 NumBytes>
	class TUnsignedIntTypeChooser {};

	template <> class TUnsignedIntTypeChooser<1> { public: using Type = TUInt8; };
	template <> class TUnsignedIntTypeChooser<2> { public: using Type = TUInt16; };
	template <> class TUnsignedIntTypeChooser<4> { public: using Type = TUInt32; };
	template <> class TUnsignedIntTypeChooser<8> { public: using Type = TUInt64; };

	static constexpr TBool IsCharType  = TAreTheSameType<typename TRemoveCV<CharType>::Type, TChar>::Value;
	static constexpr TBool IsWCharType = TAreTheSameType<typename TRemoveCV<CharType>::Type, TWChar>::Value;

	static constexpr TRNT_FORCE_INLINE TUInt32 ToUnsigned(CharType Ch) noexcept
	{
		return static_cast<typename TUnsignedIntTypeChooser<sizeof(CharType)>::Type>(Ch);
	}

	static_assert(IsCharType || IsWCharType, "TCharUtils<T> only supports Char and WChar.");

public:
	static TRNT_FORCE_INLINE CharType ToUpperCase(CharType Ch)
	{
		return (CharType)(ToUnsigned(Ch) - ((TUInt32(Ch) - 'a' < 26u) << 5));
	}

	static TRNT_FORCE_INLINE CharType ToLowerCase(CharType Ch)
	{
		return (CharType)(ToUnsigned(Ch) + ((TUInt32(Ch) - 'A' < 26u) << 5));
	}

	static TRNT_FORCE_INLINE bool IsUpper(CharType Ch)
	{
		if constexpr (IsCharType)
			return ::isupper(Ch) != 0;
		else
			return ::iswupper(Ch) != 0;
	}

	static TRNT_FORCE_INLINE bool IsLower(CharType Ch)
	{
		if constexpr (IsCharType)
			return ::islower(Ch) != 0;
		else
			return ::iswlower(Ch) != 0;
	}

	static TRNT_FORCE_INLINE bool IsAlpha(CharType Ch)
	{
		if constexpr (IsCharType)
			return ::isalpha(Ch) != 0;
		else
			return ::iswalpha(Ch) != 0;
	}

	static TRNT_FORCE_INLINE bool IsGraph(CharType Ch)
	{
		if constexpr (IsCharType)
			return ::isgraph(Ch) != 0;
		else
			return ::iswgraph(Ch) != 0;
	}

	static TRNT_FORCE_INLINE bool IsPrint(CharType Ch)
	{
		if constexpr (IsCharType)
			return ::isprint(Ch) != 0;
		else
			return ::iswprint(Ch) != 0;
	}

	static TRNT_FORCE_INLINE bool IsPunct(CharType Ch)
	{
		if constexpr (IsCharType)
			return ::ispunct(Ch) != 0;
		else
			return ::iswpunct(Ch) != 0;
	}

	static TRNT_FORCE_INLINE bool IsAlnum(CharType Ch)
	{
		if constexpr (IsCharType)
			return ::isalnum(Ch) != 0;
		else
			return ::iswalnum(Ch) != 0;
	}

	static TRNT_FORCE_INLINE bool IsDigit(CharType Ch)
	{
		if constexpr (IsCharType)
			return ::isdigit(Ch) != 0;
		else
			return ::iswdigit(Ch) != 0;
	}

	static TRNT_FORCE_INLINE bool IsHexDigit(CharType Ch)
	{
		if constexpr (IsCharType)
			return ::isxdigit(Ch) != 0;
		else
			return ::iswxdigit(Ch) != 0;
	}

	static TRNT_FORCE_INLINE bool IsWhitespace(CharType Ch)
	{
		if constexpr (IsCharType)
			return ::isspace(Ch) != 0;
		else
			return ::iswspace(Ch) != 0;
	}

	static TRNT_FORCE_INLINE bool IsControl(CharType Ch)
	{
		if constexpr (IsCharType)
			return ::iscntrl(Ch) != 0;
		else
			return ::iswcntrl(Ch) != 0;
	}

	static TRNT_FORCE_INLINE bool IsUnderscore(CharType Ch)
	{
		if constexpr (IsCharType)
			return Ch == '_';
		else
			return Ch == L'_';
	}

	static TRNT_FORCE_INLINE bool IsBlank(CharType Ch)
	{
		if constexpr (IsCharType)
			return ::isblank(Ch);
		else
			return ::iswblank(Ch);
	}

	static TRNT_FORCE_INLINE bool IsSpace(CharType Ch)
	{
		if constexpr (IsCharType)
			return ::isspace(Ch);
		else
			return ::iswspace(Ch);
	}
};