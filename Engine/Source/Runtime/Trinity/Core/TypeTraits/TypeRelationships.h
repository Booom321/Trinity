#pragma once

#include "Trinity/Core/Types/Base.h"

#include <type_traits>

template<typename T, typename U>
class TAreTheSameType : public TBoolConstant<std::is_same<T, U>::value>
{
};

template<typename Base, typename Derived>
class TIsBaseOf : public TBoolConstant<std::is_base_of<Base, Derived>::value>
{
};

template<typename From, typename To>
class TIsConvertible : public TBoolConstant<std::is_convertible<From, To>::value>
{
};