#pragma once

#include "Trinity/Core/Defines.h"
#include "DataTypes.h"

class TTrueType
{
public:
	static TRNT_CONSTEXPR TBool Value = true;
};

class TFalseType
{
public:
	static TRNT_CONSTEXPR TBool Value = false;
};

template<TBool Val>
class TBoolConstant
{
public:
	static TRNT_CONSTEXPR TBool Value = Val;
};