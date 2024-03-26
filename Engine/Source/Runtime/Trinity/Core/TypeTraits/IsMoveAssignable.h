#pragma once

#include <type_traits>

#include "Trinity/Core/Types/Base.h"

template<typename T>
class TIsMoveAssignable : public TBoolConstant<std::is_move_assignable<T>::value>
{};