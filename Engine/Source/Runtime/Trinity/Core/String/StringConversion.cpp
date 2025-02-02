#include "StringConversion.h"
#include "TrinityPCH.h"

namespace TNsStringConversion
{
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
} // namespace TNsStringConversion