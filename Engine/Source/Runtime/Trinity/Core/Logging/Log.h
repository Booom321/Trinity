#pragma once

#include <fmt/chrono.h>
#include <fmt/xchar.h>

#include "Trinity/Core/Threading/Mutex.h"
#include "Trinity/Core/Containers/LinkedList.h"
#include "Trinity/Core/String/String.h"
#include "Trinity/Core/TypeTraits/TypeRelationships.h"

#include "LogLevel.h"

template<const TChar* LogNameCharArray, const TWChar* LogNameWCharArray, TLogLevel Level>
class TCompileTimeLogInfo
{
public:
	static TRNT_CONSTEXPR const TChar*  LogNameAsCString	= LogNameCharArray;
	static TRNT_CONSTEXPR const TWChar* LogNameAsWCString	= LogNameWCharArray;
	static TRNT_CONSTEXPR TLogLevel     LogLevel			= Level;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define TRNT_MAKE_LOG_INFO_NAME(LogName) T##LogName##LogInfo

#define TRNT_DECLARE_LOG_INFO(LogName, LogLevel) \
	static TRNT_CONSTEXPR const TChar LogName##CharArray[] = TRNT_STRINGIFY(LogName);\
	static TRNT_CONSTEXPR const TWChar LogName##WCharArray[] = L#LogName;\
	class TRNT_MAKE_LOG_INFO_NAME(LogName) : public TCompileTimeLogInfo<LogName##CharArray, LogName##WCharArray, LogLevel>\
	{}

#define TRNT_GET_LOG_INFO(LogName) TRNT_MAKE_LOG_INFO_NAME(LogName)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TRNT_DECLARE_LOG_INFO(Default, TLogLevel::EDebug);

using TLogMessageHandlerCallback = void(*)(TLogLevel LogLevel, const TChar* FormattedMessageA, TSize_T FormattedMessageALen, const TWChar* FormattedMessageW, TSize_T FormattedMessageWLen);

class TRNT_API TLog
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
	template<typename CTLogInfoType, typename... FormatArgs>
	static TRNT_FORCE_INLINE void Debug(const TChar* FmtMessage, FormatArgs&&... Arguments)
	{
		ProcessMessageAndLog<CTLogInfoType, TChar, TLogLevel::EDebug>(FmtMessage, Forward<FormatArgs>(Arguments)...);
	}

	template<typename CTLogInfoType, typename... FormatArgs>
	static TRNT_FORCE_INLINE void Debug(const TWChar* FmtMessage, FormatArgs&&... Arguments)
	{
		ProcessMessageAndLog<CTLogInfoType, TWChar, TLogLevel::EDebug>(FmtMessage, Forward<FormatArgs>(Arguments)...);
	}

	template<typename CTLogInfoType, typename... FormatArgs>
	static TRNT_FORCE_INLINE void Info(const TChar* FmtMessage, FormatArgs&&... Arguments)
	{
		ProcessMessageAndLog<CTLogInfoType, TChar, TLogLevel::EInfo>(FmtMessage, Forward<FormatArgs>(Arguments)...);
	}

	template<typename CTLogInfoType, typename... FormatArgs>
	static TRNT_FORCE_INLINE void Info(const TWChar* FmtMessage, FormatArgs&&... Arguments)
	{
		ProcessMessageAndLog<CTLogInfoType, TWChar, TLogLevel::EInfo>(FmtMessage, Forward<FormatArgs>(Arguments)...);
	}

	template<typename CTLogInfoType, typename... FormatArgs>
	static TRNT_FORCE_INLINE void Success(const TChar* FmtMessage, FormatArgs&&... Arguments)
	{
		ProcessMessageAndLog<CTLogInfoType, TChar, TLogLevel::ESuccess>(FmtMessage, Forward<FormatArgs>(Arguments)...);
	}

	template<typename CTLogInfoType, typename... FormatArgs>
	static TRNT_FORCE_INLINE void Success(const TWChar* FmtMessage, FormatArgs&&... Arguments)
	{
		ProcessMessageAndLog<CTLogInfoType, TWChar, TLogLevel::ESuccess>(FmtMessage, Forward<FormatArgs>(Arguments)...);
	}

	template<typename CTLogInfoType, typename... FormatArgs>
	static TRNT_FORCE_INLINE void Warning(const TChar* FmtMessage, FormatArgs&&... Arguments)
	{
		ProcessMessageAndLog<CTLogInfoType, TChar, TLogLevel::EWarning>(FmtMessage, Forward<FormatArgs>(Arguments)...);
	}

	template<typename CTLogInfoType, typename... FormatArgs>
	static TRNT_FORCE_INLINE void Warning(const TWChar* FmtMessage, FormatArgs&&... Arguments)
	{
		ProcessMessageAndLog<CTLogInfoType, TWChar, TLogLevel::EWarning>(FmtMessage, Forward<FormatArgs>(Arguments)...);
	}

	template<typename CTLogInfoType, typename... FormatArgs>
	static TRNT_FORCE_INLINE void Error(const TChar* FmtMessage, FormatArgs&&... Arguments)
	{
		ProcessMessageAndLog<CTLogInfoType, TChar, TLogLevel::EError>(FmtMessage, Forward<FormatArgs>(Arguments)...);
	}

	template<typename CTLogInfoType, typename... FormatArgs>
	static TRNT_FORCE_INLINE void Error(const TWChar* FmtMessage, FormatArgs&&... Arguments)
	{
		ProcessMessageAndLog<CTLogInfoType, TWChar, TLogLevel::EError>(FmtMessage, Forward<FormatArgs>(Arguments)...);
	}

	template<typename CTLogInfoType, typename... FormatArgs>
	static TRNT_FORCE_INLINE void Fatal(const TChar* FmtMessage, FormatArgs&&... Arguments)
	{
		ProcessMessageAndLog<CTLogInfoType, TChar, TLogLevel::EFatal>(FmtMessage, Forward<FormatArgs>(Arguments)...);
	}

	template<typename CTLogInfoType, typename... FormatArgs>
	static TRNT_FORCE_INLINE void Fatal(const TWChar* FmtMessage, FormatArgs&&... Arguments)
	{
		ProcessMessageAndLog<CTLogInfoType, TWChar, TLogLevel::EFatal>(FmtMessage, Forward<FormatArgs>(Arguments)...);
	}

private:
	using LogMessageHandlerList = TDoublyLinkedList<TLogMessageHandlerCallback>;

	static TMutex LogMutex;

	static FILE* OutputLogFile;

	static LogMessageHandlerList MessageHandlers;

private:
	template<typename CTLogInfoType, typename CharType, TLogLevel LogLevel, typename... FormatArgs>
	static void ProcessMessageAndLog(const CharType* FmtMessage, FormatArgs&&... Arguments)
	{
		TRNT_ASSERT_AT_COMPILE_TIME(LogLevel >= TLogLevel::EDebug && LogLevel < TLogLevel::EMax);
		TRNT_ASSERT_AT_COMPILE_TIME(CTLogInfoType::LogLevel >= TLogLevel::EDebug && CTLogInfoType::LogLevel < TLogLevel::EMax);
		TRNT_ASSERT_AT_COMPILE_TIME((TAreTheSameType<CharType, TChar>::Value || TAreTheSameType<CharType, TWChar>::Value));

		if constexpr (LogLevel >= CTLogInfoType::LogLevel || LogLevel == TLogLevel::EFatal)
		{
			fmt::basic_memory_buffer<CharType> MemoryBuffer = fmt::basic_memory_buffer<CharType>();
			std::chrono::time_point Now = std::chrono::system_clock::now();

			if constexpr (TAreTheSameType<CharType, TChar>::Value)
			{
				TString Message("[{:%Y-%m-%d %H:%M:%S}] [{}] {}: ");
				Message += FmtMessage;
				Message += '\n';

				fmt::detail::vformat_to(
					MemoryBuffer,
					fmt::string_view{ Message.GetData(), static_cast<TSize_T>(Message.GetElementCount()) },
					fmt::make_format_args(Now, CTLogInfoType::LogNameAsCString, LogLevelToCString[static_cast<TUInt8>(LogLevel)], Arguments...));

				MemoryBuffer.push_back('\0');
			}
			else if constexpr (TAreTheSameType<CharType, TWChar>::Value)
			{
				TWString Message(L"[{:%Y-%m-%d %H:%M:%S}] [{}] {}: ");
				Message += FmtMessage;
				Message += L'\n';

				fmt::detail::vformat_to(
					MemoryBuffer,
					fmt::wstring_view{ Message.GetData(), static_cast<TSize_T>(Message.GetElementCount()) },
					fmt::make_wformat_args(Now, CTLogInfoType::LogNameAsWCString, LogLevelToWCString[static_cast<TUInt8>(LogLevel)], Arguments...));
				
				MemoryBuffer.push_back(L'\0');
			}

			LogMutex.Lock();

			LogMessageHandlerList::NodeType* CurrentMessageHandler = MessageHandlers.GetHead();

			while (CurrentMessageHandler != nullptr)
			{
				if constexpr (TAreTheSameType<CharType, TChar>::Value)
				{
					CurrentMessageHandler->Value(LogLevel, MemoryBuffer.data(), MemoryBuffer.size() - 1, nullptr, 0);
				}
				else if constexpr (TAreTheSameType<CharType, TWChar>::Value)
				{
					CurrentMessageHandler->Value(LogLevel, nullptr, 0, MemoryBuffer.data(), MemoryBuffer.size() - 1);
				}
				CurrentMessageHandler = CurrentMessageHandler->Next;
			}

			LogMutex.Unlock();
		}

		if constexpr (LogLevel == TLogLevel::EFatal)
		{	
			CloseLogFile();
			::abort();
		}
	}

public:
	template<typename Function>
	static void AddMessageHandlerCallback(Function Callback)
	{
		static_assert(!TIsClass<Function>::Value, "Lambda functions are not allowed!");
		static_assert(TAreTheSameType<Function, TLogMessageHandlerCallback>::Value);
		
		LogMutex.Lock();
		if (!MessageHandlers.Contains(Callback))
		{
			MessageHandlers.InsertAtTail(new LogMessageHandlerList::NodeType(Callback));
		}
		LogMutex.Unlock();
	}
	
	template<typename Function>
	static void RemoveMessageHandlerCallback(Function Callback)
	{
		static_assert(!TIsClass<Function>::Value, "Lambda functions are not allowed!");
		static_assert(TAreTheSameType<Function, TLogMessageHandlerCallback>::Value);

		LogMutex.Lock();
		typename LogMessageHandlerList::NodeType* Found = MessageHandlers.Find(Callback);
		if (Found)
		{
			MessageHandlers.RemoveAt(Found);
		}
		LogMutex.Unlock();
	}
	
	static FILE* OpenLogFile(const TChar* Filename, TBool EnableToAppend = false);

	static void CloseLogFile();

	static void Initialize();

	static void Shutdown();

	static void ClearAllMessageHandlerCallbacks();

public:
	static void DefaultStdoutMessageHandler(TLogLevel LogLevel, const TChar* FormattedMessageA, TSize_T FormattedMessageALen, const TWChar* FormattedMessageW, TSize_T FormattedMessageWLen);

	static void DefaultFileMessageHandler(TLogLevel LogLevel, const TChar* FormattedMessageA, TSize_T FormattedMessageALen, const TWChar* FormattedMessageW, TSize_T FormattedMessageWLen);
	
	static TRNT_FORCE_INLINE FILE* GetOutputLogFile()
	{
		return OutputLogFile;
	}
};