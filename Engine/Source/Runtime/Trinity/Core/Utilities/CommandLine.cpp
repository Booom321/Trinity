#include "TrinityPCH.h"

#include "CommandLine.h"

TCommandLine TCommandLine::Instance = TCommandLine();

TCommandLine::TCommandLine()
	: ArgCount(0), CommandLineArgs()
{
}

TCommandLine::~TCommandLine()
{
}

void TCommandLine::SetCommandLine(TInt32 ArgCount, TChar** Argv)
{
	this->ArgCount = ArgCount;
	this->CommandLineArgs.Reserve(ArgCount);

	for (TInt32 Index = 0; Index < ArgCount; ++Index)
	{
		this->CommandLineArgs.EmplaceBack(Argv[Index]);
	}
}

void TCommandLine::Reset()
{
	ArgCount = 0;
	CommandLineArgs.Clear();
}

void TCommandLine::AddCommandLineArgument(const TChar* CmdLineArgument)
{
	CommandLineArgs.EmplaceBack(CmdLineArgument);
	++ArgCount;
}

TBool TCommandLine::CommandLineOptionExists(const TChar* OptionName, TStringSearchCase SearchChase) const
{
	TRNT_ASSERT_MESSAGE("Command line option name must start with a hyphen (-).", OptionName[0] == '-');

	TSize_T OptionNameLen = strlen(OptionName);

	for (TInt32 Index = 0; Index < ArgCount; ++Index)
	{
		switch (SearchChase)
		{
			case TStringSearchCase::EIgnoreCase:
				if (TCString<TChar>::Strnicmp(OptionName, CommandLineArgs[Index].GetData(), OptionNameLen) == 0)
				{
					return true;
				}
				break;
			case TStringSearchCase::ECaseSensitive:
				if (::strncmp(OptionName, CommandLineArgs[Index].GetData(), OptionNameLen) == 0)
				{
					return true;
				}
				break;
		}
	}

	return false;
}

TInt32 TCommandLine::GetCommandLineOptionIndex(const TChar* OptionName, TStringSearchCase SearchChase) const
{
	TRNT_ASSERT_MESSAGE("Command line option name must start with a hyphen (-).", OptionName[0] == '-');

	TSize_T OptionNameLen = strlen(OptionName);

	for (TInt32 Index = 0; Index < ArgCount; ++Index)
	{
		switch (SearchChase)
		{
			case TStringSearchCase::EIgnoreCase:
				if (TCString<TChar>::Strnicmp(OptionName, CommandLineArgs[Index].GetData(), OptionNameLen) == 0)
				{
					return Index;
				}
				break;
			case TStringSearchCase::ECaseSensitive:
				if (::strncmp(OptionName, CommandLineArgs[Index].GetData(), OptionNameLen) == 0)
				{
					return Index;
				}
				break;
		}
	}

	return (TInt32)TDynamicArray<TString>::Npos;
}

TString TCommandLine::GetCommandLineAsString() const
{
	TString Output;

	for (TInt32 Index = 0; Index < ArgCount; ++Index)
	{
		Output.Append(CommandLineArgs[Index]);
		Output.Append(1, ' ');
	}

	Output.TrimEndInternal();
	return Output;
}

namespace TNsPrivate
{
	static TInt32 GetCommandLineOptionIndex(
		const TDynamicArray<TString>& CommandLineArgs,
		TInt32 ArgCount,
		const TChar* OptionName,
		TSize_T OptionNameLen,
		TStringSearchCase SearchChase)
	{
		TRNT_ASSERT_MESSAGE("Command line option name must start with a hyphen (-).", OptionName[0] == '-');

		for (TInt32 Index = 0; Index < ArgCount; ++Index)
		{
			switch (SearchChase)
			{
				case TStringSearchCase::EIgnoreCase:
					if (TCString<TChar>::Strnicmp(OptionName, CommandLineArgs[Index].GetData(), OptionNameLen) == 0)
					{
						return Index;
					}
					break;
				case TStringSearchCase::ECaseSensitive:
					if (::strncmp(OptionName, CommandLineArgs[Index].GetData(), OptionNameLen) == 0)
					{
						return Index;
					}
					break;
			}
		}

		return (TInt32)TDynamicArray<TString>::Npos;
	}
} // namespace TNsPrivate

TBool TCommandLine::GetValueOf(const TChar* OptionName, TString* OutValue, TStringSearchCase SearchChase) const
{
	TSize_T OptionNameLen = ::strlen(OptionName);
	TInt32 Index = TNsPrivate::GetCommandLineOptionIndex(CommandLineArgs, ArgCount, OptionName, OptionNameLen, SearchChase);
	if (Index == TDynamicArray<TString>::Npos || !OutValue)
	{
		return false;
	}

	const TChar* ValuePos = CommandLineArgs[Index].GetData() + OptionNameLen + 1;
	OutValue->Append(ValuePos);

	return true;
}

TBool TCommandLine::GetValueOf(const TChar* OptionName, TBool* OutValue, TStringSearchCase SearchChase) const
{
	TSize_T OptionNameLen = ::strlen(OptionName);
	TInt32 Index = TNsPrivate::GetCommandLineOptionIndex(CommandLineArgs, ArgCount, OptionName, OptionNameLen, SearchChase);
	if (Index == TDynamicArray<TString>::Npos || !OutValue)
	{
		return false;
	}

	const TChar* ValuePos = CommandLineArgs[Index].GetData() + OptionNameLen + 1;

	if (!TCString<TChar>::Stricmp(ValuePos, "true") || !TCString<TChar>::Stricmp(ValuePos, "on") || !TCString<TChar>::Stricmp(ValuePos, "1"))
	{
		*OutValue = true;
	}

	if (!TCString<TChar>::Stricmp(ValuePos, "false") || !TCString<TChar>::Stricmp(ValuePos, "off") || !TCString<TChar>::Stricmp(ValuePos, "0"))
	{
		*OutValue = true;
	}

	return true;
}

TBool TCommandLine::GetValueOf(const TChar* OptionName, TInt8* OutValue, TStringSearchCase SearchChase) const
{
	TSize_T OptionNameLen = ::strlen(OptionName);
	TInt32 Index = TNsPrivate::GetCommandLineOptionIndex(CommandLineArgs, ArgCount, OptionName, OptionNameLen, SearchChase);
	if (Index == TDynamicArray<TString>::Npos || !OutValue)
	{
		return false;
	}

	const TChar* ValuePos = CommandLineArgs[Index].GetData() + OptionNameLen + 1;
	TChar* StrEnd;
	*OutValue = static_cast<TInt8>(::strtol(ValuePos, &StrEnd, 10));

	return StrEnd[0] == '\0';
}

TBool TCommandLine::GetValueOf(const TChar* OptionName, TUInt8* OutValue, TStringSearchCase SearchChase) const
{
	TSize_T OptionNameLen = ::strlen(OptionName);
	TInt32 Index = TNsPrivate::GetCommandLineOptionIndex(CommandLineArgs, ArgCount, OptionName, OptionNameLen, SearchChase);
	if (Index == TDynamicArray<TString>::Npos || !OutValue)
	{
		return false;
	}

	const TChar* ValuePos = CommandLineArgs[Index].GetData() + OptionNameLen + 1;
	TChar* StrEnd;
	*OutValue = static_cast<TUInt8>(::strtoul(ValuePos, &StrEnd, 10));

	return StrEnd[0] == '\0';
}

TBool TCommandLine::GetValueOf(const TChar* OptionName, TInt16* OutValue, TStringSearchCase SearchChase) const
{
	TSize_T OptionNameLen = ::strlen(OptionName);
	TInt32 Index = TNsPrivate::GetCommandLineOptionIndex(CommandLineArgs, ArgCount, OptionName, OptionNameLen, SearchChase);
	if (Index == TDynamicArray<TString>::Npos || !OutValue)
	{
		return false;
	}

	const TChar* ValuePos = CommandLineArgs[Index].GetData() + OptionNameLen + 1;
	TChar* StrEnd;
	*OutValue = static_cast<TInt16>(::strtol(ValuePos, &StrEnd, 10));

	return StrEnd[0] == '\0';
}

TBool TCommandLine::GetValueOf(const TChar* OptionName, TUInt16* OutValue, TStringSearchCase SearchChase) const
{
	TSize_T OptionNameLen = ::strlen(OptionName);
	TInt32 Index = TNsPrivate::GetCommandLineOptionIndex(CommandLineArgs, ArgCount, OptionName, OptionNameLen, SearchChase);
	if (Index == TDynamicArray<TString>::Npos || !OutValue)
	{
		return false;
	}

	const TChar* ValuePos = CommandLineArgs[Index].GetData() + OptionNameLen + 1;
	TChar* StrEnd;
	*OutValue = static_cast<TUInt16>(::strtoul(ValuePos, &StrEnd, 10));

	return StrEnd[0] == '\0';
}

TBool TCommandLine::GetValueOf(const TChar* OptionName, TInt32* OutValue, TStringSearchCase SearchChase) const
{
	TSize_T OptionNameLen = ::strlen(OptionName);
	TInt32 Index = TNsPrivate::GetCommandLineOptionIndex(CommandLineArgs, ArgCount, OptionName, OptionNameLen, SearchChase);
	if (Index == TDynamicArray<TString>::Npos || !OutValue)
	{
		return false;
	}

	const TChar* ValuePos = CommandLineArgs[Index].GetData() + OptionNameLen + 1;
	TChar* StrEnd;
	*OutValue = static_cast<TInt32>(::strtol(ValuePos, &StrEnd, 10));

	return StrEnd[0] == '\0';
}

TBool TCommandLine::GetValueOf(const TChar* OptionName, TUInt32* OutValue, TStringSearchCase SearchChase) const
{
	TSize_T OptionNameLen = ::strlen(OptionName);
	TInt32 Index = TNsPrivate::GetCommandLineOptionIndex(CommandLineArgs, ArgCount, OptionName, OptionNameLen, SearchChase);
	if (Index == TDynamicArray<TString>::Npos || !OutValue)
	{
		return false;
	}

	const TChar* ValuePos = CommandLineArgs[Index].GetData() + OptionNameLen + 1;
	TChar* StrEnd;
	*OutValue = static_cast<TUInt32>(::strtoul(ValuePos, &StrEnd, 10));

	return StrEnd[0] == '\0';
}

TBool TCommandLine::GetValueOf(const TChar* OptionName, TInt64* OutValue, TStringSearchCase SearchChase) const
{
	TSize_T OptionNameLen = ::strlen(OptionName);
	TInt32 Index = TNsPrivate::GetCommandLineOptionIndex(CommandLineArgs, ArgCount, OptionName, OptionNameLen, SearchChase);
	if (Index == TDynamicArray<TString>::Npos || !OutValue)
	{
		return false;
	}

	const TChar* ValuePos = CommandLineArgs[Index].GetData() + OptionNameLen + 1;
	TChar* StrEnd;
	*OutValue = (::strtoll(ValuePos, &StrEnd, 10));

	return StrEnd[0] == '\0';
}

TBool TCommandLine::GetValueOf(const TChar* OptionName, TUInt64* OutValue, TStringSearchCase SearchChase) const
{
	TSize_T OptionNameLen = ::strlen(OptionName);
	TInt32 Index = TNsPrivate::GetCommandLineOptionIndex(CommandLineArgs, ArgCount, OptionName, OptionNameLen, SearchChase);
	if (Index == TDynamicArray<TString>::Npos || !OutValue)
	{
		return false;
	}

	const TChar* ValuePos = CommandLineArgs[Index].GetData() + OptionNameLen + 1;
	TChar* StrEnd;
	*OutValue = (::strtoull(ValuePos, &StrEnd, 10));

	return StrEnd[0] == '\0';
}

TBool TCommandLine::GetValueOf(const TChar* OptionName, TFloat* OutValue, TStringSearchCase SearchChase) const
{
	TSize_T OptionNameLen = ::strlen(OptionName);
	TInt32 Index = TNsPrivate::GetCommandLineOptionIndex(CommandLineArgs, ArgCount, OptionName, OptionNameLen, SearchChase);
	if (Index == TDynamicArray<TString>::Npos || !OutValue)
	{
		return false;
	}

	const TChar* ValuePos = CommandLineArgs[Index].GetData() + OptionNameLen + 1;
	TChar* StrEnd;
	*OutValue = ::strtof(ValuePos, &StrEnd);

	return StrEnd[0] == '\0';
}

TBool TCommandLine::GetValueOf(const TChar* OptionName, TDouble* OutValue, TStringSearchCase SearchChase) const
{
	TSize_T OptionNameLen = ::strlen(OptionName);
	TInt32 Index = TNsPrivate::GetCommandLineOptionIndex(CommandLineArgs, ArgCount, OptionName, OptionNameLen, SearchChase);
	if (Index == TDynamicArray<TString>::Npos || !OutValue)
	{
		return false;
	}

	const TChar* ValuePos = CommandLineArgs[Index].GetData() + OptionNameLen + 1;
	TChar* StrEnd;
	*OutValue = ::strtof(ValuePos, &StrEnd);

	return StrEnd[0] == '\0';
}