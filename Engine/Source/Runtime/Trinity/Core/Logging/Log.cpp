#include "TrinityPCH.h"

#pragma warning(push)
#pragma warning(disable : 4996)
#pragma warning(disable : 6387)

TMutex TLog::LogMutex;
FILE* TLog::OutputLogFile = nullptr;
typename TLog::LogMessageHandlerList TLog::MessageHandlers;

FILE* TLog::OpenLogFile(const TChar* Filename, TBool EnableToAppend)
{
	TRNT_ASSERT_IS_NULL(OutputLogFile);

	LogMutex.Lock();

	if (EnableToAppend)
	{
		OutputLogFile = fopen(Filename, "a+");
	}
	else
	{
		OutputLogFile = fopen(Filename, "w");
	}

	LogMutex.Unlock();

	return OutputLogFile;
}

void TLog::CloseLogFile()
{
	if (OutputLogFile != nullptr)
	{
		fclose(OutputLogFile);
		OutputLogFile = nullptr;
	}
}

void TLog::Initialize()
{
	// implement later!
}

void TLog::Shutdown()
{
	// implement later!
}

void TLog::ClearAllMessageHandlerCallbacks()
{
	LogMutex.Lock();
	MessageHandlers.Clear();
	LogMutex.Unlock();
}

void TLog::DefaultStdoutMessageHandler(TLogLevel LogLevel, TLogCharType CharType, void* FormattedMessage, TSize_T FormattedMsgLen)
{
#if defined(TRNT_DEBUG) // Only write to console in debug mode!
	static TRNT_CONSTEXPR const TChar* DefaultTextColorA = "\033[39m";
	static TRNT_CONSTEXPR const TWChar* DefaultTextColorW = L"\033[39m";
	FILE* StdoutType = nullptr;

	if (FormattedMessage)
	{
		if (CharType == TLogCharType::EChar)
		{
			const TChar* TextColor = nullptr;

			switch (LogLevel)
			{
				case TLogLevel::EDebug:
					StdoutType = stdout;
					TextColor = "\x1B[36m";
					break;
				case TLogLevel::EInfo:
					StdoutType = stdout;
					TextColor = "";
					break;
				case TLogLevel::ESuccess:
					StdoutType = stdout;
					TextColor = "\033[92m";
					break;
				case TLogLevel::EWarning:
					StdoutType = stderr;
					TextColor = "\033[33m";
					break;
				case TLogLevel::EError:
					StdoutType = stderr;
					TextColor = "\033[91m";
					break;
				case TLogLevel::EFatal:
					StdoutType = stderr;
					TextColor = "\x1B[41m\033[97m";
					break;
			}

			fprintf(StdoutType, "%s%s%s", TextColor, static_cast<const TChar*>(FormattedMessage), (LogLevel == TLogLevel::EFatal) ? "\033[39m\x1B[40m" : DefaultTextColorA);
		}
		else if (CharType == TLogCharType::EWChar)
		{
			const TWChar* TextColor = nullptr;

			switch (LogLevel)
			{
				case TLogLevel::EDebug:
					StdoutType = stdout;
					TextColor = L"\x1B[36m";
					break;
				case TLogLevel::EInfo:
					StdoutType = stdout;
					TextColor = L"";
					break;
				case TLogLevel::ESuccess:
					StdoutType = stdout;
					TextColor = L"\033[92m";
					break;
				case TLogLevel::EWarning:
					StdoutType = stderr;
					TextColor = L"\033[33m";
					break;
				case TLogLevel::EError:
					StdoutType = stderr;
					TextColor = L"\033[91m";
					break;
				case TLogLevel::EFatal:
					StdoutType = stderr;
					TextColor = L"\x1B[41m\033[97m";
					break;
			}

			fwprintf(StdoutType, L"%ls%ls%ls", TextColor, static_cast<const TWChar*>(FormattedMessage), (LogLevel == TLogLevel::EFatal) ? L"\033[39m\x1B[40m" : DefaultTextColorW);
		}
	}
#endif
}

void TLog::DefaultFileMessageHandler(TLogLevel LogLevel, TLogCharType CharType, void* FormattedMessage, TSize_T FormattedMsgLen)
{
	if (OutputLogFile == nullptr)
	{
		return;
	}

	if (FormattedMessage)
	{
		switch (CharType)
		{
			case TLogCharType::EChar:
				std::fwrite(static_cast<const TChar*>(FormattedMessage), sizeof(TChar), FormattedMsgLen, OutputLogFile);
				break;
			case TLogCharType::EWChar:
				std::fputws(static_cast<const TWChar*>(FormattedMessage), OutputLogFile);
				break;
		}
	}
}

#pragma warning(pop)
