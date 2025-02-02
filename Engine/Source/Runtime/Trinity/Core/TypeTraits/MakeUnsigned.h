#pragma once

#include "Trinity/Core/Types/Base.h"

#include <type_traits>

template<typename T>
class TMakeUnsigned
{
public:
	using Type = typename std::make_unsigned<T>::type;
};