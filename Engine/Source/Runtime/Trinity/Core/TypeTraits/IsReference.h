#pragma once

#include "Trinity/Core/Types/Base.h"

template<class T> class TIsReferenceType		: public TFalseType {};
template<class T> class TIsReferenceType<T&>	: public TTrueType {};
template<class T> class TIsReferenceType<T&&>	: public TTrueType {};