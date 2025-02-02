#pragma once

#include "Trinity/Core/Types/Base.h"

#include <type_traits>

template<typename T>
class TIsMoveAssignable : public TBoolConstant<std::is_move_assignable<T>::value>
{};