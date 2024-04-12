#include "Log.h"

#pragma warning(push)
#pragma warning(disable: 4996)
#pragma warning(disable: 6387)

TMutex TLog::LogMutex;
FILE* TLog::OutputLogFile = nullptr;
typename TLog::LogMessageHandlerList TLog::MessageHandlers;

void TLog::AddMessageHandlerCallback(TLogMessageHandlerCallback Callback)
{
	LogMutex.Lock();
	if (MessageHandlers.Contains(Callback))
	{
		return;
	}
	MessageHandlers.InsertAtTail(new LogMessageHandlerList::NodeType(Callback));
	LogMutex.Unlock();
}

void TLog::RemoveMessageHandlerCallback(TLogMessageHandlerCallback Callback)
{
	LogMutex.Lock();
	MessageHandlers.RemoveAt(MessageHandlers.Find(Callback));
	LogMutex.Unlock();
}

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

void TLog::DefaultStdoutMessageHandler(TLogLevel LogLevel, const TChar* FormattedMessageA, TSize_T FormattedMessageALen, const TWChar* FormattedMessageW, TSize_T FormattedMessageWLen)
{
#if defined(TRNT_DEBUG) // Only write to console in debug mode!
	static constexpr const TChar* DefaultTextColorA = "\033[39m";
	static constexpr const TWChar* DefaultTextColorW = L"\033[39m";
	FILE* StdoutType = nullptr;

	if (FormattedMessageA != nullptr)
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

		fprintf(StdoutType, "%s%s%s", TextColor, FormattedMessageA, (LogLevel == TLogLevel::EFatal) ? "\033[39m\x1B[40m" : DefaultTextColorA);
	}
	else if (FormattedMessageW != nullptr)
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

		fwprintf(StdoutType, L"%ls%ls%ls", TextColor, FormattedMessageW, (LogLevel == TLogLevel::EFatal) ? L"\033[39m\x1B[40m" : DefaultTextColorW);
	}
#endif
}

void TLog::DefaultFileMessageHandler(TLogLevel LogLevel, const TChar* FormattedMessageA, TSize_T FormattedMessageALen, const TWChar* FormattedMessageW, TSize_T FormattedMessageWLen)
{
	if (OutputLogFile == nullptr)
	{
		return;
	}

	if (FormattedMessageA != nullptr)
	{
		std::fwrite(FormattedMessageA, sizeof(TChar), FormattedMessageALen, OutputLogFile);
	}
	else if (FormattedMessageW != nullptr)
	{
		std::fputws(FormattedMessageW, OutputLogFile);
	}
}

#pragma warning(pop)
