#pragma once

#include <memory>

#include "Trinity/Core/TypeTraits/RemoveReference.h"

template<typename T>
using TUniquePtr = std::unique_ptr<T>;

template<typename T, typename... ArgsType>
inline constexpr TUniquePtr<T> CreateUniquePtr(ArgsType&& ... Args)
{
	return std::make_unique<T>(Forward<ArgsType>(Args)...);
}