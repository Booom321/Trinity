#pragma once

#include "Trinity/Core/Defines.h"
#include "Trinity/Core/TypeTraits/PrimaryTypes.h"

template<
	typename LhsType,
	typename RhsType, 
	bool = TIsEmpty<LhsType>::Value && !TIsFinal<LhsType>::Value
>
class TCompressedPair final : private LhsType
{
public:
    RhsType SecondValue;

    using Base = LhsType;

    template<typename TEnableIf<std::conjunction<std::is_default_constructible<LhsType>, std::is_default_constructible<RhsType>>::value, int>::Type = 0>
    TRNT_FORCE_INLINE TCompressedPair() : LhsType(), SecondValue() 
    {}

    template<typename TEnableIf<std::conjunction<std::is_copy_constructible<LhsType>, std::is_copy_constructible<RhsType>>::value, int>::Type = 0>
    TRNT_FORCE_INLINE TCompressedPair(const LhsType& Value1, const RhsType& Value2) : LhsType(Value1), SecondValue(Value2)
    {}

    template<typename TEnableIf<std::conjunction<std::is_move_constructible<LhsType>, std::is_move_constructible<RhsType>>::value, int>::Type = 0>
    TRNT_FORCE_INLINE TCompressedPair(LhsType&& Value1, RhsType&& Value2) noexcept : LhsType(Move(Value1)), SecondValue(Move(Value2))
    {}

    TRNT_FORCE_INLINE LhsType& GetFirstValue() noexcept { return *this; }

    TRNT_FORCE_INLINE RhsType& GetSecondValue() noexcept { return SecondValue; }

    TRNT_FORCE_INLINE const LhsType& GetFirstValue() const noexcept { return *this; }

    TRNT_FORCE_INLINE const RhsType& GetSecondValue() const noexcept { return SecondValue; }
};

template <typename LhsType, typename RhsType>
class TCompressedPair<LhsType, RhsType, false> final
{
public:
    LhsType FirstValue;
    RhsType SecondValue;

    using Base = LhsType;

    template<typename TEnableIf<std::conjunction<std::is_default_constructible<LhsType>, std::is_default_constructible<RhsType>>::value, int>::Type = 0>
    TRNT_FORCE_INLINE TCompressedPair() : FirstValue(), SecondValue()
    {}

    template<typename TEnableIf<std::conjunction<std::is_copy_constructible<LhsType>, std::is_copy_constructible<RhsType>>::value, int>::Type = 0>
    TRNT_FORCE_INLINE TCompressedPair(const LhsType& Value1, const RhsType& Value2) : FirstValue(Value1), SecondValue(Value2)
    {}

    template<typename TEnableIf<std::conjunction<std::is_move_constructible<LhsType>, std::is_move_constructible<RhsType>>::value, int>::Type = 0>
    TRNT_FORCE_INLINE TCompressedPair(LhsType&& Value1, RhsType&& Value2) noexcept : FirstValue(Move(Value1)), SecondValue(Move(Value2))
    {}

    TRNT_FORCE_INLINE LhsType& GetFirstValue() noexcept { return FirstValue; }

    TRNT_FORCE_INLINE RhsType& GetSecondValue() noexcept { return SecondValue; }

    TRNT_FORCE_INLINE const LhsType& GetFirstValue() const noexcept { return FirstValue; }

    TRNT_FORCE_INLINE const RhsType& GetSecondValue() const noexcept { return SecondValue; }
};