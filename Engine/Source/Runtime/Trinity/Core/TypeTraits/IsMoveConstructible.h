#pragma once

#include "Trinity/Core/Types/Base.h"

#include <type_traits>

template<typename T>
class TIsMoveConstructible : public TBoolConstant<std::is_move_constructible<T>::value>
{};