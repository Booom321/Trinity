#pragma once

#include "CharBuffer.h"
#include "String.h"
#include "StringView.h"

namespace TNsStringFormatter
{
	template<typename CharT, typename T>
	class TFormatSpecifier
	{
	public:
		static TRNT_CONSTEXPR TBool IsDynamic = false;
		static TRNT_CONSTEXPR TInt64 NumChars = 256; // this will be ignored if IsDynamic is true

		using CharType = CharT;
		using ElementType = T;
		using CharBufferType = typename TTypeChooser<IsDynamic, TStringBase<CharType>, TCharBuffer<CharType, NumChars>>::Type;
	};

	template<typename CharT, typename T>
	class TFormatSpecifier<CharT, const T> : public TFormatSpecifier<CharT, T>
	{};

	template<typename CharT, typename T>
	class TFormatSpecifier<CharT, volatile T> : public TFormatSpecifier<CharT, T>
	{};

	template<typename CharT, typename T>
	class TFormatSpecifier<CharT, const volatile T> : public TFormatSpecifier<CharT, T>
	{};

	template<typename CharT, typename T>
	class TFormatSpecifier<CharT, T&> : public TFormatSpecifier<CharT, T>
	{};

	template<typename CharT, typename T>
	class TFormatSpecifier<CharT, const T&> : public TFormatSpecifier<CharT, T>
	{};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined(TRNT_DECL_FORMAT_SPECIFIER)
	#define TRNT_DECL_FORMAT_SPECIFIER(IsDynamic_, NumChars_, CharType_, ...)                                                      \
		template<>                                                                                                                 \
		class TFormatSpecifier<CharType_, __VA_ARGS__>                                                                             \
		{                                                                                                                          \
		public:                                                                                                                    \
			static TRNT_CONSTEXPR TBool IsDynamic = IsDynamic_;                                                                    \
			static TRNT_CONSTEXPR TInt64 NumChars = NumChars_;                                                                     \
			using CharType = CharType_;                                                                                            \
			using ElementType = __VA_ARGS__;                                                                                       \
			using CharBufferType = typename TTypeChooser<IsDynamic, TStringBase<CharType>, TCharBuffer<CharType, NumChars>>::Type; \
		}
#endif
	// Signed / Unsigned int types
	TRNT_DECL_FORMAT_SPECIFIER(false, 5, TChar, TInt8);		// -128 -> 127
	TRNT_DECL_FORMAT_SPECIFIER(false, 7, TChar, TInt16);	// -32768 -> 32767
	TRNT_DECL_FORMAT_SPECIFIER(false, 12, TChar, TInt32);	// -2147483648 -> 2147483647
	TRNT_DECL_FORMAT_SPECIFIER(false, 21, TChar, TInt64);	// (-9223372036854775807 - 1) -> 9223372036854775807
															//
	TRNT_DECL_FORMAT_SPECIFIER(false, 4, TChar, TUInt8);	// 0 -> 255
	TRNT_DECL_FORMAT_SPECIFIER(false, 6, TChar, TUInt16);	// 0 -> 65535
	TRNT_DECL_FORMAT_SPECIFIER(false, 11, TChar, TUInt32);	// 0 -> 4294967295
	TRNT_DECL_FORMAT_SPECIFIER(false, 21, TChar, TUInt64);	// 0 -> 18446744073709551615

	TRNT_DECL_FORMAT_SPECIFIER(false, 5, TWChar, TInt8);	// -128 -> 127
	TRNT_DECL_FORMAT_SPECIFIER(false, 7, TWChar, TInt16);	// -32768 -> 32767
	TRNT_DECL_FORMAT_SPECIFIER(false, 12, TWChar, TInt32);	// -2147483648 -> 2147483647
	TRNT_DECL_FORMAT_SPECIFIER(false, 21, TWChar, TInt64);	// (-9223372036854775807 - 1) -> 9223372036854775807

	TRNT_DECL_FORMAT_SPECIFIER(false, 4, TWChar, TUInt8);	// 0 -> 255
	TRNT_DECL_FORMAT_SPECIFIER(false, 6, TWChar, TUInt16);	// 0 -> 65535
	TRNT_DECL_FORMAT_SPECIFIER(false, 11, TWChar, TUInt32); // 0 -> 4294967295
	TRNT_DECL_FORMAT_SPECIFIER(false, 21, TWChar, TUInt64); // 0 -> 18446744073709551615

	// Floating point types
	TRNT_DECL_FORMAT_SPECIFIER(false, 64, TChar, TFloat);
	TRNT_DECL_FORMAT_SPECIFIER(false, 64, TChar, TDouble);
	TRNT_DECL_FORMAT_SPECIFIER(false, 86, TChar, TLongDouble);
	TRNT_DECL_FORMAT_SPECIFIER(false, 64, TWChar, TFloat);
	TRNT_DECL_FORMAT_SPECIFIER(false, 64, TWChar, TDouble);
	TRNT_DECL_FORMAT_SPECIFIER(false, 64, TWChar, TLongDouble);

	// Boolean types
	TRNT_DECL_FORMAT_SPECIFIER(false, 2, TChar, TBool);	 // true = 1 and false = 0
	TRNT_DECL_FORMAT_SPECIFIER(false, 2, TWChar, TBool); // true = 1 and false = 0

	// Char types
	TRNT_DECL_FORMAT_SPECIFIER(false, 2, TChar, TChar);
	TRNT_DECL_FORMAT_SPECIFIER(false, 2, TWChar, TWChar);

	// Char pointer types
	TRNT_DECL_FORMAT_SPECIFIER(false, 1, TChar, TChar*);
	TRNT_DECL_FORMAT_SPECIFIER(false, 1, TChar, const TChar*);

	TRNT_DECL_FORMAT_SPECIFIER(false, 1, TWChar, TWChar*);
	TRNT_DECL_FORMAT_SPECIFIER(false, 1, TWChar, const TWChar*);

	// String types
	TRNT_DECL_FORMAT_SPECIFIER(false, 1, TChar, TString);
	TRNT_DECL_FORMAT_SPECIFIER(false, 1, TWChar, TWString);

	// String view types
	TRNT_DECL_FORMAT_SPECIFIER(false, 1, TChar, TStringView);
	TRNT_DECL_FORMAT_SPECIFIER(false, 1, TWChar, TWStringView);

	// Bounded char array types
	template<typename CharT, typename T, TSize_T Num>
	class TFormatSpecifier<CharT, T[Num]>
	{
	public:
		using ElementType = typename TRemoveCVRef<T>::Type;
		static_assert(TAreTheSameType<ElementType, CharT>::Value && !TIsArray<ElementType>::Value);

		static TRNT_CONSTEXPR TBool IsDynamic = false;
		static TRNT_CONSTEXPR TInt64 NumChars = Num;

		using CharType = CharT;
		using CharBufferType = typename TTypeChooser<IsDynamic, TStringBase<CharType>, TCharBuffer<CharType, NumChars>>::Type;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<typename CharType>
	class TFormatterDetails;

	template<>
	class TFormatterDetails<TChar>
	{
	public:
		static TRNT_CONSTEXPR const TChar* DigitToChar = "9876543210123456789";
		static TRNT_CONSTEXPR const TChar* FloatingPointFormatStr = "%lf";
		static TRNT_CONSTEXPR const TChar* TrueStr = "true";
		static TRNT_CONSTEXPR const TChar* FalseStr = "false";
		static TRNT_CONSTEXPR const TChar Dot = '.';
		static TRNT_CONSTEXPR const TChar Bracket = '{';
		static TRNT_CONSTEXPR const TChar CloseBracket = '}';
	};

	template<>
	class TFormatterDetails<TWChar>
	{
	public:
		static TRNT_CONSTEXPR const TWChar* DigitToChar = L"9876543210123456789";
		static TRNT_CONSTEXPR const TWChar* FloatingPointFormatStr = L"%lf";
		static TRNT_CONSTEXPR const TWChar* TrueStr = L"true";
		static TRNT_CONSTEXPR const TWChar* FalseStr = L"false";
		static TRNT_CONSTEXPR const TWChar Dot = L'.';
		static TRNT_CONSTEXPR const TWChar Bracket = L'{';
		static TRNT_CONSTEXPR const TWChar CloseBracket = L'}';
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<typename CharType, typename IntegralType>
	CharType* IntegerToCharBuffer(IntegralType Number, CharType* EndBufferPtr, TInt64& Length)
	{
		static_assert(TIsIntegral<IntegralType>::Value);

		const CharType* EndPtr = EndBufferPtr;
		const IntegralType Temp = Number;

		do
		{
			*--EndBufferPtr = TFormatterDetails<CharType>::DigitToChar[9 + (Number % 10)];
			Number /= 10;
		} while (Number != 0);

		if constexpr (std::is_signed<IntegralType>::value)
		{
			if (Temp < 0)
			{
				*--EndBufferPtr = '-';
			}
		}

		Length = static_cast<TInt64>(EndPtr - EndBufferPtr);
		return EndBufferPtr;
	}

	template<typename CharType, typename FloatingType>
	TInt64 FloatingPointToCharBuffer(FloatingType Value, typename TFormatSpecifier<CharType, FloatingType>::CharBufferType& Buffer)
	{
		static_assert(TIsFloatingPoint<FloatingType>::Value);

		using SizeType = typename TStringBase<CharType>::SizeType;

		SizeType Length = static_cast<SizeType>(::snprintf(nullptr, 0, TFormatterDetails<CharType>::FloatingPointFormatStr, static_cast<TLongDouble>(Value)));
		Length = static_cast<SizeType>(::snprintf(Buffer.GetData(), (TRNT_MIN(Length, (TFormatSpecifier<CharType, FloatingType>::NumChars))) + 1, TFormatterDetails<CharType>::FloatingPointFormatStr, static_cast<TLongDouble>(Value)));

		return Length;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined(TRNT_DECLARE_TO_STRING_FUNCTION_FOR_INT_TYPE)
	#define TRNT_DECLARE_TO_STRING_FUNCTION_FOR_INT_TYPE(IntType)                                                                \
		template<typename CharType>                                                                                              \
		TStringViewBase<CharType> ToString(IntType Value, typename TFormatSpecifier<CharType, IntType>::CharBufferType&& Buffer) \
		{                                                                                                                        \
			using CharBufferType = typename TFormatSpecifier<CharType, IntType>::CharBufferType;                                 \
			using SizeType = typename CharBufferType::SizeType;                                                                  \
			static TRNT_CONSTEXPR SizeType CharBufferSize = TFormatSpecifier<CharType, IntType>::NumChars;                       \
			SizeType Length;                                                                                                     \
			CharType* BufferPtr = Buffer.GetData();                                                                              \
			CharType* Ptr = IntegerToCharBuffer<CharType, IntType>(Value, BufferPtr + CharBufferSize, Length);                   \
			return TStringViewBase<CharType>{ Ptr, Length };                                                                     \
		}

#endif

	TRNT_DECLARE_TO_STRING_FUNCTION_FOR_INT_TYPE(TInt8);
	TRNT_DECLARE_TO_STRING_FUNCTION_FOR_INT_TYPE(TInt16);
	TRNT_DECLARE_TO_STRING_FUNCTION_FOR_INT_TYPE(TInt32);
	TRNT_DECLARE_TO_STRING_FUNCTION_FOR_INT_TYPE(TInt64);

	TRNT_DECLARE_TO_STRING_FUNCTION_FOR_INT_TYPE(TUInt8);
	TRNT_DECLARE_TO_STRING_FUNCTION_FOR_INT_TYPE(TUInt16);
	TRNT_DECLARE_TO_STRING_FUNCTION_FOR_INT_TYPE(TUInt32);
	TRNT_DECLARE_TO_STRING_FUNCTION_FOR_INT_TYPE(TUInt64);

	template<typename CharType>
	TRNT_FORCE_INLINE TStringViewBase<CharType> ToString(TLongDouble Value, typename TFormatSpecifier<CharType, TLongDouble>::CharBufferType&& Buffer)
	{
		typename TStringBase<CharType>::SizeType Length = FloatingPointToCharBuffer<CharType, TLongDouble>(Value, Buffer);
		return TStringViewBase<CharType>{ Buffer.GetData(), Length };
	}

	template<typename CharType>
	TRNT_FORCE_INLINE TStringViewBase<CharType> ToString(TDouble Value, typename TFormatSpecifier<CharType, TDouble>::CharBufferType&& Buffer)
	{
		typename TStringBase<CharType>::SizeType Length = FloatingPointToCharBuffer<CharType, TDouble>(Value, Buffer);
		return TStringViewBase<CharType>{ Buffer.GetData(), Length };
	}

	template<typename CharType>
	TRNT_FORCE_INLINE TStringViewBase<CharType> ToString(TFloat Value, typename TFormatSpecifier<CharType, TFloat>::CharBufferType&& Buffer)
	{
		typename TStringBase<CharType>::SizeType Length = FloatingPointToCharBuffer<CharType, TFloat>(Value, Buffer);
		return TStringViewBase<CharType>{ Buffer.GetData(), Length };
	}

	template<typename CharType>
	TRNT_FORCE_INLINE TStringViewBase<CharType> ToString(TBool Value, typename TFormatSpecifier<CharType, TBool>::CharBufferType&& Buffer)
	{
		return Value ? TStringViewBase<CharType>{ TFormatterDetails<CharType>::TrueStr, 4 } : TStringViewBase<CharType>{ TFormatterDetails<CharType>::FalseStr, 5 };
	}

	template<typename CharType>
	TRNT_FORCE_INLINE TStringViewBase<CharType> ToString(const CharType* Value, typename TFormatSpecifier<CharType, CharType*>::CharBufferType&& Buffer)
	{
		return TStringViewBase<CharType>{ Value };
	}

	template<typename CharType, TInt64 Num>
	TRNT_FORCE_INLINE TStringViewBase<CharType> ToString(const CharType (&Value)[Num], typename TFormatSpecifier<CharType, CharType[Num]>::CharBufferType&& Buffer)
	{
		return TStringViewBase<CharType>{ Value };
	}

	template<typename CharType>
	TRNT_FORCE_INLINE TStringViewBase<CharType> ToString(const TStringBase<CharType>& Value, typename TFormatSpecifier<CharType, TStringBase<CharType>>::CharBufferType&&)
	{
		return TStringViewBase<CharType>{ Value.GetData(), Value.GetElementCount() };
	}

	template<typename CharType>
	TRNT_FORCE_INLINE TStringViewBase<CharType> ToString(TStringViewBase<CharType> Value, typename TFormatSpecifier<CharType, TStringViewBase<CharType>>::CharBufferType&&)
	{
		return Value;
	}
} // namespace TNsStringFormatter

template<typename T>
class TStringFormatter
{
public:
	static_assert(TIsCharTypeSupported<T>::Value, "TStringFormatter<T> is not implemented for this char type.");

	using SizeType = typename TStringBase<T>::SizeType;
	using ElementType = typename TStringBase<T>::ElementType;

public:
	TRNT_FORCE_INLINE TStringFormatter() = default;
	TRNT_FORCE_INLINE TStringFormatter(const TStringFormatter&) = default;
	TRNT_FORCE_INLINE TStringFormatter(TStringFormatter&&) noexcept = default;
	TRNT_FORCE_INLINE TStringFormatter& operator=(const TStringFormatter&) = default;
	TRNT_FORCE_INLINE TStringFormatter& operator=(TStringFormatter&&) noexcept = default;

	TRNT_FORCE_INLINE TStringFormatter(const ElementType* FmtString)
		: FmtString(FmtString)
	{}

	~TStringFormatter() = default;

public:
	TRNT_FORCE_INLINE void SetFormat(const ElementType* FmtString)
	{
		this->FmtString = FmtString;
	}

	TRNT_FORCE_INLINE const ElementType* GetFormat() const
	{
		return FmtString;
	}

public:
	template<typename... FormatArgs>
	TRNT_NODISCARD TStringBase<T> Format(FormatArgs&&... Args)
	{
		using FormatterDetails = TNsStringFormatter::TFormatterDetails<ElementType>;
		static TRNT_CONSTEXPR TInt32 ArgCount = sizeof...(FormatArgs);

		TStringViewBase<ElementType> StrViews[ArgCount] = {
			TNsStringFormatter::ToString<ElementType>(
				Args,
				typename TNsStringFormatter::TFormatSpecifier<ElementType, FormatArgs>::CharBufferType{})...
		};

		const ElementType
			*CurrentPos = TCString<ElementType>::Strchr(FmtString, FormatterDetails::Bracket),
			*LastPos = FmtString;

		TStringBase<ElementType> Result{};
		if (CurrentPos != nullptr)
		{
			ElementType NextChar = *(CurrentPos + 1);
			if (NextChar == FormatterDetails::CloseBracket)
			{
				SizeType ArgIndex = 0;
				while (CurrentPos != nullptr)
				{
					TRNT_ASSERT_MESSAGE("Too many placeholders (\"{}\") in format string.", ArgIndex < ArgCount);
					TRNT_ASSERT_MESSAGE("Unmatched '}' in format string.", *(CurrentPos + 1) == FormatterDetails::CloseBracket);
					Result.Append(LastPos, CurrentPos - LastPos);
					Result.Append(StrViews[ArgIndex].GetData(), StrViews[ArgIndex].GetElementCount());

					LastPos = CurrentPos + 2;
					CurrentPos = TCString<ElementType>::Strchr(LastPos, FormatterDetails::Bracket);
					++ArgIndex;
				}
			}
			else
			{
				if (NextChar >= 48 && NextChar <= 57)
				{
					const ElementType* CurrentCloseBracketPos = nullptr;
					TInt32 ArgIndex = 0;
					while (CurrentPos != nullptr)
					{
						CurrentCloseBracketPos = StringToInteger(ArgIndex, CurrentPos + 1);
						TRNT_ASSERT_MESSAGE("Argument index out of range.", ArgIndex < ArgCount);
						TRNT_ASSERT_MESSAGE("Unmatched '}' in format string.", *CurrentCloseBracketPos == FormatterDetails::CloseBracket);

						Result.Append(LastPos, CurrentPos - LastPos);
						Result.Append(StrViews[ArgIndex].GetData(), StrViews[ArgIndex].GetElementCount());

						LastPos = CurrentCloseBracketPos + 1;
						CurrentPos = TCString<ElementType>::Strchr(LastPos, FormatterDetails::Bracket);
					}
				}
			}
		}

		return Result;
	}

private:
	const ElementType* StringToInteger(TInt32& Value, const ElementType* CharPtr)
	{
		Value = 0;
		for (; (*CharPtr >= 48 && *CharPtr <= 57); ++CharPtr)
		{
			// Value = (Value << 1) + (Value << 3) + *CharPtr - 48;
			Value = Value * 10 + *CharPtr - 48;
		}

		return CharPtr;
	}

	const ElementType* FmtString;
};