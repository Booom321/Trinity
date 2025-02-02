#pragma once

#include "Trinity/Core/Types/Base.h"

#include <type_traits>

template<typename T>
class TIsScalar : public TBoolConstant<std::is_scalar<T>::value>
{};