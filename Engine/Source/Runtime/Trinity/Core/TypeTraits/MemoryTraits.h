#pragma once

#include "IsStandardLayout.h"
#include "PrimaryTypes.h"
#include "Trivial.h"

template<typename T>
class TIsMemcpyCompatible : public TBoolConstant<TIsTriviallyCopyable<T>::Value>
{};

template<typename T>
class TIsMemmoveCompatible : public TBoolConstant<TIsTriviallyCopyable<T>::Value>
{};

template<typename T>
class TIsMemcmpCompatible : public TBoolConstant<TOr<TIsEnum<T>, TIsArithmetic<T>, TIsPointer<T>>::Value>
{};

template<typename T>
class TIsMemsetCompatible : public TBoolConstant<TIsTriviallyCopyable<T>::Value || TOr<TIsEnum<T>, TIsArithmetic<T>, TIsPointer<T>>::Value>
{};