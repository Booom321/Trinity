#pragma once

#include "Trinity/Core/Types/Base.h"
#include "Trinity/Core/Types/DataTypes.h"

#include "TypeRelationships.h"
#include "Trivial.h"
#include "RemoveCV.h"
#include "IsReference.h"

template <typename T, typename Arg>
class TIsBitwiseConstructible : public TFalseType
{
	static_assert(!TIsReferenceType<T>::Value && !TIsReferenceType<Arg>::Value);

	static_assert(TAreTheSameType<T, typename TRemoveCV<T>::Type>::Value && TAreTheSameType<Arg, typename TRemoveCV<Arg>::Type>::Value);
};

template <typename T>
class TIsBitwiseConstructible<T, T> : public TBoolConstant<TIsTriviallyCopyConstructible<T>::Value>
{
};

template <typename T, typename U>
class TIsBitwiseConstructible<const T, U> : public TIsBitwiseConstructible<T, U>
{
};

template <typename T>
class TIsBitwiseConstructible<const T*, T*> : public TTrueType
{
};

template <> class TIsBitwiseConstructible<TUInt8, TInt8> : public TTrueType
{};
template <> class TIsBitwiseConstructible<TInt8, TUInt8> : public TTrueType
{};
template <> class TIsBitwiseConstructible<TUInt16, TInt16> : public TTrueType
{};
template <> class TIsBitwiseConstructible<TInt16, TUInt16> : public TTrueType
{};
template <> class TIsBitwiseConstructible<TUInt32, TInt32> : public TTrueType
{};
template <> class TIsBitwiseConstructible<TInt32, TUInt32> : public TTrueType
{};
template <> class TIsBitwiseConstructible<TUInt64, TInt64> : public TTrueType
{};
template <> class TIsBitwiseConstructible<TInt64, TUInt64> : public TTrueType
{};