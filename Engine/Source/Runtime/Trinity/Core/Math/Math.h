#pragma once

#include "Trinity/Core/Defines.h"
#include "Trinity/Core/TypeTraits/PrimaryTypes.h"

#include "MathConstants.h"

class TMath
{
public:
	template<typename NumberType>
	static TRNT_CONSTEXPR TRNT_FORCE_INLINE NumberType Abs(NumberType X)
	{
		return (X >= 0) ? X : -X;
	}

	template<typename NumberType>
	static TRNT_CONSTEXPR TRNT_FORCE_INLINE NumberType Clamp(NumberType X, NumberType MinVal, NumberType MaxVal)
	{
		int TempMax = (X > MinVal) ? X : MinVal;
		return (TempMax > MaxVal) ? MaxVal : TempMax;
	}

	template<typename NumberType>
	static TRNT_CONSTEXPR TRNT_FORCE_INLINE NumberType Max(NumberType A, NumberType B)
	{
		return (A > B) ? A : B;
	}

	template<typename NumberType>
	static TRNT_CONSTEXPR TRNT_FORCE_INLINE NumberType Min(NumberType A, NumberType B)
	{
		return (A > B) ? B : A;
	}

	template<typename NumberType, typename... NumberTypes>
	static TRNT_CONSTEXPR TRNT_FORCE_INLINE NumberType Max(NumberType A, NumberType B, NumberTypes... Rest)
	{
		return Max<NumberType, NumberTypes...>(Max<NumberType>(A, B), Rest...);
	}

	template<typename NumberType, typename... NumberTypes>
	static TRNT_CONSTEXPR TRNT_FORCE_INLINE NumberType Min(NumberType A, NumberType B, NumberTypes... Rest)
	{
		return Min<NumberType, NumberTypes...>(Min<NumberType>(A, B), Rest...);
	}

	template<typename NumberType>
	static TRNT_CONSTEXPR TRNT_FORCE_INLINE NumberType Sign(NumberType X)
	{
		return (X > 0) ? static_cast<NumberType>(1) : (X < 0 ? static_cast<NumberType>(-1) : 0);
	}

	template<typename NumberType>
	static TRNT_FORCE_INLINE void Swap(NumberType& A, NumberType& B)
	{
		NumberType Tmp = A;
		A = B;
		B = Tmp;
	}

public:
	template<typename NumberType>
	static TRNT_CONSTEXPR TRNT_FORCE_INLINE NumberType Square(NumberType X)
	{
		return X * X;
	}

	template<typename NumberType>
	static TRNT_CONSTEXPR TRNT_FORCE_INLINE NumberType Cube(NumberType X)
	{
		return X * X * X;
	}

	template<typename NumberType>
	static TRNT_CONSTEXPR TRNT_FORCE_INLINE NumberType Invert(NumberType X)
	{
		static_assert(TIsFloatingPoint<NumberType>::Value);
		return static_cast<NumberType>(((NumberType)1) / X);
	}

public:
	template<typename T>
	static TRNT_CONSTEXPR TRNT_FORCE_INLINE auto RadiansToDegrees(const T& Radian) -> decltype(Radian * (180.0f / TRNT_PI))
	{
		return Radian * (180.0f / TRNT_PI);
	}

	static TRNT_CONSTEXPR TRNT_FORCE_INLINE float RadiansToDegrees(float const& Radian) 
	{
		return Radian * (180.0f / TRNT_PI);
	}

	static TRNT_CONSTEXPR TRNT_FORCE_INLINE double RadiansToDegrees(double const& Radian) 
	{
		return Radian * (180.0 / TRNT_DOUBLE_PI);
	}

	template<typename T>
	static TRNT_CONSTEXPR TRNT_FORCE_INLINE auto DegreesToRadians(const T& Degree) -> decltype(Degree * (TRNT_PI / 180.f))
	{
		return Degree * (TRNT_PI / 180.0f);
	}

	static TRNT_CONSTEXPR TRNT_FORCE_INLINE float DegreesToRadians(float const& Degree) 
	{ 
		return Degree * (TRNT_PI / 180.0f);
	}
	static TRNT_CONSTEXPR TRNT_FORCE_INLINE double DegreesToRadians(double const& Degree) 
	{
		return Degree * (TRNT_DOUBLE_PI / 180.0);
	}
};