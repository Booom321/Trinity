#pragma once

#include <type_traits>

#include "Trinity/Core/Types/Base.h"

template<typename T>
class TIsCopyAssignable : public TBoolConstant<std::is_copy_assignable<T>::value>
{};
