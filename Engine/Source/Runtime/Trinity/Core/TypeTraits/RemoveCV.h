#pragma once

#include <type_traits>

template<typename T>
class TRemoveCV
{
public:
	using Type = T;
};

template<typename T>
class TRemoveCV<const T>
{
public:
	using Type = T;
};

template<typename T>
class TRemoveCV<volatile T>
{
public:
	using Type = T;
};

template<typename T>
class TRemoveCV<const volatile T>
{
public:
	using Type = T;
};