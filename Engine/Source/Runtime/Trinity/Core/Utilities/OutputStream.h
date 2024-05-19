#pragma once

#include <fmt/core.h>
#include <fmt/chrono.h>
#include <fmt/xchar.h>

#include "Trinity/Core/Defines.h"
#include "Trinity/Core/TypeTraits/RemoveReference.h"

template<typename ... Arguments>
TRNT_INLINE void Printf(std::FILE* OutputFile, fmt::string_view FmtString, Arguments&&... Args)
{
	const auto& FormatArgs = fmt::make_format_args(Args...);

	if constexpr (!fmt::detail::is_utf8())
	{
		return fmt::detail::vprint_mojibake(OutputFile, FmtString, FormatArgs);
	}
	else
	{
		if constexpr (fmt::detail::is_locking<Arguments...>())
		{
			return fmt::vprint(OutputFile, FmtString, FormatArgs);
		}
		else
		{
			return fmt::vprint_locked(OutputFile, FmtString, FormatArgs);
		}
	}
}

template<typename ... Arguments>
TRNT_INLINE void Printf(fmt::string_view FmtString, Arguments&&... Args)
{
	const auto& FormatArgs = fmt::make_format_args(Args...);

	if constexpr (!fmt::detail::is_utf8())
	{
		return fmt::detail::vprint_mojibake(stdout, FmtString, FormatArgs);
	}
	else
	{
		if constexpr (fmt::detail::is_locking<Arguments...>())
		{
			return fmt::vprint(stdout, FmtString, FormatArgs);
		}
		else
		{
			return fmt::vprint_locked(stdout, FmtString, FormatArgs);
		}
	}
}

template<typename ... Arguments>
TRNT_INLINE void Printf(std::FILE* OutputFile, fmt::wstring_view FmtString, Arguments&&... Args)
{
	auto MemoryBuffer = fmt::wmemory_buffer();
	auto FormatArgs = fmt::make_wformat_args(Args...);

	fmt::detail::vformat_to(MemoryBuffer, FmtString, FormatArgs);
	MemoryBuffer.push_back(L'\0');

	std::fputws(MemoryBuffer.data(), OutputFile);
}

template<typename ... Arguments>
TRNT_INLINE void Printf(fmt::wstring_view FmtString, Arguments&&... Args)
{
	auto MemoryBuffer = fmt::wmemory_buffer();
	auto FormatArgs = fmt::make_wformat_args(Args...);

	fmt::detail::vformat_to(MemoryBuffer, FmtString, FormatArgs);
	MemoryBuffer.push_back(L'\0');

	std::fputws(MemoryBuffer.data(), stdout);
}