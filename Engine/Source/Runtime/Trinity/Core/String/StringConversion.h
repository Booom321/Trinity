#pragma once

#include "String.h"
#include "Trinity/Core/Defines.h"
#include "Trinity/Core/Types/Version.h"

#pragma warning(push)
#pragma warning(disable : 4996)

namespace TNsStringConversion
{
	TRNT_API TBool ConvertWStringToAnsiString(const TWString& Source, TString& Dest);
	TRNT_API TBool ConvertWStringToAnsiString(const TWChar* Source, TString& Dest);
	TRNT_API TBool ConvertAnsiStringToWString(const TString& Source, TWString& Dest);
	TRNT_API TBool ConvertAnsiStringToWString(const TChar* Source, TWString& Dest);
} // namespace TNsStringConversion

#pragma warning(pop)
