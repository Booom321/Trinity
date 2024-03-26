#pragma once

#include "DataTypes.h"

class TVersion
{
public:
	TUInt8 Major;
	TUInt8 Minor;
	TUInt8 Patch;

	TVersion() = default;

	~TVersion() = default;

	TVersion(TUInt8 Major, TUInt8 Minor, TUInt8 Patch)
		: Major(Major), Minor(Minor), Patch(Patch)
	{}
};