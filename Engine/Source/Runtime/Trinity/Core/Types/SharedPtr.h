#pragma once

#include <memory>

#include "Trinity/Core/TypeTraits/RemoveReference.h"

template<typename T>
using TSharedPtr = std::shared_ptr<T>;

template<typename T, typename... ArgsType>
inline constexpr TSharedPtr<T> CreateSharedPtr(ArgsType&& ... Args)
{
	return std::make_shared<T>(Forward<ArgsType>(Args)...);
}