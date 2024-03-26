#pragma once

#include "IsScalar.h"

template<typename T>
class TIsZeroConstructType : public TBoolConstant<TIsScalar<T>::Value>
{
};
