#pragma once

#include <type_traits>

#include "Trinity/Core/Types/Base.h"

template<typename T>
class TIsScalar : public TBoolConstant<std::is_scalar<T>::value>
{
};