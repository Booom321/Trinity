#pragma once

#include "Trinity/Core/Defines.h"

#include "String.h"
#include "Trinity/Core/Types/Version.h"

#pragma warning(push)
#pragma warning(disable: 4996)

namespace TNsStringConversion
{
	static TRNT_CONSTEXPR TInt32 MaxBufferSize = 23;

	namespace TNsPrivate
	{
		template<typename CharType>
		class TStringConverterDetails;

		template<>
		class TStringConverterDetails<TChar>
		{
		public:
			static TRNT_CONSTEXPR const TChar* DigitToChar = "9876543210123456789";
			static TRNT_CONSTEXPR const TChar* FloatingPointFormatStr = "%lf";
		};

		template<>
		class TStringConverterDetails<TWChar>
		{
		public:
			static TRNT_CONSTEXPR const TWChar* DigitToChar = L"9876543210123456789";
			static TRNT_CONSTEXPR const TWChar* FloatingPointFormatStr = L"%lf";
		};
	}

	template<typename IntegralType, typename CharType> CharType* IntegerToString(IntegralType IntValue, CharType* EndCharPtr, TInt64& Length)
	{
		static_assert(TIsIntegral<IntegralType>::Value);

		const CharType* EndPtr = EndCharPtr;
		const IntegralType Temp = IntValue;

		do
		{
			*--EndCharPtr = TNsPrivate::TStringConverterDetails<CharType>::DigitToChar[9 + (IntValue % 10)];
			IntValue /= 10;
		} while (IntValue != 0);

		if constexpr (std::is_signed<IntegralType>::value)
		{
			if (Temp < 0)
			{
				*--EndCharPtr = '-';
			}
		}

		Length = static_cast<TInt64>(EndPtr - EndCharPtr);

		return EndCharPtr;
	}

	template<typename IntegralType, typename CharType> IntegralType StringToInteger(const CharType* CharPtr)
	{
		static_assert(TIsIntegral<IntegralType>::Value);

		TBool IsNegative = false;

		if constexpr (TAreTheSameType<CharType, TChar>::Value)
		{
			while (::isspace(*CharPtr))
			{
				++CharPtr;
			}
		}
		else if constexpr (TAreTheSameType<CharType, TWChar>::Value)
		{
			while (::iswspace(*CharPtr))
			{
				++CharPtr;
			}
		}

		if (*CharPtr == 43) // +
		{
			++CharPtr;
		}
		else if (*CharPtr == 45) // -
		{
			++CharPtr;
			IsNegative = true;
		}

		IntegralType Result = 0;

		for (; (*CharPtr >= 48 && *CharPtr <= 58); ++CharPtr)
		{
			Result = (Result << 1) + (Result << 3) + *CharPtr - 48;
		}

		return (IsNegative) ? (0 - Result) : Result;
	}

	template<typename FPType, typename CharType> void FloatingPointToString(FPType Value, TStringBase<CharType>& OutString)
	{
		static_assert(TIsFloatingPoint<FPType>::Value);

		if constexpr (TAreTheSameType<TChar, CharType>::Value)
		{
			TInt64 Length = static_cast<TInt64>(snprintf(nullptr, 0, TNsPrivate::TStringConverterDetails<CharType>::FloatingPointFormatStr, static_cast<long double>(Value)));
			OutString.Resize(Length);
			snprintf(OutString.GetData(), Length + 1, TNsPrivate::TStringConverterDetails<CharType>::FloatingPointFormatStr, static_cast<long double>(Value));
		}
		else if constexpr (TAreTheSameType<TWChar, CharType>::Value)
		{
			TInt64 Length = static_cast<TInt64>(swprintf(nullptr, 0, TNsPrivate::TStringConverterDetails<CharType>::FloatingPointFormatStr, static_cast<long double>(Value)));
			OutString.Resize(Length);
			swprintf(OutString.GetData(), Length + 1, TNsPrivate::TStringConverterDetails<CharType>::FloatingPointFormatStr, static_cast<long double>(Value));
		}
	}

	template<typename CharType> void ToString(TInt8 Value, TStringBase<CharType>& OutString)
	{
		CharType Buffer[MaxBufferSize];

		TInt64 Length;
		const CharType* Ptr = IntegerToString<TInt8>(Value, Buffer + MaxBufferSize, Length);

		OutString.Clear();
		OutString.Append(Ptr, Length);
	}

	template<typename CharType> void ToString(TInt16 Value, TStringBase<CharType>& OutString)
	{
		CharType Buffer[MaxBufferSize];

		TInt64 Length;
		const CharType* Ptr = IntegerToString<TInt16>(Value, Buffer + MaxBufferSize, Length);

		OutString.Clear();
		OutString.Append(Ptr, Length);
	}

	template<typename CharType> void ToString(TInt32 Value, TStringBase<CharType>& OutString)
	{
		CharType Buffer[MaxBufferSize];

		TInt64 Length;
		const CharType* Ptr = IntegerToString<TInt32>(Value, Buffer + MaxBufferSize, Length);

		OutString.Clear();
		OutString.Append(Ptr, Length);
	}

	template<typename CharType> void ToString(TInt64 Value, TStringBase<CharType>& OutString)
	{
		CharType Buffer[MaxBufferSize];

		TInt64 Length;
		const CharType* Ptr = IntegerToString<TInt64>(Value, Buffer + MaxBufferSize, Length);

		OutString.Clear();
		OutString.Append(Ptr, Length);
	}

	template<typename CharType> void ToString(TUInt8 Value, TStringBase<CharType>& OutString)
	{
		CharType Buffer[MaxBufferSize];

		TInt64 Length;
		const CharType* Ptr = IntegerToString<TUInt8>(Value, Buffer + MaxBufferSize, Length);

		OutString.Clear();
		OutString.Append(Ptr, Length);
	}

	template<typename CharType> void ToString(TUInt16 Value, TStringBase<CharType>& OutString)
	{
		CharType Buffer[MaxBufferSize];

		TInt64 Length;
		const CharType* Ptr = IntegerToString<TUInt16>(Value, Buffer + MaxBufferSize, Length);

		OutString.Clear();
		OutString.Append(Ptr, Length);
	}

	template<typename CharType> void ToString(TUInt32 Value, TStringBase<CharType>& OutString)
	{
		CharType Buffer[MaxBufferSize];

		TInt64 Length;
		const CharType* Ptr = IntegerToString<TUInt32>(Value, Buffer + MaxBufferSize, Length);

		OutString.Clear();
		OutString.Append(Ptr, Length);
	}

	template<typename CharType> void ToString(TUInt64 Value, TStringBase<CharType>& OutString)
	{
		CharType Buffer[MaxBufferSize];

		TInt64 Length;
		const CharType* Ptr = IntegerToString<TUInt64>(Value, Buffer + MaxBufferSize, Length);

		OutString.Clear();
		OutString.Append(Ptr, Length);
	}

	template<typename CharType> TRNT_FORCE_INLINE void ToString(long double Value, TStringBase<CharType>& OutString)
	{
		FloatingPointToString<long double>(Value, OutString);
	}

	template<typename CharType> TRNT_FORCE_INLINE void ToString(TDouble Value, TStringBase<CharType>& OutString)
	{
		FloatingPointToString<TDouble>(Value, OutString);
	}

	template<typename CharType> TRNT_FORCE_INLINE void ToString(TFloat Value, TStringBase<CharType>& OutString)
	{
		FloatingPointToString<TFloat>(Value, OutString);
	}

	template<typename CharType> TRNT_FORCE_INLINE void ToString(TBool Value, TStringBase<CharType>& OutString)
	{
		OutString.Clear();
		OutString.Append(Value ? "true" : "false");
	}

	template<typename CharType> TRNT_FORCE_INLINE void ToString(const CharType* Value, TStringBase<CharType>& OutString)
	{
		OutString.Clear();
		OutString.Append(Value);
	}

	template<typename CharType> TRNT_FORCE_INLINE void ToString(const TStringBase<CharType>& Value, TStringBase<CharType>& OutString)
	{
		OutString.Clear();
		OutString.Append(Value);
	}

	template<typename CharType> void ToString(TVersion Value, TStringBase<CharType>& OutString)
	{
		static_assert(TAreTheSameType<TUInt8, decltype(Value.Major)>::Value);
		static_assert(TAreTheSameType<TUInt8, decltype(Value.Minor)>::Value);
		static_assert(TAreTheSameType<TUInt8, decltype(Value.Patch)>::Value);

		static TRNT_CONSTEXPR TUInt32 MaxBufferSizeForUInt8 = 3;

		OutString.Clear();

		CharType DotChr;

		if constexpr (TAreTheSameType<CharType, TChar>::Value)
		{
			DotChr = '.';
		}
		else if constexpr (TAreTheSameType<CharType, TWChar>::Value)
		{
			DotChr = L'.';
		}

		CharType Buffer[MaxBufferSizeForUInt8];

		TInt64 Length;
		const CharType* Ptr = IntegerToString<TUInt8>(Value.Major, Buffer + MaxBufferSizeForUInt8, Length);
		OutString.Append(Ptr, Length);
		OutString += DotChr; 

		Ptr = IntegerToString<TUInt8>(Value.Minor, Buffer + MaxBufferSizeForUInt8, Length);
		OutString.Append(Ptr, Length);
		OutString += DotChr; 

		Ptr = IntegerToString<TUInt8>(Value.Patch, Buffer + MaxBufferSizeForUInt8, Length);
		OutString.Append(Ptr, Length);
	}

	template<typename CharType> TRNT_FORCE_INLINE void ConvertTo(TInt8& Output, const TStringBase<CharType>& String)
	{
		Output = StringToInteger<TInt8>(String.GetData());
	}

	template<typename CharType> TRNT_FORCE_INLINE void ConvertTo(TInt16& Output, const TStringBase<CharType>& String)
	{
		Output = StringToInteger<TInt16>(String.GetData());
	}

	template<typename CharType> TRNT_FORCE_INLINE void ConvertTo(TInt32& Output, const TStringBase<CharType>& String)
	{
		Output = StringToInteger<TInt32>(String.GetData());
	}

	template<typename CharType> TRNT_FORCE_INLINE void ConvertTo(TInt64& Output, const TStringBase<CharType>& String)
	{
		Output = StringToInteger<TInt64>(String.GetData());
	}

	template<typename CharType> TRNT_FORCE_INLINE void ConvertTo(TUInt8& Output, const TStringBase<CharType>& String)
	{
		Output = StringToInteger<TUInt8>(String.GetData());
	}

	template<typename CharType> TRNT_FORCE_INLINE void ConvertTo(TUInt16& Output, const TStringBase<CharType>& String)
	{
		Output = StringToInteger<TUInt16>(String.GetData());
	}

	template<typename CharType> TRNT_FORCE_INLINE void ConvertTo(TUInt32& Output, const TStringBase<CharType>& String)
	{
		Output = StringToInteger<TUInt32>(String.GetData());
	}

	template<typename CharType> TRNT_FORCE_INLINE void ConvertTo(TUInt64& Output, const TStringBase<CharType>& String)
	{
		Output = StringToInteger<TUInt64>(String.GetData());
	}

	template<typename CharType> TRNT_FORCE_INLINE void ConvertTo(long double& Output, const TStringBase<CharType>& String)
	{
		Output = std::strtold(String.GetData(), nullptr);
	}

	template<typename CharType> TRNT_FORCE_INLINE void ConvertTo(TDouble& Output, const TStringBase<CharType>& String)
	{
		Output = std::strtod(String.GetData(), nullptr);
	}

	template<typename CharType> TRNT_FORCE_INLINE void ConvertTo(TFloat& Output, const TStringBase<CharType>& String)
	{
		Output = std::strtof(String.GetData(), nullptr);
	}

	template<typename CharType> void ConvertTo(TBool& Output, const TStringBase<CharType>& String)
	{
		if (String.IsEquals("1") || String.IsEquals("true", TStringSearchCase::EIgnoreCase))
		{
			return true;
		}

		if (String.IsEquals("0") || String.IsEquals("false", TStringSearchCase::EIgnoreCase))
		{
			return false;
		}

		return false;
	}

	template<typename CharType> TRNT_FORCE_INLINE void ConvertTo(const CharType*& Output, const TStringBase<CharType>& String)
	{
		memcpy(Output, String.GetData(), String.GetElementCount() * sizeof(CharType));
	}

	template<typename CharType> TRNT_FORCE_INLINE void ConvertTo(TStringBase<CharType>& Output, const TStringBase<CharType>& String)
	{
		Output = String;
	}

	TRNT_API TBool ConvertWStringToAnsiString(const TWString& Source, TString& Dest);
	TRNT_API TBool ConvertWStringToAnsiString(const TWChar* Source, TString& Dest);
	TRNT_API TBool ConvertAnsiStringToWString(const TString& Source, TWString& Dest);
	TRNT_API TBool ConvertAnsiStringToWString(const TChar* Source, TWString& Dest);
}


template<typename T, typename CharType>
class TIsStringable
{
public:
	static TRNT_CONSTEXPR TBool Value = TAreTheSameType<
		void, decltype(TNsStringConversion::ToString<CharType>(std::declval<T>(), std::declval<TStringBase<CharType>&>()))>::Value;
};


template<typename DestType, typename CharType>
class TCanConvertToType
{
public:
	static TRNT_CONSTEXPR TBool Value = TAreTheSameType<
		void, decltype(TNsStringConversion::ConvertTo<CharType>(std::declval<DestType&>(), std::declval<const TStringBase<CharType>&>()))>::Value;
};

#pragma warning(pop)
