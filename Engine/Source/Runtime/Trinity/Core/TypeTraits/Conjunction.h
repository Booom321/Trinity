#pragma once

#include "Trinity/Core/Types/Base.h"

#include <type_traits>

template<TBool Value, typename First, typename... Rest>
class TPrivateConjunction
{
public:
	using Type = First;
};

template<typename True, typename Next, typename... Rest>
class TPrivateConjunction<true, True, Next, Rest...>
{
public:
	using Type = typename TPrivateConjunction<Next::Value, Next, Rest...>::Type;
};

template<typename... Traits>
class TConjunction : public TTrueType {};

template<typename First, typename... Rest>
class TConjunction<First, Rest...> : public TPrivateConjunction<First::Value, First, Rest...>::Type
{
};