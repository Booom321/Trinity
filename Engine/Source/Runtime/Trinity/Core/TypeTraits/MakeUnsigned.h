#pragma once

#include <type_traits>

#include "Trinity/Core/Types/Base.h"

template<typename T>
class TMakeUnsigned
{
public:
	using Type = typename std::make_unsigned<T>::type;
};