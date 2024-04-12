#pragma once

#include <type_traits>

#include "Trinity/Core/Types/Base.h"

template<typename T>
class TMakeSigned
{
public:
	using Type = typename std::make_signed<T>::type;
};