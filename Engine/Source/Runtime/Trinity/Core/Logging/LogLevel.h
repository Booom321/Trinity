#pragma once

enum class TLogLevel : unsigned char
{
	EDebug = 0,
	EInfo,
	ESuccess,
	EWarning,
	EError,
	EFatal,
	EMax
};