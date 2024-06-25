#pragma once

#include "Trinity/Core/Defines.h"

#include "Trinity/Core/Containers/DynamicArray.h"
#include "Trinity/Core/String/StringConversion.h"
#include "Trinity/Core/TypeTraits/RemoveCVRef.h"
#include "Trinity/Core/String/String.h"
#include "Trinity/Core/Types/Pair.h"

enum class TCommandLineOptType : TInt8
{
	EUnknown = -1,
	EString,
	EInt32,
	EUInt32,
	EInt64,
	EUInt64,
	EFloat,
	EDouble,
	EBool,
};

class TCommandLineOption
{
public:
	TCommandLineOption() = default;

	~TCommandLineOption() = default;

	TCommandLineOption(const TString& OptName, TCommandLineOptType OptType, const TString& Value)
		: OptName(OptName), OptType(OptType), Value(Value)
	{
		TRNT_ASSERT_MESSAGE(OptName[0] == '-', "Command line option name must start with a hyphen(\"-\")");
	}

	TString OptName;
	TCommandLineOptType OptType = TCommandLineOptType::EUnknown;
	TString Value;
};

class TRNT_API TCommandLineParser
{
public:
	TCommandLineParser() = default;

	TCommandLineParser(const TCommandLineParser&) = delete;
	TCommandLineParser(TCommandLineParser&&) = delete;
	TCommandLineParser& operator=(const TCommandLineParser&) = delete;
	TCommandLineParser& operator=(TCommandLineParser&&) = delete;

public:
	~TCommandLineParser();

	static TRNT_FORCE_INLINE TCommandLineParser* GetInstance() { return &Instance; }

	static TRNT_FORCE_INLINE TCommandLineParser* Create() { return new TCommandLineParser(); }

	static void Delete(TCommandLineParser* CmdLineParser);

public:
	void SetCommandLine(TInt32 ArgCount, TChar** ArgV);

	template<typename T>
	TRNT_FORCE_INLINE void AddCommandLineOption(const TString& OptName, const T& DefaultValue)
	{
		static_assert(TStringable<T>::Value);

		const auto Predicate = [&OptName](const TCommandLineOption& Option) -> TBool
			{
				return Option.OptName == OptName;
			};

		TUInt64 Index = CommandLineOptions.FindElementIf(Predicate);

		if (Index != TDynamicArray<TCommandLineOption>::Npos)
		{
			GetCommandLineOption<T>(OptName, DefaultValue, CommandLineOptions[Index]);
			return;
		}
	
		TCommandLineOption CommandLineOpt;
		GetCommandLineOption<T>(OptName, DefaultValue, CommandLineOpt);

		CommandLineOptions.EmplaceBack(CommandLineOpt);
	}

	TBool IsCommandLineOptionExists(TString OptName);

	TBool Parse();

public:
	TRNT_NODISCARD TBool GetStringOption(TString& Value, const TString& OptionName, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive);

	TRNT_NODISCARD TBool GetBooleanOption(TBool& Value, const TString& OptionName, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive);

	TRNT_NODISCARD TBool GetFloatOption(TFloat& Value, const TString& OptionName, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive);

	TRNT_NODISCARD TBool GetDoubleOption(TDouble& Value, const TString& OptionName, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive);

	TRNT_NODISCARD TBool GetInt32Option(TInt32& Value, const TString& OptionName, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive);

	TRNT_NODISCARD TBool GetUInt32Option(TUInt32& Value, const TString& OptionName, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive);

	TRNT_NODISCARD TBool GetInt64Option(TInt64& Value, const TString& OptionName, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive);

	TRNT_NODISCARD TBool GetUInt64Option(TUInt64& Value, const TString& OptionName, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive);

public:
	TRNT_NODISCARD const TString& CommandLineArgAt(TInt32 Index) const
	{
		return CommandLineArgs[(TInt64)Index];
	}

	TRNT_NODISCARD const TCommandLineOption& CommandLineOptionAt(TInt32 Index) const
	{
		return CommandLineOptions[(TInt64)Index];
	}

	TRNT_NODISCARD TInt64 GetCommanndLineArgCount() const { return CommandLineArgs.GetElementCount(); }

	TRNT_NODISCARD TInt64 GetCommanndLineOptionCount() const { return CommandLineOptions.GetElementCount(); }

private:
	template<typename T>
	TRNT_FORCE_INLINE void GetCommandLineOption(const TString& OptName, const T& Value, TCommandLineOption& Option)
	{
		Option.OptName = OptName;
		
		if constexpr (TAreTheSameType<T, TString>::Value)
		{
			Option.OptType = TCommandLineOptType::EString;
			Option.Value = Value;
		}
		else
		{
			TString ValueAsString;
			TNsStringConversion::ToString(Value, ValueAsString);

			Option.Value = ValueAsString;

			if constexpr (TAreTheSameType<T, TUInt32>::Value)
			{
				Option.OptType = TCommandLineOptType::EUInt32;
			}
			else if constexpr (TAreTheSameType<T, TInt32>::Value)
			{
				Option.OptType = TCommandLineOptType::EInt32;
			}
			else if constexpr (TAreTheSameType<T, TUInt64>::Value)
			{
				Option.OptType = TCommandLineOptType::EUInt64;
			}
			else if constexpr (TAreTheSameType<T, TInt64>::Value)
			{
				Option.OptType = TCommandLineOptType::EInt64;
			}
			else if constexpr (TAreTheSameType<T, TFloat>::Value)
			{
				Option.OptType = TCommandLineOptType::EFloat;
			}
			else if constexpr (TAreTheSameType<T, TDouble>::Value)
			{
				Option.OptType = TCommandLineOptType::EDouble;
			}
			else if constexpr (TAreTheSameType<T, TBool>::Value)
			{
				Option.OptType = TCommandLineOptType::EBool;
			}
		}

		return;
	}

	static TCommandLineParser Instance;

	TDynamicArray<TString> CommandLineArgs;
	TDynamicArray<TCommandLineOption> CommandLineOptions;
};