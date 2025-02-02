#pragma once

#include "Trinity/Core/Types/Base.h"

#include <type_traits>

template<typename T>
class TMakeSigned
{
public:
	using Type = typename std::make_signed<T>::type;
};