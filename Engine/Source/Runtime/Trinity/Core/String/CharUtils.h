#pragma once

#include "Trinity/Core/Defines.h"
#include "Trinity/Core/TypeTraits/IsCharTypeSupported.h"
#include "Trinity/Core/TypeTraits/Logical.h"
#include "Trinity/Core/TypeTraits/RemoveCV.h"
#include "Trinity/Core/TypeTraits/TypeRelationships.h"
#include "Trinity/Core/Types/DataTypes.h"

#include <ctype.h>
#include <wctype.h>

template<typename T>
class TCharUtils
{
public:
	static_assert(TIsCharTypeSupported<T>::Value, "TCharUtils<T> is not implemented for this char type.");

	using CharType = typename TRemoveCVRef<T>::Type;

	static constexpr TBool IsCharType = TAreTheSameType<CharType, TChar>::Value;
	static constexpr TBool IsWCharType = TAreTheSameType<CharType, TWChar>::Value;

public:
	template<TInt32 NumBytes>
	class TUnsignedIntTypeChooser
	{};

	template<>
	class TUnsignedIntTypeChooser<1>
	{
	public:
		using Type = TUInt8;
	};

	template<>
	class TUnsignedIntTypeChooser<2>
	{
	public:
		using Type = TUInt16;
	};

	template<>
	class TUnsignedIntTypeChooser<4>
	{
	public:
		using Type = TUInt32;
	};

	template<>
	class TUnsignedIntTypeChooser<8>
	{
	public:
		using Type = TUInt64;
	};

	static TRNT_CONSTEXPR TRNT_FORCE_INLINE TUInt32 ToUnsigned(CharType Ch) noexcept
	{
		return static_cast<typename TUnsignedIntTypeChooser<sizeof(CharType)>::Type>(Ch);
	}

public:
	static TRNT_CONSTEXPR TRNT_FORCE_INLINE CharType ToUpperCase(CharType Ch)
	{
		if constexpr (IsCharType)
		{
			return (CharType)(ToUnsigned(Ch) - ((TUInt32(Ch) - 'a' < 26u) << 5));
		}
		else if constexpr (IsWCharType)
		{
			return (CharType)(ToUnsigned(Ch) - ((TUInt32(Ch) - L'a' < 26u) << 5));
		}
	}

	static TRNT_FORCE_INLINE CharType ToLowerCase(CharType Ch)
	{
		if constexpr (IsCharType)
		{
			return (CharType)(ToUnsigned(Ch) + ((TUInt32(Ch) - 'A' < 26u) << 5));
		}
		else if constexpr (IsWCharType)
		{
			return (CharType)(ToUnsigned(Ch) + ((TUInt32(Ch) - L'A' < 26u) << 5));
		}
	}

	static TRNT_FORCE_INLINE bool IsUpper(CharType Ch)
	{
		if constexpr (IsCharType)
		{
			return ::isupper(Ch) != 0;
		}
		else if constexpr (IsWCharType)
		{
			return ::iswupper(Ch) != 0;
		}
	}

	static TRNT_FORCE_INLINE bool IsLower(CharType Ch)
	{
		if constexpr (IsCharType)
		{
			return ::islower(Ch) != 0;
		}
		else if constexpr (IsWCharType)
		{
			return ::iswlower(Ch) != 0;
		}
	}

	static TRNT_FORCE_INLINE bool IsAlpha(CharType Ch)
	{
		if constexpr (IsCharType)
		{
			return ::isalpha(Ch) != 0;
		}
		else if constexpr (IsWCharType)
		{
			return ::iswalpha(Ch) != 0;
		}
	}

	static TRNT_FORCE_INLINE bool IsGraph(CharType Ch)
	{
		if constexpr (IsCharType)
		{
			return ::isgraph(Ch) != 0;
		}
		else if constexpr (IsWCharType)
		{
			return ::iswgraph(Ch) != 0;
		}
	}

	static TRNT_FORCE_INLINE bool IsPrint(CharType Ch)
	{
		if constexpr (IsCharType)
		{
			return ::isprint(Ch) != 0;
		}
		else if constexpr (IsWCharType)
		{
			return ::iswprint(Ch) != 0;
		}
	}

	static TRNT_FORCE_INLINE bool IsPunct(CharType Ch)
	{
		if constexpr (IsCharType)
		{
			return ::ispunct(Ch) != 0;
		}
		else if constexpr (IsWCharType)
		{
			return ::iswpunct(Ch) != 0;
		}
	}

	static TRNT_FORCE_INLINE bool IsAlnum(CharType Ch)
	{
		if constexpr (IsCharType)
		{
			return ::isalnum(Ch) != 0;
		}
		else if constexpr (IsWCharType)
		{
			return ::iswalnum(Ch) != 0;
		}
	}

	static TRNT_FORCE_INLINE bool IsDigit(CharType Ch)
	{
		if constexpr (IsCharType)
		{
			return ::isdigit(Ch) != 0;
		}
		else if constexpr (IsWCharType)
		{
			return ::iswdigit(Ch) != 0;
		}
	}

	static TRNT_FORCE_INLINE bool IsHexDigit(CharType Ch)
	{
		if constexpr (IsCharType)
		{
			return ::isxdigit(Ch) != 0;
		}
		else if constexpr (IsWCharType)
		{
			return ::iswxdigit(Ch) != 0;
		}
	}

	static TRNT_FORCE_INLINE bool IsWhitespace(CharType Ch)
	{
		if constexpr (IsCharType)
		{
			return ::isspace(Ch) != 0;
		}
		else if constexpr (IsWCharType)
		{
			return ::iswspace(Ch) != 0;
		}
	}

	static TRNT_FORCE_INLINE bool IsControl(CharType Ch)
	{
		if constexpr (IsCharType)
		{
			return ::iscntrl(Ch) != 0;
		}
		else if constexpr (IsWCharType)
		{
			return ::iswcntrl(Ch) != 0;
		}
	}

	static TRNT_FORCE_INLINE bool IsUnderscore(CharType Ch)
	{
		if constexpr (IsCharType)
		{
			return Ch == '_';
		}
		else if constexpr (IsWCharType)
		{
			return Ch == L'_';
		}
	}

	static TRNT_FORCE_INLINE bool IsBlank(CharType Ch)
	{
		if constexpr (IsCharType)
		{
			return ::isblank(Ch);
		}
		else if constexpr (IsWCharType)
		{
			return ::iswblank(Ch);
		}
	}

	static TRNT_FORCE_INLINE bool IsSpace(CharType Ch)
	{
		if constexpr (IsCharType)
		{
			return ::isspace(Ch);
		}
		else if constexpr (IsWCharType)
		{
			return ::iswspace(Ch);
		}
	}
};