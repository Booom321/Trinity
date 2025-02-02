#pragma once

#include "Trinity/Core/Types/Base.h"

#include <type_traits>

template<typename T>
class TIsStandardLayout : public TBoolConstant<std::is_standard_layout<T>::value>
{};