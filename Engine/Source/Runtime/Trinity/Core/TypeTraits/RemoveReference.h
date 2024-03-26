#pragma once

#include <type_traits>

template<typename T> class TRemoveReference			{ public: using Type = T; };
template<typename T> class TRemoveReference<T&>		{ public: using Type = T; };
template<typename T> class TRemoveReference<T&&>	{ public: using Type = T; };

template<typename T>
constexpr typename TRemoveReference<T>::Type&& Move(T&& Obj) noexcept
{
	return static_cast<typename TRemoveReference<T>::Type&&>(Obj);
}

template<typename T>
constexpr T&& Forward(typename TRemoveReference<T>::Type& Obj) noexcept
{
	return static_cast<T&&>(Obj);
}

template<typename T>
constexpr T&& Forward(typename TRemoveReference<T>::Type&& Obj) noexcept
{
	static_assert(!std::is_lvalue_reference<T>::value, "Can not forward an rvalue as an lvalue.");
	return static_cast<T&&>(Obj);
}