#pragma once

#include "Trinity/Core/Types/Base.h"

#include <type_traits>

template<typename T>
class TIsCopyAssignable : public TBoolConstant<std::is_copy_assignable<T>::value>
{};
