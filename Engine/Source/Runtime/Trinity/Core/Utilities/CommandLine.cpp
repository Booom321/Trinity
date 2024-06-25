#include "TrinityPCH.h"
#include "CommandLine.h"

TCommandLineParser TCommandLineParser::Instance = TCommandLineParser();

TCommandLineParser::~TCommandLineParser()
{
	CommandLineArgs.Clear();
	CommandLineOptions.Clear();
}

void TCommandLineParser::Delete(TCommandLineParser* CmdLineParser)
{
	if (!CmdLineParser)
	{
		return;
	}

	delete CmdLineParser;
	return;
}

void TCommandLineParser::SetCommandLine(TInt32 ArgCount, TChar** ArgV)
{
	CommandLineArgs.Clear();
	CommandLineArgs.Reserve(ArgCount);

	for (TInt32 Index = 1; Index < ArgCount; ++Index)
	{
		CommandLineArgs.EmplaceBack(ArgV[Index]);
	}
}

TBool TCommandLineParser::IsCommandLineOptionExists(TString OptName)
{
	TRNT_ASSERT_MESSAGE(OptName[0] == '-', "Command line option name must start with a hyphen (\"-\").");

	for (TInt32 Index = 0, OptionCount = (TInt32)CommandLineOptions.GetElementCount(); Index < OptionCount; ++Index)
	{
		if (CommandLineOptions[Index].OptName == OptName)
		{
			return true;
		}
	}

	return false;
}

#pragma warning( push )
#pragma warning(disable: 6031)

TBool IsCommandLineArgValueValid(TCommandLineOptType OptType, const TString& Value)
{
	TChar* StrEnd;

	switch (OptType)
	{
	case TCommandLineOptType::EString:
		return true;
	case TCommandLineOptType::EInt32:
		::strtol(Value.GetData(), &StrEnd, 10);
		return StrEnd[0] == '\0';
	case TCommandLineOptType::EInt64:
		::strtoll(Value.GetData(), &StrEnd, 10);
		return StrEnd[0] == '\0';
	case TCommandLineOptType::EUInt32:
		::strtoul(Value.GetData(), &StrEnd, 10);
		return StrEnd[0] == '\0';
	case TCommandLineOptType::EUInt64:
		::strtoull(Value.GetData(), &StrEnd, 10);
		return StrEnd[0] == '\0';
	case TCommandLineOptType::EFloat:
		::strtof(Value.GetData(), &StrEnd);
		return StrEnd[0] == '\0';
	case TCommandLineOptType::EDouble:
		::strtod(Value.GetData(), &StrEnd);
		return StrEnd[0] == '\0';
	case TCommandLineOptType::EBool:
		return Value == "1" || Value == "0" || Value.IsEquals("true", TStringSearchCase::EIgnoreCase) || Value.IsEquals("false", TStringSearchCase::EIgnoreCase);
	}

	return false;
}

#pragma warning( pop )

TBool TCommandLineParser::Parse()
{
	TDynamicArray<TString> CommandArgParts(2);

	for (TInt32 Index = 0, CommandLineArgCount = (TInt32)CommandLineArgs.GetElementCount(); Index < CommandLineArgCount; ++Index)
	{
		CommandLineArgs[Index].SplitByString(CommandArgParts, "=");

		const TString& Argument = CommandArgParts[0];

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if (Argument[0] != '-')
		{
			return false;
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		TInt64 FoundIndex = CommandLineOptions.FindElementIf([&Argument](const TCommandLineOption& Option) -> TBool
		{
			return Option.OptName == Argument;
		});

		if (FoundIndex == TDynamicArray<TCommandLineOption>::Npos)
		{
			// Unknown command line argument
			CommandArgParts.Clear();
			continue;
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		if (CommandArgParts.GetElementCount() > 1)
		{
			const TString& Value = CommandArgParts[1];

			if (!IsCommandLineArgValueValid(CommandLineOptions[FoundIndex].OptType, Value))
			{
				// invalid value!
				return false;
			}
			CommandLineOptions[FoundIndex].Value = Value;
		}

		CommandArgParts.Clear();
	}

	return true;
}

TRNT_NODISCARD TBool TCommandLineParser::GetStringOption(TString& Value, const TString& OptionName, TStringSearchCase SearchCase)
{
	TInt64 Index = CommandLineOptions.FindElementIf([&OptionName, SearchCase](const TCommandLineOption& Option) -> TBool
		{
			return Option.OptName.IsEquals(OptionName, SearchCase);
		});

	if (Index == TDynamicArray<TCommandLineOption>::Npos)
	{
		return false;
	}

	TRNT_ASSERT(CommandLineOptions[Index].OptType == TCommandLineOptType::EString);

	Value = CommandLineOptions[Index].Value;
	return true;
}

TRNT_NODISCARD TBool TCommandLineParser::GetBooleanOption(TBool& Value, const TString& OptionName, TStringSearchCase SearchCase)
{
	TInt64 Index = CommandLineOptions.FindElementIf([&OptionName, SearchCase](const TCommandLineOption& Option) -> TBool
		{
			return Option.OptName.IsEquals(OptionName, SearchCase);
		});

	if (Index == TDynamicArray<TCommandLineOption>::Npos)
	{
		return false;
	}
	
	TRNT_ASSERT(CommandLineOptions[Index].OptType == TCommandLineOptType::EBool);

	Value = TNsStringConversion::StringToBool(CommandLineOptions[Index].Value);
	return true;
}

TRNT_NODISCARD TBool TCommandLineParser::GetFloatOption(TFloat& Value, const TString& OptionName, TStringSearchCase SearchCase)
{
	TInt64 Index = CommandLineOptions.FindElementIf([&OptionName, SearchCase](const TCommandLineOption& Option) -> TBool
		{
			return Option.OptName.IsEquals(OptionName, SearchCase);
		});

	if (Index == TDynamicArray<TCommandLineOption>::Npos)
	{
		return false;
	}

	TRNT_ASSERT(CommandLineOptions[Index].OptType == TCommandLineOptType::EFloat);

	Value = ::strtof(CommandLineOptions[Index].Value.GetData(), nullptr);
	return true;
}

TRNT_NODISCARD TBool TCommandLineParser::GetDoubleOption(TDouble& Value, const TString& OptionName, TStringSearchCase SearchCase)
{
	TInt64 Index = CommandLineOptions.FindElementIf([&OptionName, SearchCase](const TCommandLineOption& Option) -> TBool
		{
			return Option.OptName.IsEquals(OptionName, SearchCase);
		});

	if (Index == TDynamicArray<TCommandLineOption>::Npos)
	{
		return false;
	}

	TRNT_ASSERT(CommandLineOptions[Index].OptType == TCommandLineOptType::EDouble);

	Value = ::atof(CommandLineOptions[Index].Value.GetData());
	return true;
}

TRNT_NODISCARD TBool TCommandLineParser::GetInt32Option(TInt32& Value, const TString& OptionName, TStringSearchCase SearchCase)
{
	TInt64 Index = CommandLineOptions.FindElementIf([&OptionName, SearchCase](const TCommandLineOption& Option) -> TBool
		{
			return Option.OptName.IsEquals(OptionName, SearchCase);
		});

	if (Index == TDynamicArray<TCommandLineOption>::Npos)
	{
		return false;
	}

	TRNT_ASSERT(CommandLineOptions[Index].OptType == TCommandLineOptType::EInt32);

	Value = ::atoi(CommandLineOptions[Index].Value.GetData());
	return true;
}

TRNT_NODISCARD TBool TCommandLineParser::GetUInt32Option(TUInt32& Value, const TString& OptionName, TStringSearchCase SearchCase)
{
	TInt64 Index = CommandLineOptions.FindElementIf([&OptionName, SearchCase](const TCommandLineOption& Option) -> TBool
		{
			return Option.OptName.IsEquals(OptionName, SearchCase);
		});

	if (Index == TDynamicArray<TCommandLineOption>::Npos)
	{
		return false;
	}

	TRNT_ASSERT(CommandLineOptions[Index].OptType == TCommandLineOptType::EUInt32);

	Value = TNsStringConversion::StringToUInt32(CommandLineOptions[Index].Value);
	return true;
}

TRNT_NODISCARD TBool TCommandLineParser::GetInt64Option(TInt64& Value, const TString& OptionName, TStringSearchCase SearchCase)
{
	TInt64 Index = CommandLineOptions.FindElementIf([&OptionName, SearchCase](const TCommandLineOption& Option) -> TBool
		{
			return Option.OptName.IsEquals(OptionName, SearchCase);
		});

	if (Index == TDynamicArray<TCommandLineOption>::Npos)
	{
		return false;
	}

	TRNT_ASSERT(CommandLineOptions[Index].OptType == TCommandLineOptType::EInt64);

	Value = ::atoll(CommandLineOptions[Index].Value.GetData());
	return true;
}

TRNT_NODISCARD TBool TCommandLineParser::GetUInt64Option(TUInt64& Value, const TString& OptionName, TStringSearchCase SearchCase)
{
	TInt64 Index = CommandLineOptions.FindElementIf([&OptionName, SearchCase](const TCommandLineOption& Option) -> TBool
		{
			return Option.OptName.IsEquals(OptionName, SearchCase);
		});

	if (Index == TDynamicArray<TCommandLineOption>::Npos)
	{
		return false;
	}

	TRNT_ASSERT(CommandLineOptions[Index].OptType == TCommandLineOptType::EUInt64);

	Value = TNsStringConversion::StringToUInt64(CommandLineOptions[Index].Value);
	return true;
}