#pragma once

#include "Trinity/Core/Types/Base.h"

#include <type_traits>

template<typename T>
class TIsTriviallyDefaultConstructible : public TBoolConstant<std::is_trivially_default_constructible<T>::value>
{};

template<typename T>
class TIsTriviallyDestructible : public TBoolConstant<std::is_trivially_destructible<T>::value>
{};

template<typename T>
class TIsTriviallyCopyConstructible : public TBoolConstant<std::is_trivially_copy_constructible<T>::value>
{};

template<typename T>
class TIsTriviallyCopyAssignable : public TBoolConstant<std::is_trivially_copy_assignable<T>::value>
{};

template<typename T>
class TIsTriviallyMoveConstructible : public TBoolConstant<std::is_trivially_move_constructible<T>::value>
{};

template<typename T>
class TIsTriviallyMoveAssignable : public TBoolConstant<std::is_trivially_move_assignable<T>::value>
{};

template<typename T>
class TIsTrivial : public TBoolConstant<std::is_trivial<T>::value>
{};

template<typename T>
class TIsTriviallyCopyable : public TBoolConstant<std::is_trivially_copyable<T>::value>
{};