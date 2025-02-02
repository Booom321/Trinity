#pragma once

#include "Trinity/Core/Types/Base.h"

#include <type_traits>

template<typename T>
class TIsCopyConstructible : public TBoolConstant<std::is_copy_constructible<T>::value>
{};