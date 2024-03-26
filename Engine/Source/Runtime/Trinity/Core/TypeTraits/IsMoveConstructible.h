#pragma once

#include <type_traits>

#include "Trinity/Core/Types/Base.h"

template<typename T>
class TIsMoveConstructible : public TBoolConstant<std::is_move_constructible<T>::value>
{};