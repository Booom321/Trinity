#pragma once

#include "Trinity/Core/Types/DataTypes.h"

template<typename T>
class TRemoveExtent
{
public:
	using Type = T;
};

template<typename T>
class TRemoveExtent<T[]>
{
public:
	using Type = T;
};

template<typename T, TUInt32 N>
class TRemoveExtent<T[N]>
{
public:
	using Type = T;
};