#pragma once

#include "Trinity/Core/Defines.h"

#include "Trinity/Core/Containers/DynamicArray.h"
#include "Trinity/Core/String/StringConversion.h"
#include "Trinity/Core/String/String.h"

class TCommandLine
{
public:
	TCommandLine();

	~TCommandLine();

	TCommandLine(const TCommandLine&) = delete;
	TCommandLine(TCommandLine&&) = delete;
	TCommandLine& operator=(const TCommandLine&) = delete;
	TCommandLine& operator=(TCommandLine&&) = delete;

	void SetCommandLine(TInt32 ArgCount, TChar** Argv);
	void Reset();

	TBool CommandLineOptionExists(const TChar* OptionName, TStringSearchCase SearchChase = TStringSearchCase::ECaseSensitive) const;
	TInt32 GetCommandLineOptionIndex(const TChar* OptionName, TStringSearchCase SearchChase = TStringSearchCase::ECaseSensitive) const;
	TString GetCommandLineAsString() const;

	void AddCommandLineArgument(const TChar* CmdLineArgument);
	template<typename T>
	void AddCommandLineArgument(const TChar* OptionName, T Value)
	{
		static_assert(TIsStringable<T, TChar>::Value, "TStringConverter<TChar>::ToString does not support this type `T`");

		TString CmdLineArgument(OptionName);
		CmdLineArgument += "=";
		CmdLineArgument += TNsStringConversion::ToString<TChar>(Value);

		CommandLineArgs.EmplaceBack(CmdLineArgument);
		++ArgCount;
	}

public:
	TRNT_NODISCARD TRNT_FORCE_INLINE TInt64 GetCommanndLineArgCount() const { return ArgCount; }

	TRNT_NODISCARD TRNT_FORCE_INLINE const TString& GetExecutableFilePath() const { return CommandLineArgs[0]; }

	TRNT_NODISCARD TRNT_FORCE_INLINE const TDynamicArray<TString>& GetCommandLineArgs() const { return CommandLineArgs; }

	TRNT_NODISCARD TRNT_FORCE_INLINE const TString& CommandLineArgAt(TInt32 Index) const { return CommandLineArgs[(TInt64)Index]; }

public:
	TRNT_NODISCARD TBool GetValueOf(const TChar* OptionName, TString* OutValue, TStringSearchCase SearchChase = TStringSearchCase::ECaseSensitive) const;

	TRNT_NODISCARD TBool GetValueOf(const TChar* OptionName, TBool* OutValue, TStringSearchCase SearchChase = TStringSearchCase::ECaseSensitive) const;

	TRNT_NODISCARD TBool GetValueOf(const TChar* OptionName, TInt8* OutValue, TStringSearchCase SearchChase = TStringSearchCase::ECaseSensitive) const;

	TRNT_NODISCARD TBool GetValueOf(const TChar* OptionName, TUInt8* OutValue, TStringSearchCase SearchChase = TStringSearchCase::ECaseSensitive) const;

	TRNT_NODISCARD TBool GetValueOf(const TChar* OptionName, TInt16* OutValue, TStringSearchCase SearchChase = TStringSearchCase::ECaseSensitive) const;

	TRNT_NODISCARD TBool GetValueOf(const TChar* OptionName, TUInt16* OutValue, TStringSearchCase SearchChase = TStringSearchCase::ECaseSensitive) const;

	TRNT_NODISCARD TBool GetValueOf(const TChar* OptionName, TInt32* OutValue, TStringSearchCase SearchChase = TStringSearchCase::ECaseSensitive) const;

	TRNT_NODISCARD TBool GetValueOf(const TChar* OptionName, TUInt32* OutValue, TStringSearchCase SearchChase = TStringSearchCase::ECaseSensitive) const;

	TRNT_NODISCARD TBool GetValueOf(const TChar* OptionName, TInt64* OutValue, TStringSearchCase SearchChase = TStringSearchCase::ECaseSensitive) const;

	TRNT_NODISCARD TBool GetValueOf(const TChar* OptionName, TUInt64* OutValue, TStringSearchCase SearchChase = TStringSearchCase::ECaseSensitive) const;

	TRNT_NODISCARD TBool GetValueOf(const TChar* OptionName, TFloat* OutValue, TStringSearchCase SearchChase = TStringSearchCase::ECaseSensitive) const;

	TRNT_NODISCARD TBool GetValueOf(const TChar* OptionName, TDouble* OutValue, TStringSearchCase SearchChase = TStringSearchCase::ECaseSensitive) const;

public:
	static TRNT_NODISCARD TRNT_FORCE_INLINE TCommandLine* GetInstance() { return &Instance; }

private:
	static TCommandLine Instance;

	TDynamicArray<TString> CommandLineArgs;
	TInt32 ArgCount;
};