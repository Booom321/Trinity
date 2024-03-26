#pragma once

template <bool Predicate, typename Result = void>
class TEnableIf;

template <typename Result>
class TEnableIf<true, Result>
{
public:
	using Type = Result;
};

template <typename Result>
class TEnableIf<false, Result>
{};