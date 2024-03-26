#pragma once

#include <type_traits>

#include "Trinity/Core/Types/Base.h"

template<typename T>
class TIsCopyConstructible : public TBoolConstant<std::is_copy_constructible<T>::value>
{};