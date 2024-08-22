#pragma once

#include "Trinity/Core/Defines.h"
#include "PrimaryTypes.h"

// Check to see if the Trinity Engine supports this char type
template<typename T>
class TIsCharTypeSupported : public TBoolConstant<TIsCharTypeSupported<typename TRemoveCVRef<T>::Type>::Value>  // char8_t, char16_t and char32_t are not supported
{
public:
	static_assert(TIsCharacterType<T>::Value, "T is not a character type.");
};

template<>
class TIsCharTypeSupported<char> : public TTrueType
{};

template<>
class TIsCharTypeSupported<wchar_t> : public TTrueType
{};

#ifdef __cpp_char8_t

template<>
class TIsCharTypeSupported<char8_t> : public TFalseType
{};

#endif // DEBUG

template<>
class TIsCharTypeSupported<char16_t> : public TFalseType
{};

template<>
class TIsCharTypeSupported<char32_t> : public TFalseType
{};
