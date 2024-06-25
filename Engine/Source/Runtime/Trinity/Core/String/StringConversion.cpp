#include "TrinityPCH.h"

#include "StringConversion.h"

#pragma warning(push)
#pragma warning(disable: 4996)

static constexpr TInt32 MaxBufferSize = 23;
static constexpr const TChar* DigitToChar = "9876543210123456789";

static constexpr TInt32 MaxDigit = 50;
static constexpr TInt32 MaxCharCount = 6;
static TChar FloatingPointFormatStr[MaxCharCount] = { '%', 'f' };

template<typename IntegerType>
static TChar* IntegerToString(IntegerType IntValue, TChar* CharPtr, TInt64& Length)
{
	static_assert(TIsIntegral<IntegerType>::Value);
	
	const TChar* EndPtr = CharPtr;
	const IntegerType Temp = IntValue;

	do
	{
		*--CharPtr = DigitToChar[9 + (IntValue % 10)];
		IntValue /= 10;
	} while (IntValue != 0);

	if constexpr (std::is_signed<IntegerType>::value)
	{
		if (Temp < 0)
		{
			*--CharPtr = '-';
		}
	}

	Length = static_cast<TInt64>(EndPtr - CharPtr);

	return CharPtr;
}

template<typename IntegerType>
static IntegerType StringToInteger(const TChar* CharPtr)
{
	static_assert(TIsIntegral<IntegerType>::Value);

	TBool IsNegative = false;

	while (::isspace(static_cast<unsigned char>(*CharPtr)))
	{
		++CharPtr;
	}

	if (*CharPtr == 43)
	{
		++CharPtr;
	}
	else if (*CharPtr == 45)
	{
		++CharPtr;
		IsNegative = true;
	}

	IntegerType Result = 0;

	for (; (*CharPtr >= 48 && *CharPtr <= 58); ++CharPtr)
	{
		Result = (Result << 1) + (Result << 3) + *CharPtr - 48;
	}

	return (IsNegative) ? (0 - Result) : Result;
}

namespace TNsStringConversion
{
	void ToString(TInt8 Value, TString& OutString)
	{
		TChar Buffer[MaxBufferSize];

		TInt64 Length;
		const TChar* Ptr = IntegerToString<TInt8>(Value, Buffer + MaxBufferSize, Length);

		OutString.Append(Ptr, Length);
	}

	void ToString(TInt16 Value, TString& OutString)
	{
		TChar Buffer[MaxBufferSize];

		TInt64 Length;
		const TChar* Ptr = IntegerToString<TInt16>(Value, Buffer + MaxBufferSize, Length);

		OutString.Append(Ptr, Length);
	}

	void ToString(TInt32 Value, TString& OutString)
	{
		TChar Buffer[MaxBufferSize];

		TInt64 Length;
		const TChar* Ptr = IntegerToString<TInt32>(Value, Buffer + MaxBufferSize, Length);

		OutString.Append(Ptr, Length);
	}

	void ToString(TInt64 Value, TString& OutString)
	{
		TChar Buffer[MaxBufferSize];

		TInt64 Length;
		const TChar* Ptr = IntegerToString<TInt64>(Value, Buffer + MaxBufferSize, Length);

		OutString.Append(Ptr, Length);
	}

	void ToString(TUInt8 Value, TString& OutString)
	{
		TChar Buffer[MaxBufferSize];

		TInt64 Length;
		const TChar* Ptr = IntegerToString<TUInt8>(Value, Buffer + MaxBufferSize, Length);

		OutString.Append(Ptr, Length);
	}

	void ToString(TUInt16 Value, TString& OutString)
	{
		TChar Buffer[MaxBufferSize];

		TInt64 Length;
		const TChar* Ptr = IntegerToString<TUInt16>(Value, Buffer + MaxBufferSize, Length);

		OutString.Append(Ptr, Length);
	}

	void ToString(TUInt32 Value, TString& OutString)
	{
		TChar Buffer[MaxBufferSize];

		TInt64 Length;
		const TChar* Ptr = IntegerToString<TUInt32>(Value, Buffer + MaxBufferSize, Length);

		OutString.Append(Ptr, Length);
	}

	void ToString(TUInt64 Value, TString& OutString)
	{
		TChar Buffer[MaxBufferSize];

		TInt64 Length;
		const TChar* Ptr = IntegerToString<TUInt64>(Value, Buffer + MaxBufferSize, Length);

		OutString.Append(Ptr, Length);
	}

	void ToString(TDouble Value, TString& OutString)
	{
		TInt64 Length = static_cast<TInt64>(snprintf(nullptr, 0, FloatingPointFormatStr, Value));

		TString String(Length, '\0');
		::snprintf(String.GetData(), Length + 1, FloatingPointFormatStr, Value);

		OutString.Append(String);
	}

	void ToString(TFloat Value, TString& OutString)
	{
		TInt64 Length = static_cast<TInt64>(snprintf(nullptr, 0, FloatingPointFormatStr, Value));

		TString String(Length, '\0');
		::snprintf(String.GetData(), Length + 1, FloatingPointFormatStr, Value);

		OutString.Append(String);
	}

	void ToString(TBool Value, TString& OutString)
	{
		OutString.Append(Value ? "true" : "false");
	}

	void SetPrecision(TUInt32 Number)
	{
		TRNT_ASSERT(Number >= 0 && Number <= MaxDigit);
		memset(FloatingPointFormatStr, '\0', MaxCharCount * sizeof(TChar));

		TString NewFormat("%.");
		ToString(Number, NewFormat);
		NewFormat += "f";

		memcpy(FloatingPointFormatStr, NewFormat.GetData(), NewFormat.GetElementCount() * sizeof(TChar));
	}

	TInt8 StringToInt8(const TString& String)
	{
		return StringToInteger<TInt8>(String.GetData());
	}

	TInt16 StringToInt16(const TString& String)
	{
		return StringToInteger<TInt16>(String.GetData());
	}

	TInt32 StringToInt32(const TString& String)
	{
		return StringToInteger<TInt32>(String.GetData());
	}

	TInt64 StringToInt64(const TString& String)
	{
		return StringToInteger<TInt64>(String.GetData());
	}

	TUInt8 StringToUInt8(const TString& String)
	{
		return StringToInteger<TUInt8>(String.GetData());
	}

	TUInt16 StringToUInt16(const TString& String)
	{
		return StringToInteger<TUInt16>(String.GetData());
	}

	TUInt32 StringToUInt32(const TString& String)
	{
		return StringToInteger<TUInt32>(String.GetData());
	}

	TUInt64 StringToUInt64(const TString& String)
	{
		return StringToInteger<TUInt64>(String.GetData());
	}

	TFloat StringToFloat(const TString& String)
	{
		return std::strtof(String.GetData(), nullptr);
	}

	TDouble StringToDouble(const TString& String)
	{
		return std::strtod(String.GetData(), nullptr);
	}

	TBool StringToBool(const TString& String)
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

	TBool ConvertWStringToAnsiString(const TWString& Source, TString& Dest)
	{
		const TInt64 SourceLen = Source.GetElementCount();
		Dest.Resize(SourceLen);
		TChar* DestData = Dest.GetData();
		*DestData = (TChar)0;
		TSize_T Result = ::wcstombs(DestData, Source.GetData(), SourceLen + 1);
		if (Result == (TSize_T)-1)
		{
			return false;
		}
		return true;
	}

	TBool ConvertWStringToAnsiString(const TWChar* Source, TString& Dest)
	{
		if (!Source)
		{
			return false;
		}

		const TInt64 SourceLen = ::wcslen(Source);
		Dest.Resize(SourceLen);
		TChar* DestData = Dest.GetData();
		*DestData = (TChar)0;
		TSize_T Result = ::wcstombs(DestData, Source, SourceLen + 1);
		if (Result == (TSize_T)-1)
		{
			return false;
		}
		return true;
	}

	TBool ConvertAnsiStringToWString(const TString& Source, TWString& Dest)
	{
		const TInt64 SourceLen = Source.GetElementCount();
		Dest.Resize(SourceLen);
		TWChar* DestData = Dest.GetData();
		*DestData = (TWChar)0;
		TSize_T Result = ::mbstowcs(DestData, Source.GetData(), SourceLen);

		if (Result == (TSize_T)-1)
		{
			return false;
		}

		return true;
	}

	TBool ConvertAnsiStringToWString(const TChar* Source, TWString& Dest)
	{
		if (!Source)
		{
			return false;
		}

		const TInt64 SourceLen = ::strlen(Source);
		Dest.Resize(SourceLen);
		TWChar* DestData = Dest.GetData();
		*DestData = (TWChar)0;
		TSize_T Result = ::mbstowcs(DestData, Source, SourceLen);

		if (Result == (TSize_T)-1)
		{
			return false;
		}

		return true;
	}
}

#pragma warning(pop)