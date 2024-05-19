#include "CommandLine.h"

TCommandLineParser* TCommandLineParser::Instance = nullptr;

void TCommandLineParser::DeleteInstance()
{
	if (!Instance)
	{
		return;
	}
	
	Instance->CommandLineOptions.Clear();
	Instance->CommandLineArgs.Clear();

	delete Instance;
	Instance = nullptr;
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

TRNT_NODISCARD TString TCommandLineParser::GetStringOption(const TString& OptionName, TStringSearchCase SearchCase)
{
	TInt64 Index = CommandLineOptions.FindElementIf([&OptionName, SearchCase](const TCommandLineOption& Option) -> TBool
		{
			return Option.OptName.IsEquals(OptionName, SearchCase);
		});

	if (Index == TDynamicArray<TCommandLineOption>::Npos)
	{
		return "";
	}

	TRNT_ASSERT(CommandLineOptions[Index].OptType == TCommandLineOptType::EString);

	return CommandLineOptions[Index].Value;
}

TRNT_NODISCARD TBool TCommandLineParser::GetBooleanOption(const TString& OptionName, TStringSearchCase SearchCase)
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

	return TNsStringConversion::StringToBool(CommandLineOptions[Index].Value);
}

TRNT_NODISCARD TFloat TCommandLineParser::GetFloatOption(const TString& OptionName, TStringSearchCase SearchCase)
{
	TInt64 Index = CommandLineOptions.FindElementIf([&OptionName, SearchCase](const TCommandLineOption& Option) -> TBool
		{
			return Option.OptName.IsEquals(OptionName, SearchCase);
		});

	if (Index == TDynamicArray<TCommandLineOption>::Npos)
	{
		return 0.0f;
	}

	TRNT_ASSERT(CommandLineOptions[Index].OptType == TCommandLineOptType::EFloat);

	return strtof(CommandLineOptions[Index].Value.GetData(), nullptr);
}

TRNT_NODISCARD TDouble TCommandLineParser::GetDoubleOption(const TString& OptionName, TStringSearchCase SearchCase)
{
	TInt64 Index = CommandLineOptions.FindElementIf([&OptionName, SearchCase](const TCommandLineOption& Option) -> TBool
		{
			return Option.OptName.IsEquals(OptionName, SearchCase);
		});

	if (Index == TDynamicArray<TCommandLineOption>::Npos)
	{
		return 0.0;
	}

	TRNT_ASSERT(CommandLineOptions[Index].OptType == TCommandLineOptType::EDouble);

	return ::atof(CommandLineOptions[Index].Value.GetData());
}

TRNT_NODISCARD TInt32 TCommandLineParser::GetInt32Option(const TString& OptionName, TStringSearchCase SearchCase)
{
	TInt64 Index = CommandLineOptions.FindElementIf([&OptionName, SearchCase](const TCommandLineOption& Option) -> TBool
		{
			return Option.OptName.IsEquals(OptionName, SearchCase);
		});

	if (Index == TDynamicArray<TCommandLineOption>::Npos)
	{
		return 0;
	}

	TRNT_ASSERT(CommandLineOptions[Index].OptType == TCommandLineOptType::EInt32);

	return ::atoi(CommandLineOptions[Index].Value.GetData());
}

TRNT_NODISCARD TUInt32 TCommandLineParser::GetUInt32Option(const TString& OptionName, TStringSearchCase SearchCase)
{
	TInt64 Index = CommandLineOptions.FindElementIf([&OptionName, SearchCase](const TCommandLineOption& Option) -> TBool
		{
			return Option.OptName.IsEquals(OptionName, SearchCase);
		});

	if (Index == TDynamicArray<TCommandLineOption>::Npos)
	{
		return 0;
	}

	TRNT_ASSERT(CommandLineOptions[Index].OptType == TCommandLineOptType::EUInt32);

	return TNsStringConversion::StringToUInt32(CommandLineOptions[Index].Value);
}

TRNT_NODISCARD TInt64 TCommandLineParser::GetInt64Option(const TString& OptionName, TStringSearchCase SearchCase)
{
	TInt64 Index = CommandLineOptions.FindElementIf([&OptionName, SearchCase](const TCommandLineOption& Option) -> TBool
		{
			return Option.OptName.IsEquals(OptionName, SearchCase);
		});

	if (Index == TDynamicArray<TCommandLineOption>::Npos)
	{
		return 0;
	}

	TRNT_ASSERT(CommandLineOptions[Index].OptType == TCommandLineOptType::EInt64);

	return ::atoll(CommandLineOptions[Index].Value.GetData());
}

TRNT_NODISCARD TUInt64 TCommandLineParser::GetUInt64Option(const TString& OptionName, TStringSearchCase SearchCase)
{
	TInt64 Index = CommandLineOptions.FindElementIf([&OptionName, SearchCase](const TCommandLineOption& Option) -> TBool
		{
			return Option.OptName.IsEquals(OptionName, SearchCase);
		});

	if (Index == TDynamicArray<TCommandLineOption>::Npos)
	{
		return 0;
	}

	TRNT_ASSERT(CommandLineOptions[Index].OptType == TCommandLineOptType::EUInt64);

	return TNsStringConversion::StringToUInt64(CommandLineOptions[Index].Value);
}