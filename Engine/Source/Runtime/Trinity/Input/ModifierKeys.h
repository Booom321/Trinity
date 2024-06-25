#pragma once

enum class TModifierKey : signed char
{
	EUnknown = -1,
	EShift = 0x0001,
	EControl = 0x0002,
	EAlt = 0x0004,
	ESuper = 0x0008,
	ECapsLock = 0x0010,
	ENumLock = 0x0020,
};