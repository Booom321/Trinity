#pragma once

#include "Trinity/Core/Types/Base.h"

template <typename... Types>
class TAnd;

template <bool LhsValue, typename... Rhs>
class TAndValue : public TBoolConstant<TAnd<Rhs...>::Value>
{
};

template <typename... Rhs>
class TAndValue<false, Rhs...> : public TFalseType
{
};

template <typename Lhs, typename... Rhs>
class TAnd<Lhs, Rhs...> : public TAndValue<Lhs::Value, Rhs...>
{
};

template <>
class TAnd<> : public TTrueType
{
};
///////////////////////////////////////////////////////////////////////////////////
template <typename... Types>
class TOr;

template <bool LhsValue, typename... Rhs>
class TOrValue : public TBoolConstant<TOr<Rhs...>::Value>
{
};

template <typename... Rhs>
class TOrValue<true, Rhs...> : public TTrueType
{
};

template <typename Lhs, typename... Rhs>
class TOr<Lhs, Rhs...> : public TOrValue<Lhs::Value, Rhs...>
{
};

template <>
class TOr<> : public TFalseType
{
};
///////////////////////////////////////////////////////////////////////////////////
template <typename Type>
class TNot : public TBoolConstant<!Type::Value>
{
};