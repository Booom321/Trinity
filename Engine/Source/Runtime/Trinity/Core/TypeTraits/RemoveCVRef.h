#pragma once

#include "RemoveCV.h"
#include "RemoveReference.h"

template<typename T>
class TRemoveCVRef
{
public:
	using Type = typename TRemoveCV<typename TRemoveReference<T>::Type>::Type;
};