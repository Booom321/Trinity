#pragma once

#include "Trinity/Core/Types/DataTypes.h"

enum class TInputAction : TInt8
{
	EUnknown = -1,
	EPress,
	ERelease,
	ERepeat,
};