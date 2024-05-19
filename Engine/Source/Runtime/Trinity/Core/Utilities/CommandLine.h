#pragma once

#include "Trinity/Core/Defines.h"

#include "Trinity/Core/Containers/DynamicArray.h"
#include "Trinity/Core/String/StringConversion.h"
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
	TCommandLineParser() = default;
	~TCommandLineParser() = default;

	TCommandLineParser(const TCommandLineParser&) = delete;
	TCommandLineParser(TCommandLineParser&&) = delete;
	TCommandLineParser& operator=(const TCommandLineParser&) = delete;
	TCommandLineParser& operator=(TCommandLineParser&&) = delete;

public:
	static TRNT_FORCE_INLINE TCommandLineParser* GetInstance() { return (Instance) ? Instance : (Instance = new TCommandLineParser()); }

	static void DeleteInstance();

public:
	void SetCommandLine(TInt32 ArgCount, TChar** ArgV);

	template<typename T>
	TRNT_FORCE_INLINE void AddCommandLineOption(const TString& OptName, const T& DefaultValue)
	{
		static_assert(
			TOr<
				TAreTheSameType<T, TString>,
				TAreTheSameType<T, TUInt32>, TAreTheSameType<T, TInt32>, TAreTheSameType<T, TUInt64>, TAreTheSameType<T, TInt64>,
				TIsFloatingPoint<T>, TAreTheSameType<T, TBool>
			>::Value);

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
	TRNT_NODISCARD TString GetStringOption(const TString& OptionName, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive);

	TRNT_NODISCARD TBool GetBooleanOption(const TString& OptionName, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive);

	TRNT_NODISCARD TFloat GetFloatOption(const TString& OptionName, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive);

	TRNT_NODISCARD TDouble GetDoubleOption(const TString& OptionName, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive);

	TRNT_NODISCARD TInt32 GetInt32Option(const TString& OptionName, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive);

	TRNT_NODISCARD TUInt32 GetUInt32Option(const TString& OptionName, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive);

	TRNT_NODISCARD TInt64 GetInt64Option(const TString& OptionName, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive);

	TRNT_NODISCARD TUInt64 GetUInt64Option(const TString& OptionName, TStringSearchCase SearchCase = TStringSearchCase::ECaseSensitive);

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

	static TCommandLineParser* Instance;

	TDynamicArray<TString> CommandLineArgs;

	TDynamicArray<TCommandLineOption> CommandLineOptions;
};