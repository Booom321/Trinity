#pragma once

#include "CharBuffer.h"
#include "String.h"
#include "StringView.h"

// TStringParser helper functions
namespace TNsStringParser
{
	template<typename CharType>
	class TParserDetails;

	template<>
	class TParserDetails<TChar>
	{
	public:
		static TRNT_CONSTEXPR TChar DoubleQuoteChar = '"';
		static TRNT_CONSTEXPR TChar BracketChar = '{';
		static TRNT_CONSTEXPR TChar CloseBracketChar = '}';
		static TRNT_CONSTEXPR const TChar* TerminatingChars = ",)]} \r\n\t";
		static TRNT_CONSTEXPR const TChar* TrueStr = "true";
		static TRNT_CONSTEXPR const TChar* TrueAsNum = "1";
		static TRNT_CONSTEXPR const TChar* FalseStr = "false";
		static TRNT_CONSTEXPR const TChar* FalseAsNum = "0";
	};

	template<>
	class TParserDetails<TWChar>
	{
	public:
		static TRNT_CONSTEXPR TWChar DoubleQuoteChar = L'"';
		static TRNT_CONSTEXPR TWChar BracketChar = L'{';
		static TRNT_CONSTEXPR TWChar CloseBracketChar = L'}';
		static TRNT_CONSTEXPR const TWChar* TerminatingChars = L",)]} \r\n\t";
		static TRNT_CONSTEXPR const TWChar* TrueStr = L"true";
		static TRNT_CONSTEXPR const TWChar* TrueAsNum = L"1";
		static TRNT_CONSTEXPR const TWChar* FalseStr = L"false";
		static TRNT_CONSTEXPR const TWChar* FalseAsNum = L"0";
	};

	template<typename IntegralType, typename CharType>
	IntegralType StringToInteger(const CharType* CharPtr)
	{
		static_assert(TIsIntegral<IntegralType>::Value);

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

		TBool IsNegative = false;
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

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename CharType>
	class TSimpleCharArray
	{
	public:
		template<TInt64 Num>
		TRNT_FORCE_INLINE TSimpleCharArray(CharType (&Array)[Num])
			: Array(Array), Len(Num)
		{}

		CharType* Array;
		TInt64 Len;
	};

	class TArg
	{
	public:
		enum class TType : TInt8
		{
			EUnknown = -1,
			EBool,
			EChar,
			EWChar,

			EInt8,
			EInt16,
			EInt32,
			EInt64,

			EUInt8,
			EUInt16,
			EUInt32,
			EUInt64,

			ELongDouble,
			EFloat,
			EDouble,

			ECharPointer,
			EWCharPointer,
			ECharArray,
			EWCharArray,
		};

		union
		{
			std::reference_wrapper<TBool> BoolValue;
			std::reference_wrapper<TChar> CharValue;
			std::reference_wrapper<TWChar> WCharValue;

			std::reference_wrapper<TInt8> Int8Value;
			std::reference_wrapper<TInt16> Int16Value;
			std::reference_wrapper<TInt32> Int32Value;
			std::reference_wrapper<TInt64> Int64Value;

			std::reference_wrapper<TUInt8> UInt8Value;
			std::reference_wrapper<TUInt16> UInt16Value;
			std::reference_wrapper<TUInt32> UInt32Value;
			std::reference_wrapper<TUInt64> UInt64Value;

			std::reference_wrapper<TLongDouble> LongDoubleValue;
			std::reference_wrapper<TFloat> FloatValue;
			std::reference_wrapper<TDouble> DoubleValue;

			std::reference_wrapper<TChar*> CharPointerValue;
			std::reference_wrapper<TWChar*> WCharPointerValue;

			TSimpleCharArray<TChar> CharArrValue;
			TSimpleCharArray<TWChar> WCharArrValue;
		};

		TType Type;

	public:
		TArg(TBool& Value)
			: BoolValue(Value), Type(TType::EBool)
		{}

		TArg(TChar& Value)
			: CharValue(Value), Type(TType::EChar)
		{}

		TArg(TWChar& Value)
			: WCharValue(Value), Type(TType::EWChar)
		{}

		TArg(TInt8& Value)
			: Int8Value(Value), Type(TType::EInt8)
		{}

		TArg(TInt16& Value)
			: Int16Value(Value), Type(TType::EInt16)
		{}

		TArg(TInt32& Value)
			: Int32Value(Value), Type(TType::EInt32)
		{}

		TArg(TInt64& Value)
			: Int64Value(Value), Type(TType::EInt64)
		{}

		TArg(TUInt8& Value)
			: UInt8Value(Value), Type(TType::EUInt8)
		{}

		TArg(TUInt16& Value)
			: UInt16Value(Value), Type(TType::EUInt16)
		{}

		TArg(TUInt32& Value)
			: UInt32Value(Value), Type(TType::EUInt32)
		{}

		TArg(TUInt64& Value)
			: UInt64Value(Value), Type(TType::EUInt64)
		{}

		TArg(TLongDouble& Value)
			: LongDoubleValue(Value), Type(TType::ELongDouble)
		{}

		TArg(TFloat& Value)
			: FloatValue(Value), Type(TType::EFloat)
		{}

		TArg(TDouble& Value)
			: DoubleValue(Value), Type(TType::EDouble)
		{}

		TArg(TChar*& Value)
			: CharPointerValue(Value), Type(TType::ECharPointer)
		{}

		TArg(TWChar*& Value)
			: WCharPointerValue(Value), Type(TType::EWCharPointer)
		{}

		template<TInt64 Num>
		TArg(TChar (&Value)[Num])
			: CharArrValue(Value), Type(TType::ECharArray)
		{}

		template<TInt64 Num>
		TArg(TWChar (&Value)[Num])
			: CharArrValue(Value), Type(TType::EWCharArray)
		{}
	};
} // namespace TNsStringParser

template<typename T>
class TStringParser
{
public:
	static_assert(TIsCharTypeSupported<T>::Value, "TStringParser<T> is not implemented for this char type.");

	using ElementType = T;
	using SizeType = TInt64;
	using CString = TCString<ElementType>;
	using ParserDetails = TNsStringParser::TParserDetails<ElementType>;

	template<typename... ArgsType>
	static TBool Parse(const ElementType* MainString, const ElementType* KeyString, const ElementType* Pattern, ArgsType&... Args)
	{
		if (!MainString || !KeyString || !Pattern)
		{
			return false;
		}

		const ElementType* KeyStringPtr = CString::Strstr(MainString, KeyString);
		if (!KeyStringPtr)
		{
			return false;
		}

		const SizeType KeyStrLen = static_cast<SizeType>(CString::Strlen(KeyString));
		const ElementType* ValueBeginPtr = KeyStringPtr + KeyStrLen;
		const ElementType* ValueEndPtr = nullptr;

		if (*ValueBeginPtr == ParserDetails::DoubleQuoteChar)
		{
			++ValueBeginPtr;
			ValueEndPtr = CString::Strchr(ValueBeginPtr, ParserDetails::DoubleQuoteChar);

			if (!ValueEndPtr)
			{
				ValueEndPtr = ValueBeginPtr + CString::Strlen(ValueBeginPtr);
			}
			else
			{
				--ValueEndPtr;
			}
		}
		else
		{
			ValueEndPtr = ValueBeginPtr + CString::Strcspn(ValueBeginPtr, ParserDetails::TerminatingChars);
		}

		return ExtractValuesByPattern(ValueBeginPtr, ValueEndPtr, Pattern, Args...);
	}

private:
	template<typename... ArgsType>
	static TBool ExtractValuesByPattern(const ElementType* ValueBeginPtr, const ElementType* ValueEndPtr, const ElementType* Pattern, ArgsType&... Args)
	{
		static TRNT_CONSTEXPR TInt32 ArgCount = sizeof...(ArgsType);

		TNsStringParser::TArg ArgArray[ArgCount] = { TNsStringParser::TArg(Args)... };
		TInt32 CurrentArgIndex = 0;

		const ElementType* PatternStartPos = nullptr;
		const ElementType* LastPos = ValueBeginPtr;
		const ElementType* CurrentPos = ValueBeginPtr;

		SizeType MaxLen = -1;
		ElementType* ValueAsStr = nullptr;

		while (ValueBeginPtr <= ValueEndPtr)
		{
			if (*Pattern == *ValueBeginPtr)
			{
				++Pattern, ++ValueBeginPtr;
			}
			else if (*Pattern == ParserDetails::BracketChar)
			{
				if (*(Pattern + 1) != ParserDetails::CloseBracketChar)
				{
					return false;
				}

				if (PatternStartPos != nullptr)
				{
					TRNT_ASSERT(CurrentArgIndex < ArgCount);
					const SizeType Len = CurrentPos - LastPos;
					if (Len > MaxLen)
					{
						MaxLen = Len;
						delete[] ValueAsStr;
						ValueAsStr = new ElementType[MaxLen + 1];
					}
					memcpy(ValueAsStr, LastPos, Len * sizeof(ElementType));
					ValueAsStr[Len] = (ElementType)0;

					GetValueFromString(ArgArray[CurrentArgIndex], ValueAsStr, Len);
					++CurrentArgIndex;
				}

				PatternStartPos = Pattern;
				CurrentPos = ValueBeginPtr;
				Pattern += 2;
				LastPos = CurrentPos;
			}
			else if (PatternStartPos != nullptr)
			{
				Pattern = PatternStartPos + 2;
				ValueBeginPtr = ++CurrentPos;
			}
			else
			{
				return false;
			}
		}
		if (PatternStartPos != nullptr)
		{
			const SizeType Len = CurrentPos - LastPos;
			if (Len > MaxLen)
			{
				MaxLen = Len;
				delete[] ValueAsStr;
				ValueAsStr = new ElementType[MaxLen + 1];
			}
			memcpy(ValueAsStr, LastPos, Len * sizeof(ElementType));
			ValueAsStr[Len] = (ElementType)0;

			GetValueFromString(ArgArray[CurrentArgIndex], ValueAsStr, Len);
		}
		delete[] ValueAsStr;
		ValueAsStr = nullptr;

		return true;
	}

	static void GetValueFromString(TNsStringParser::TArg& Arg, const ElementType* Str, const SizeType StrLen)
	{
		using ArgType = TNsStringParser::TArg::TType;
		switch (Arg.Type)
		{
			case ArgType::EBool:
				if (CString::Strcmp(Str, ParserDetails::TrueAsNum) == 0 || CString::Stricmp(Str, ParserDetails::TrueStr) == 0)
				{
					Arg.BoolValue.get() = true;
				}
				else if (CString::Strcmp(Str, ParserDetails::FalseAsNum) == 0 || CString::Stricmp(Str, ParserDetails::FalseStr) == 0)
				{
					Arg.BoolValue.get() = false;
				}
				break;
			case ArgType::EChar:
				Arg.CharValue.get() = *Str;
				break;
			case ArgType::EWChar:
				Arg.WCharValue.get() = *Str;
				break;
			case ArgType::EInt8:
				Arg.Int8Value.get() = TNsStringParser::StringToInteger<TInt8, ElementType>(Str);
				break;
			case ArgType::EInt16:
				Arg.Int16Value.get() = TNsStringParser::StringToInteger<TInt16, ElementType>(Str);
				break;
			case ArgType::EInt32:
				Arg.Int32Value.get() = TNsStringParser::StringToInteger<TInt32, ElementType>(Str);
				break;
			case ArgType::EInt64:
				Arg.Int64Value.get() = TNsStringParser::StringToInteger<TInt64, ElementType>(Str);
				break;
			case ArgType::EUInt8:
				Arg.UInt8Value.get() = TNsStringParser::StringToInteger<TUInt8, ElementType>(Str);
				break;
			case ArgType::EUInt16:
				Arg.UInt16Value.get() = TNsStringParser::StringToInteger<TUInt16, ElementType>(Str);
				break;
			case ArgType::EUInt32:
				Arg.UInt32Value.get() = TNsStringParser::StringToInteger<TUInt32, ElementType>(Str);
				break;
			case ArgType::EUInt64:
				Arg.UInt64Value.get() = TNsStringParser::StringToInteger<TUInt64, ElementType>(Str);
				break;
			case ArgType::ELongDouble:
				if constexpr (TAreTheSameType<ElementType, TChar>::Value)
				{
					Arg.LongDoubleValue.get() = std::strtold(Str, nullptr);
				}
				else if constexpr (TAreTheSameType<ElementType, TWChar>::Value)
				{
					Arg.LongDoubleValue.get() = std::wcstold(Str, nullptr);
				}
				break;
			case ArgType::EFloat:
				if constexpr (TAreTheSameType<ElementType, TChar>::Value)
				{
					Arg.FloatValue.get() = std::strtof(Str, nullptr);
				}
				else if constexpr (TAreTheSameType<ElementType, TWChar>::Value)
				{
					Arg.FloatValue.get() = std::wcstof(Str, nullptr);
				}
				break;
			case ArgType::EDouble:
				if constexpr (TAreTheSameType<ElementType, TChar>::Value)
				{
					Arg.DoubleValue.get() = std::strtod(Str, nullptr);
				}
				else if constexpr (TAreTheSameType<ElementType, TWChar>::Value)
				{
					Arg.DoubleValue.get() = std::wcstod(Str, nullptr);
				}
				break;
			case ArgType::ECharPointer:
				if constexpr (TAreTheSameType<ElementType, TChar>::Value)
				{
					memcpy(Arg.CharPointerValue.get(), Str, StrLen * sizeof(ElementType));
				}
				break;
			case ArgType::EWCharPointer:
				if constexpr (TAreTheSameType<ElementType, TWChar>::Value)
				{
					wmemcpy(Arg.WCharPointerValue.get(), Str, StrLen);
				}
				break;
			case ArgType::ECharArray:
				if constexpr (TAreTheSameType<ElementType, TChar>::Value)
				{
					memcpy(Arg.CharArrValue.Array, Str, TRNT_MIN(Arg.CharArrValue.Len, StrLen) * sizeof(ElementType));
				}
				break;
			case ArgType::EWCharArray:
				if constexpr (TAreTheSameType<ElementType, TWChar>::Value)
				{
					wmemcpy(Arg.WCharArrValue.Array, Str, TRNT_MIN(Arg.WCharArrValue.Len, StrLen));
				}
				break;
		}
	}
};
