#pragma once


#include "Hashable.h"
#include "xxHash.h"

#include "Trinity/Core/PlatformDetection.h"

#include "Trinity/Core/Assert/AssertionMacros.h"

#include "Trinity/Core/String/String.h"

#include "Trinity/Core/Types/Delegate.h"

#include "Trinity/Core/TypeTraits/EnableIf.h"
#include "Trinity/Core/TypeTraits/PrimaryTypes.h"
#include "Trinity/Core/TypeTraits/RemoveCV.h"

#include <functional>

#if defined(TRNT_PLATFORM_WIN64)
inline constexpr TSize_T FNV_OffsetBasis = 14695981039346656037ULL;
inline constexpr TSize_T FNV_Prime = 1099511628211ULL;
#else
inline constexpr TSize_T FNV_OffsetBasis = 2166136261U;
inline constexpr TSize_T FNV_Prime = 16777619U;
#endif

template<typename CharType>
static TSize_T GetHashCodeFromCString(const CharType* CString, TSize_T CSLen) noexcept
{
	TSize_T HashCode = FNV_OffsetBasis;

	const TSize_T Count = CSLen * sizeof(CharType);
	const unsigned char* const Data = reinterpret_cast<const unsigned char*>(CString);

	for (TSize_T Idx = 0; Idx < Count; ++Idx)
	{
		HashCode ^= static_cast<TSize_T>(Data[Idx]);
		HashCode *= FNV_Prime;
	}

	return HashCode;
}

template<typename CharType>
struct std::hash<TStringBase<CharType>>
{
	TRNT_FORCE_INLINE TSize_T operator()(const TStringBase<CharType>& String) const noexcept
	{
		return GetHashCodeFromCString<CharType>(String.Data, String.Len);
	}
};

template<typename ReturnType, typename ... Arguments>
struct std::hash<TDelegate<ReturnType(Arguments...)>>
{
	TRNT_FORCE_INLINE TSize_T operator()(const TDelegate<ReturnType(Arguments...)>& Delegate) const noexcept
	{
		auto const Seed(hash<void*>()(Delegate.ObjectPointer));

		return hash<decltype(Delegate.StubPointer)>()(Delegate.StubPointer) + 0x9e3779b9 + (Seed << 6) + (Seed >> 2);
	}
};

namespace TNsHash
{
	inline TSize_T GetHashCode(TUInt8 Value)
	{
		return static_cast<TSize_T>(XXH64(&Value, sizeof(TUInt8), 0));
	}

	inline TSize_T GetHashCode(TUInt16 Value)
	{
		return static_cast<TSize_T>(XXH64(&Value, sizeof(TUInt16), 0));
	}

	inline TSize_T GetHashCode(TUInt32 Value)
	{
		return static_cast<TSize_T>(XXH64(&Value, sizeof(TUInt32), 0));
	}

	inline TSize_T GetHashCode(TUInt64 Value)
	{
		return static_cast<TSize_T>(XXH64(&Value, sizeof(TUInt64), 0));
	}

	inline TSize_T GetHashCode(TInt8 Value)
	{
		return static_cast<TSize_T>(XXH64(&Value, sizeof(TInt8), 0));
	}

	inline TSize_T GetHashCode(TInt16 Value)
	{
		return static_cast<TSize_T>(XXH64(&Value, sizeof(TInt16), 0));
	}

	inline TSize_T GetHashCode(TInt32 Value)
	{
		return static_cast<TSize_T>(XXH64(&Value, sizeof(TInt32), 0));
	}

	inline TSize_T GetHashCode(TInt64 Value)
	{
		return static_cast<TSize_T>(XXH64(&Value, sizeof(TInt64), 0));
	}

	inline TSize_T GetHashCode(TFloat Value)
	{
		return static_cast<TSize_T>(XXH64(&Value, sizeof(TFloat), 0));
	}

	inline TSize_T GetHashCode(TDouble Value)
	{
		return static_cast<TSize_T>(XXH64(&Value, sizeof(TDouble), 0));
	}

	inline TSize_T GetHashCode(TBool Value)
	{
		return static_cast<TSize_T>(XXH64(&Value, sizeof(TBool), 0));
	}

	inline TSize_T GetHashCode(TChar Value)
	{
		return static_cast<TSize_T>(XXH64(&Value, sizeof(TChar), 0));
	}

	inline TSize_T GetHashCode(TWChar Value)
	{
		return static_cast<TSize_T>(XXH64(&Value, sizeof(TWChar), 0));
	}

#ifdef __cpp_char8_t

	inline TSize_T GetHashCode(TChar8 Value)
	{
		return static_cast<TSize_T>(XXH64(&Value, sizeof(TChar8), 0));
	}

#endif 
	inline TSize_T GetHashCode(TChar16 Value)
	{
		return static_cast<TSize_T>(XXH64(&Value, sizeof(TChar16), 0));
	}

	inline TSize_T GetHashCode(TChar32 Value)
	{
		return static_cast<TSize_T>(XXH64(&Value, sizeof(TChar32), 0));
	}

	inline TSize_T GetHashCode(const TChar* Value)
	{
		return GetHashCodeFromCString<TChar>(Value, strlen(Value));
	}

	inline TSize_T GetHashCode(const TWChar* Value)
	{
		return GetHashCodeFromCString<TWChar>(Value, wcslen(Value));
	}

	inline TSize_T GetHashCode(const TString& String)
	{
		return GetHashCodeFromCString<TChar>(String.GetData(), String.Length());
	}

	inline TSize_T GetHashCode(const TWString& WString)
	{
		return GetHashCodeFromCString<TWChar>(WString.GetData(), WString.Length());
	}

	inline TSize_T GetHashCode(const FXxHash32& Hash)
	{
		return static_cast<TSize_T>(Hash.HashCode);
	}

	inline TSize_T GetHashCode(const FXxHash64& Hash)
	{
		return static_cast<TSize_T>(Hash.HashCode);
	}

	inline TSize_T GetHashCode(const FXxHash128& Hash)
	{
		return static_cast<TSize_T>(Hash.HashLow64);
	}

	template <typename ReturnType, typename ... Arguments>
	inline TSize_T GetHashCode(const TDelegate<ReturnType(Arguments...)>& Delegate)
	{
		return std::hash<TDelegate<ReturnType(Arguments...)>>{}(Delegate);
	}

	template<typename T>
	inline typename TEnableIf<
		!TAreTheSameType<TChar*, typename TRemoveCV<T>::Type>::Value && !TAreTheSameType<TWChar*, typename TRemoveCV<T>::Type>::Value &&
		(TIsEnum<T>::Value || TIsPointer<T>::Value || TIsBaseOf<THashable, T>::Value), TSize_T
	>::Type GetHashCode(const T& Value)
	{
		if constexpr (TIsEnum<T>::Value)
		{
			return GetHashCode((__underlying_type(T))Value);
		}
		else if constexpr (TIsPointer<T>::Value)
		{
			static const TSize_T Shift = (TSize_T)log2(1 + sizeof(T));
			return (TSize_T)(Value) >> Shift;
		}
		else if constexpr (TIsBaseOf<THashable, T>::Value)
		{
			return Value.CalculateHash();
		}
		else
		{
			TRNT_ASSERT_AT_COMPILE_TIME_MESSAGE(false, "GetHashCode(const T& Value) doesn't support for this type.");
		}
	}
}
