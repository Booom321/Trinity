#pragma once

#include "Trinity/Core/Types/Base.h"
#include "Trinity/Core/Types/DataTypes.h"

#include "Logical.h"
#include "RemoveCVRef.h"
#include "TypeRelationships.h"

#include <type_traits>

template <typename T>
class TIsFloatingPoint : public TFalseType
{};

template <> class TIsFloatingPoint<float> : public TTrueType
{};

template <> class TIsFloatingPoint<double> : public TTrueType
{};

template <> class TIsFloatingPoint<long double> : public TTrueType
{};

template <typename T> class TIsFloatingPoint<const T> : public TBoolConstant<TIsFloatingPoint<T>::Value>
{};

template <typename T> class TIsFloatingPoint<volatile T> : public TBoolConstant<TIsFloatingPoint<T>::Value>
{};

template <typename T> class TIsFloatingPoint<const volatile T> : public TBoolConstant<TIsFloatingPoint<T>::Value>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
class TIsIntegral : public TFalseType
{};

template <> class TIsIntegral<bool> : public TTrueType
{};

template <> class TIsIntegral<char> : public TTrueType
{};

template <> class TIsIntegral<signed char> : public TTrueType
{};

template <> class TIsIntegral<unsigned char> : public TTrueType
{};

template <> class TIsIntegral<char16_t> : public TTrueType
{};

template <> class TIsIntegral<char32_t> : public TTrueType
{};

#ifdef __cpp_char8_t
template <> class TIsIntegral<char8_t> : public TTrueType
{};
#endif // __cpp_char8_t


template <> class TIsIntegral<wchar_t> : public TTrueType
{};

template <> class TIsIntegral<short> : public TTrueType
{};

template <> class TIsIntegral<unsigned short> : public TTrueType
{};

template <> class TIsIntegral<int> : public TTrueType
{};

template <> class TIsIntegral<unsigned int> : public TTrueType
{};

template <> class TIsIntegral<long> : public TTrueType
{};

template <> class TIsIntegral<unsigned long> : public TTrueType
{};

template <> class TIsIntegral<long long> : public TTrueType
{};

template <> class TIsIntegral<unsigned long long> : public TTrueType
{};

template <typename T> class TIsIntegral<const T> : public TBoolConstant<TIsIntegral<T>::Value>
{};

template <typename T> class TIsIntegral<volatile T> : public TBoolConstant<TIsIntegral<T>::Value>
{};

template <typename T> class TIsIntegral<const volatile T> : public TBoolConstant<TIsIntegral<T>::Value>
{};

///////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
class TIsArithmetic : public TBoolConstant<TIsFloatingPoint<T>::Value || TIsIntegral<T>::Value> 
{
};

////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
class TIsArray : public TFalseType {};

template<typename T>
class TIsArray<T[]> : public TTrueType {};

template<typename T, TSize_T N>
class TIsArray<T[N]> : public TTrueType {};

////////////////////////////////////////////////////////////////////////////////////////////////

template<typename>
class TIsFunction : public TFalseType {};

template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args...)> : public TTrueType {};

template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args......)> : public TTrueType {};

template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args...) const> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args...) volatile> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args...) const volatile> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args......) const> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args......) volatile> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args......) const volatile> : public TTrueType {};

template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args...)&> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args...) const&> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args...) volatile&> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args...) const volatile&> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args......)&> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args......) const&> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args......) volatile&> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args......) const volatile&> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args...)&&> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args...) const&&> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args...) volatile&&> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args...) const volatile&&> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args......)&&> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args......) const&&> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args......) volatile&&> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args......) const volatile&&> : public TTrueType {};

template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args...) noexcept> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args......) noexcept> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args...) const noexcept> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args...) volatile noexcept> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args...) const volatile noexcept> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args......) const noexcept> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args......) volatile noexcept> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args......) const volatile noexcept> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args...) & noexcept> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args...) const& noexcept> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args...) volatile& noexcept> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args...) const volatile& noexcept> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args......) & noexcept> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args......) const& noexcept> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args......) volatile& noexcept> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args......) const volatile& noexcept> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args...) && noexcept> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args...) const&& noexcept> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args...) volatile&& noexcept> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args...) const volatile&& noexcept> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args......) && noexcept> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args......) const&& noexcept> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args......) volatile&& noexcept> : public TTrueType {};
template<typename ReturnType, typename... Args>
class TIsFunction<ReturnType(Args......) const volatile&& noexcept> : public TTrueType {};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
class TIsEnum : public TBoolConstant<std::is_enum<T>::value>
{};

template<typename T>
class TIsPointer : public TFalseType
{};

template <typename T> 
class TIsPointer<T*> : public TTrueType
{};

template <typename T> 
class TIsPointer<const T*> : public TTrueType
{};

template <typename T> 
class TIsPointer<volatile T*> : public TTrueType
{};

template <typename T> 
class TIsPointer<const volatile T*> : public TTrueType
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
class TIsClass : public TBoolConstant<std::is_class<T>::value>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename>
class TIsUnboundedArray : public TFalseType
{
};

template<typename T>
class TIsUnboundedArray<T[]> : public TTrueType
{
};

template<typename>
class TIsBoundedArray : public TFalseType
{
};

template<typename T, TSize_T N>
class TIsBoundedArray<T[N]> : public TTrueType
{
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
class TIsCharacterType
{
public:
	using CharacterType = typename TRemoveCVRef<T>::Type;

	static TRNT_CONSTEXPR bool Value = TOr<
		TAreTheSameType<CharacterType, char>,
		TAreTheSameType<CharacterType, wchar_t>,
#ifdef __cpp_char8_t
		TAreTheSameType<CharacterType, char8_t>,
#endif
		TAreTheSameType<CharacterType, char16_t>,
		TAreTheSameType<CharacterType, char32_t>
	>::Value;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
class TIsEmpty : public TBoolConstant<std::is_empty<T>::value>
{};

template<typename T>
class TIsFinal : public TBoolConstant<std::is_final<T>::value>
{};