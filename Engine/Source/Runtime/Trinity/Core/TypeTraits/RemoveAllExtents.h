#pragma once

#include "Trinity/Core/Types/DataTypes.h"

template<typename T>
class TRemoveAllExtents 
{ 
public:
    using Type = T;
};

template<typename T>
class TRemoveAllExtents<T[]> 
{
public:
    using Type = typename TRemoveAllExtents<T>::Type;
};

template<typename T, TSize_T N>
class TRemoveAllExtents<T[N]> 
{
public:
    using Type = typename TRemoveAllExtents<T>::Type;
};