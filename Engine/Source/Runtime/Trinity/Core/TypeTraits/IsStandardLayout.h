#pragma once

#include <type_traits>

#include "Trinity/Core/Types/Base.h"

template<typename T>
class TIsStandardLayout : public TBoolConstant<std::is_standard_layout<T>::value>
{};