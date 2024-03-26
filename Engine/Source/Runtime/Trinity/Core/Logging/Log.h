#pragma once

#include <fmt/chrono.h>
#include <fmt/xchar.h>

#include "Trinity/Platform/FileIO.h"
#include "Trinity/Core/Threading/Mutex.h"
#include "Trinity/Core/Containers/LinkedList.h"
#include "Trinity/Core/Utilities/OutputStream.h"
#include "LogLevel.h"

template<const TChar* LogNameCharArray, const TWChar* LogNameWCharArray, TLogLevel Level>
class TCompileTimeLogSettings
{
public:
	static TRNT_CONSTEXPR const TChar*  LogNameAsCString	= LogNameCharArray;
	static TRNT_CONSTEXPR const TWChar* LogNameAsWCString	= LogNameWCharArray;
	static TRNT_CONSTEXPR TLogLevel     LogLevel			= Level;
};

#define TRNT_MAKE_LOG_SETTINGS_NAME(LogName) T##LogName##LogSettings

#define TRNT_DECLARE_LOG_SETTINGS(LogName, LogLevel) \
	static TRNT_CONSTEXPR const TChar LogName##CharArray[] = TRNT_STRINGIFY(LogName);\
	static TRNT_CONSTEXPR const TWChar LogName##WCharArray[] = L#LogName;\
	class TRNT_MAKE_LOG_SETTINGS_NAME(LogName) : public TCompileTimeLogSettings<LogName##CharArray, LogName##WCharArray, LogLevel>\
	{}

#define TRNT_LOG_SETTINGS_NAME(LogName) TRNT_MAKE_LOG_SETTINGS_NAME(LogName)

TRNT_DECLARE_LOG_SETTINGS(Default, TLogLevel::EDebug);


/*

**************************************************** EXAMPLE ****************************************************



TRNT_DECLARE_LOG_SETTINGS(MyLogName, TLogLevel::EDebug);

void MessageHandlerCallback(const TChar* TCMessage, TSize_T TCMessageLen, const TWChar* TWCMessage, TSize_T TWCMessageLen)
{
	printf("%s", TCMessage);
}

void Foo()
{
	TLog<TRNT_LOG_SETTINGS_NAME(MyLogName)>::AddMessageHandlerCallback(MessageHandlerCallback);

	TLog<TRNT_LOG_SETTINGS_NAME(MyLogName)>::Debug("{} {}", "Hello", "world");
	TLog<TRNT_LOG_SETTINGS_NAME(MyLogName)>::Info("{} {}", "Hello", "world");
	TLog<TRNT_LOG_SETTINGS_NAME(MyLogName)>::Success("{} {}", "Hello", "world");
	TLog<TRNT_LOG_SETTINGS_NAME(MyLogName)>::Warning("{} {}", "Hello", "world");
	TLog<TRNT_LOG_SETTINGS_NAME(MyLogName)>::Error("{} {}", "Hello", "world");
	TLog<TRNT_LOG_SETTINGS_NAME(MyLogName)>::Fatal("{} {}", "Hello", "world");

	// Or...

	using MyLog = TLog<TRNT_LOG_SETTINGS_NAME(MyLogName)>;

	MyLog::AddMessageHandlerCallback(MessageHandlerCallback);
	MyLog::Debug("{} {}", "Hello", "world");
	MyLog::Info("{} {}", "Hello", "world");
	MyLog::Success("{} {}", "Hello", "world");
	MyLog::Warning("{} {}", "Hello", "world");
	MyLog::Error("{} {}", "Hello", "world");
	MyLog::Fatal("{} {}", "Hello", "world");
}



*****************************************************************************************************************

*/


using TLogMessageHandlerCallback = void(*)(const TChar* TCMessage, TSize_T TCMessageLen, const TWChar* TWCMessage, TSize_T TWCMessageLen);

template<typename CTLogSettingsType>
class TLog
{
private:
	static constexpr const TChar* LogLevelToCString[6] = {
		"Debug",
		"Info",
		"Success",
		"Warning",
		"Error",
		"Fatal",
	};

private:
	static constexpr const TWChar* LogLevelToWCString[6] = {
		L"Debug",
		L"Info",
		L"Success",
		L"Warning",
		L"Error",
		L"Fatal",
	};

public:
	template<typename... FormatArgs>
	static TRNT_FORCE_INLINE void Debug(const TChar* FmtMessage, FormatArgs&&... Arguments)
	{
		ProcessMessageAndLog<TChar, TLogLevel::EDebug>(FmtMessage, Forward<FormatArgs>(Arguments)...);
	}

	template<typename... FormatArgs>
	static TRNT_FORCE_INLINE void Debug(const TWChar* FmtMessage, FormatArgs&&... Arguments)
	{
		ProcessMessageAndLog<TWChar, TLogLevel::EDebug>(FmtMessage, Forward<FormatArgs>(Arguments)...);
	}

	template<typename... FormatArgs>
	static TRNT_FORCE_INLINE void Info(const TChar* FmtMessage, FormatArgs&&... Arguments)
	{
		ProcessMessageAndLog<TChar, TLogLevel::EInfo>(FmtMessage, Forward<FormatArgs>(Arguments)...);
	}

	template<typename... FormatArgs>
	static TRNT_FORCE_INLINE void Info(const TWChar* FmtMessage, FormatArgs&&... Arguments)
	{
		ProcessMessageAndLog<TWChar, TLogLevel::EInfo>(FmtMessage, Forward<FormatArgs>(Arguments)...);
	}

	template<typename... FormatArgs>
	static TRNT_FORCE_INLINE void Success(const TChar* FmtMessage, FormatArgs&&... Arguments)
	{
		ProcessMessageAndLog<TChar, TLogLevel::ESuccess>(FmtMessage, Forward<FormatArgs>(Arguments)...);
	}

	template<typename... FormatArgs>
	static TRNT_FORCE_INLINE void Success(const TWChar* FmtMessage, FormatArgs&&... Arguments)
	{
		ProcessMessageAndLog<TWChar, TLogLevel::ESuccess>(FmtMessage, Forward<FormatArgs>(Arguments)...);
	}

	template<typename... FormatArgs>
	static TRNT_FORCE_INLINE void Warning(const TChar* FmtMessage, FormatArgs&&... Arguments)
	{
		ProcessMessageAndLog<TChar, TLogLevel::EWarning>(FmtMessage, Forward<FormatArgs>(Arguments)...);
	}

	template<typename... FormatArgs>
	static TRNT_FORCE_INLINE void Warning(const TWChar* FmtMessage, FormatArgs&&... Arguments)
	{
		ProcessMessageAndLog<TWChar, TLogLevel::EWarning>(FmtMessage, Forward<FormatArgs>(Arguments)...);
	}

	template<typename... FormatArgs>
	static TRNT_FORCE_INLINE void Error(const TChar* FmtMessage, FormatArgs&&... Arguments)
	{
		ProcessMessageAndLog<TChar, TLogLevel::EError>(FmtMessage, Forward<FormatArgs>(Arguments)...);
	}

	template<typename... FormatArgs>
	static TRNT_FORCE_INLINE void Error(const TWChar* FmtMessage, FormatArgs&&... Arguments)
	{
		ProcessMessageAndLog<TWChar, TLogLevel::EError>(FmtMessage, Forward<FormatArgs>(Arguments)...);
	}

	template<typename... FormatArgs>
	static TRNT_FORCE_INLINE void Fatal(const TChar* FmtMessage, FormatArgs&&... Arguments)
	{
		ProcessMessageAndLog<TChar, TLogLevel::EFatal>(FmtMessage, Forward<FormatArgs>(Arguments)...);
	}

	template<typename... FormatArgs>
	static TRNT_FORCE_INLINE void Fatal(const TWChar* FmtMessage, FormatArgs&&... Arguments)
	{
		ProcessMessageAndLog<TWChar, TLogLevel::EFatal>(FmtMessage, Forward<FormatArgs>(Arguments)...);
	}

private:
	using LogMessageHandlerList = TDoublyLinkedList<TLogMessageHandlerCallback>;

	static TMutex LogMutex;

	static LogMessageHandlerList MessageHandlers;

	template<typename CharType, TLogLevel LogLevel, typename... FormatArgs>
	static void ProcessMessageAndLog(const CharType* FmtMessage, FormatArgs&&... Arguments)
	{
		TRNT_ASSERT_AT_COMPILE_TIME(LogLevel >= TLogLevel::EDebug && LogLevel < TLogLevel::EMax);
		TRNT_ASSERT_AT_COMPILE_TIME(CTLogSettingsType::LogLevel >= TLogLevel::EDebug && CTLogSettingsType::LogLevel < TLogLevel::EMax);
		TRNT_ASSERT_AT_COMPILE_TIME((TAreTheSameType<CharType, TChar>::Value || TAreTheSameType<CharType, TWChar>::Value));

		if constexpr (LogLevel == TLogLevel::EFatal || LogLevel >= CTLogSettingsType::LogLevel)
		{
			fmt::basic_memory_buffer<CharType> MemoryBuffer = fmt::basic_memory_buffer<CharType>();
			std::chrono::time_point Now = std::chrono::system_clock::now();

			if constexpr (TAreTheSameType<CharType, TChar>::Value)
			{
				TString Message("[{:%Y-%m-%d %H:%M:%S}] [{}] {}: ");
				Message += FmtMessage;
				Message += "\n\0";

				fmt::detail::vformat_to(
					MemoryBuffer,
					fmt::string_view{ Message.GetData(), static_cast<TSize_T>(Message.Length()) },
					fmt::make_format_args(Now, CTLogSettingsType::LogNameAsCString, LogLevelToCString[static_cast<TUInt8>(LogLevel)], Arguments...));
			}
			else if constexpr (TAreTheSameType<CharType, TWChar>::Value)
			{
				TWString Message(L"[{:%Y-%m-%d %H:%M:%S}] [{}] {}: ");
				Message += FmtMessage;
				Message += L"\n\0";


				fmt::detail::vformat_to(
					MemoryBuffer,
					fmt::wstring_view{ Message.GetData(), static_cast<TSize_T>(Message.Length()) },
					fmt::make_wformat_args(Now, CTLogSettingsType::LogNameAsWCString, LogLevelToWCString[static_cast<TUInt8>(LogLevel)], Arguments...));
			}

			LogMutex.Lock();

			LogMessageHandlerList::NodeType* CurrentMessageHandler = MessageHandlers.GetHead();

			while (CurrentMessageHandler != nullptr)
			{
				if constexpr (TAreTheSameType<CharType, TChar>::Value)
				{
					CurrentMessageHandler->Value(MemoryBuffer.data(), MemoryBuffer.size(), nullptr, 0);
				}
				else if constexpr (TAreTheSameType<CharType, TWChar>::Value)
				{
					CurrentMessageHandler->Value(nullptr, 0, MemoryBuffer.data(), MemoryBuffer.size());
				}
				CurrentMessageHandler = CurrentMessageHandler->Next;
			}

			LogMutex.Unlock();
		}

		if constexpr (LogLevel == TLogLevel::EFatal)
		{
			::abort();
		}
	}

public:
	static void AddMessageHandlerCallback(TLogMessageHandlerCallback Callback)
	{
		MessageHandlers.InsertAtTail(new LogMessageHandlerList::NodeType(Callback));
	}

	static void RemoveMessageHandlerCallback(TLogMessageHandlerCallback Callback)
	{
		MessageHandlers.RemoveAt(MessageHandlers.Find(Callback));
	}

	static void Initialize()
	{
	
	}

	static void Destroy()
	{
		
	}

public:
	static void StdoutMessageHandlers(const TChar* TCMessage, TSize_T TCMessageLen, const TWChar* TWCMessage, TSize_T TWCMessageLen)
	{

	}

	static void FileMessageHandlers(const TChar* TCMessage, TSize_T TCMessageLen, const TWChar* TWCMessage, TSize_T TWCMessageLen)
	{

	}
};

template<typename CTLogSettingsType> TMutex TLog<CTLogSettingsType>::LogMutex;
template<typename CTLogSettingsType> typename TLog<CTLogSettingsType>::LogMessageHandlerList TLog<CTLogSettingsType>::MessageHandlers;