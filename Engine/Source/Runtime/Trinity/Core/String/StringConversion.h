#pragma once

#include "Trinity/Core/Defines.h"

#include "String.h"

namespace TNsStringConversion
{
	TRNT_API void ToString(TInt8 Value, TString& OutString);

	TRNT_API void ToString(TInt16 Value, TString& OutString);

	TRNT_API void ToString(TInt32 Value, TString& OutString);

	TRNT_API void ToString(TInt64 Value, TString& OutString);

	TRNT_API void ToString(TUInt8 Value, TString& OutString);

	TRNT_API void ToString(TUInt16 Value, TString& OutString);

	TRNT_API void ToString(TUInt32 Value, TString& OutString);

	TRNT_API void ToString(TUInt64 Value, TString& OutString);

	TRNT_API void ToString(TDouble Value, TString& OutString);

	TRNT_API void ToString(TFloat Value, TString& OutString);

	TRNT_API void ToString(TBool Value, TString& OutString);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	TRNT_API void SetPrecision(TUInt32 Number);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	TRNT_API TInt8 StringToInt8(const TString& String);

	TRNT_API TInt16 StringToInt16(const TString& String);

	TRNT_API TInt32 StringToInt32(const TString& String);

	TRNT_API TInt64 StringToInt64(const TString& String);

	TRNT_API TUInt8 StringToUInt8(const TString& String);

	TRNT_API TUInt16 StringToUInt16(const TString& String);

	TRNT_API TUInt32 StringToUInt32(const TString& String);

	TRNT_API TUInt64 StringToUInt64(const TString& String);

	TRNT_API TFloat StringToFloat(const TString& String);

	TRNT_API TDouble StringToDouble(const TString& String);

	TRNT_API TBool StringToBool(const TString& String);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	TRNT_API TBool ConvertWStringToAnsiString(const TWString& Source, TString& Dest);

	TRNT_API TBool ConvertWStringToAnsiString(const TWChar* Source, TString& Dest);

	TRNT_API TBool ConvertAnsiStringToWString(const TString& Source, TWString& Dest);

	TRNT_API TBool ConvertAnsiStringToWString(const TChar* Source, TWString& Dest);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
class TStringable
{
public:
	static TRNT_CONSTEXPR TBool Value = TAreTheSameType<void, decltype(TNsStringConversion::ToString(std::declval<T>(), std::declval<TString&>()))>::Value;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////