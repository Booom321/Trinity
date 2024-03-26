#pragma once

#include <type_traits>

template<typename T>
class TDecay
{
public:
	using Type = typename std::decay<T>::type;
};