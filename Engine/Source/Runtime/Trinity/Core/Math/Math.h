#pragma once

#include "MathConstants.h"
#include "Trinity/Core/Defines.h"
#include "Trinity/Core/TypeTraits/PrimaryTypes.h"

#include <cmath>

template<typename T>
class TVector2;
template<typename T>
class TVector3;
template<typename T>
class TVector4;

namespace TNsMath
{
	/**
		Trigonometric functions
	**/
	static TRNT_FORCE_INLINE TFloat Sin(TFloat X)
	{
		return sinf(X);
	}

	static TRNT_FORCE_INLINE TDouble Sin(TDouble X)
	{
		return sin(X);
	}

	static TRNT_FORCE_INLINE TLongDouble Sin(TLongDouble X)
	{
		return sinl(X);
	}

	static TRNT_FORCE_INLINE TFloat Cos(TFloat X)
	{
		return cosf(X);
	}

	static TRNT_FORCE_INLINE TDouble Cos(TDouble X)
	{
		return cos(X);
	}

	static TRNT_FORCE_INLINE TLongDouble Cos(TLongDouble X)
	{
		return cosl(X);
	}

	static TRNT_FORCE_INLINE TFloat Tan(TFloat X)
	{
		return tanf(X);
	}

	static TRNT_FORCE_INLINE TDouble Tan(TDouble X)
	{
		return tan(X);
	}

	static TRNT_FORCE_INLINE TLongDouble Tan(TLongDouble X)
	{
		return tanl(X);
	}

	static TRNT_FORCE_INLINE TFloat Asin(TFloat X)
	{
		return asinf(X);
	}

	static TRNT_FORCE_INLINE TDouble Asin(TDouble X)
	{
		return asin(X);
	}

	static TRNT_FORCE_INLINE TLongDouble Asin(TLongDouble X)
	{
		return asinl(X);
	}

	static TRNT_FORCE_INLINE TFloat Acos(TFloat X)
	{
		return acosf(X);
	}

	static TRNT_FORCE_INLINE TDouble Acos(TDouble X)
	{
		return acos(X);
	}

	static TRNT_FORCE_INLINE TLongDouble Acos(TLongDouble X)
	{
		return acosl(X);
	}

	static TRNT_FORCE_INLINE TFloat Atan(TFloat X)
	{
		return atanf(X);
	}

	static TRNT_FORCE_INLINE TDouble Atan(TDouble X)
	{
		return atan(X);
	}

	static TRNT_FORCE_INLINE TLongDouble Atan(TLongDouble X)
	{
		return atanl(X);
	}

	/**
		Hyperbolic functions
	**/
	static TRNT_FORCE_INLINE TFloat Sinh(TFloat X)
	{
		return sinhf(X);
	}

	static TRNT_FORCE_INLINE TDouble Sinh(TDouble X)
	{
		return sinh(X);
	}

	static TRNT_FORCE_INLINE TLongDouble Sinh(TLongDouble X)
	{
		return sinhl(X);
	}

	static TRNT_FORCE_INLINE TFloat Cosh(TFloat X)
	{
		return coshf(X);
	}

	static TRNT_FORCE_INLINE TDouble Cosh(TDouble X)
	{
		return cosh(X);
	}

	static TRNT_FORCE_INLINE TLongDouble Cosh(TLongDouble X)
	{
		return coshl(X);
	}

	static TRNT_FORCE_INLINE TFloat Tanh(TFloat X)
	{
		return tanhf(X);
	}

	static TRNT_FORCE_INLINE TDouble Tanh(TDouble X)
	{
		return tanh(X);
	}

	static TRNT_FORCE_INLINE TLongDouble Tanh(TLongDouble X)
	{
		return tanhl(X);
	}

	static TRNT_FORCE_INLINE TFloat Asinh(TFloat X)
	{
		return asinhf(X);
	}

	static TRNT_FORCE_INLINE TDouble Asinh(TDouble X)
	{
		return asinh(X);
	}

	static TRNT_FORCE_INLINE TLongDouble Asinh(TLongDouble X)
	{
		return asinhl(X);
	}

	static TRNT_FORCE_INLINE TFloat Acosh(TFloat X)
	{
		return acoshf(X);
	}

	static TRNT_FORCE_INLINE TDouble Acosh(TDouble X)
	{
		return acosh(X);
	}

	static TRNT_FORCE_INLINE TLongDouble Acosh(TLongDouble X)
	{
		return acoshl(X);
	}

	static TRNT_FORCE_INLINE TFloat Atanh(TFloat X)
	{
		return atanhf(X);
	}

	static TRNT_FORCE_INLINE TDouble Atanh(TDouble X)
	{
		return atanh(X);
	}

	static TRNT_FORCE_INLINE TLongDouble Atanh(TLongDouble X)
	{
		return atanhl(X);
	}

	/**
		Exponential functions
	**/
	static TRNT_FORCE_INLINE TFloat Exp(TFloat X)
	{
		return expf(X);
	}

	static TRNT_FORCE_INLINE TDouble Exp(TDouble X)
	{
		return exp(X);
	}

	static TRNT_FORCE_INLINE TLongDouble Exp(TLongDouble X)
	{
		return expl(X);
	}

	static TRNT_FORCE_INLINE TFloat Ln(TFloat X)
	{
		return logf(X);
	}

	static TRNT_FORCE_INLINE TDouble Ln(TDouble X)
	{
		return log(X);
	}

	static TRNT_FORCE_INLINE TLongDouble Ln(TLongDouble X)
	{
		return logl(X);
	}

	static TRNT_FORCE_INLINE TFloat Log2(TFloat X)
	{
		return log2f(X);
	}

	static TRNT_FORCE_INLINE TDouble Log2(TDouble X)
	{
		return log2(X);
	}

	static TRNT_FORCE_INLINE TLongDouble Log2(TLongDouble X)
	{
		return log2l(X);
	}

	static TRNT_FORCE_INLINE TFloat Log10(TFloat X)
	{
		return log10f(X);
	}

	static TRNT_FORCE_INLINE TDouble Log10(TDouble X)
	{
		return log10(X);
	}

	static TRNT_FORCE_INLINE TLongDouble Log10(TLongDouble X)
	{
		return log10l(X);
	}

	/**
		Power functions
	**/
	static TRNT_FORCE_INLINE TFloat Pow(TFloat X, TFloat Y)
	{
		return powf(X, Y);
	}

	static TRNT_FORCE_INLINE TDouble Pow(TDouble X, TDouble Y)
	{
		return pow(X, Y);
	}

	static TRNT_FORCE_INLINE TLongDouble Pow(TLongDouble X, TLongDouble Y)
	{
		return powl(X, Y);
	}

	static TRNT_FORCE_INLINE TFloat Sqrt(TFloat X)
	{
		return sqrtf(X);
	}

	static TRNT_FORCE_INLINE TDouble Sqrt(TDouble X)
	{
		return sqrt(X);
	}

	static TRNT_FORCE_INLINE TLongDouble Sqrt(TLongDouble X)
	{
		return sqrtl(X);
	}

	static TRNT_FORCE_INLINE TFloat InvSqrt(TFloat X)
	{
		return 1.0f / sqrtf(X);
	}

	static TRNT_FORCE_INLINE TDouble InvSqrt(TDouble X)
	{
		return 1.0 / sqrt(X);
	}

	static TRNT_FORCE_INLINE TLongDouble InvSqrt(TLongDouble X)
	{
		return ((TLongDouble)1.0) / sqrtl(X);
	}

	/**
		Nearest integer floating-point operations
	**/
	static TRNT_FORCE_INLINE TFloat Ceil(TFloat X)
	{
		return ceilf(X);
	}

	static TRNT_FORCE_INLINE TDouble Ceil(TDouble X)
	{
		return ceil(X);
	}

	static TRNT_FORCE_INLINE TLongDouble Ceil(TLongDouble X)
	{
		return ceill(X);
	}

	static TRNT_FORCE_INLINE TFloat Floor(TFloat X)
	{
		return floorf(X);
	}

	static TRNT_FORCE_INLINE TDouble Floor(TDouble X)
	{
		return floor(X);
	}

	static TRNT_FORCE_INLINE TLongDouble Floor(TLongDouble X)
	{
		return floorl(X);
	}

	static TRNT_FORCE_INLINE TFloat Round(TFloat X)
	{
		return roundf(X);
	}

	static TRNT_FORCE_INLINE TDouble Round(TDouble X)
	{
		return round(X);
	}

	static TRNT_FORCE_INLINE TLongDouble Round(TLongDouble X)
	{
		return roundl(X);
	}

	static TRNT_FORCE_INLINE TFloat Trunc(TFloat X)
	{
		return truncf(X);
	}

	static TRNT_FORCE_INLINE TDouble Trunc(TDouble X)
	{
		return trunc(X);
	}

	static TRNT_FORCE_INLINE TLongDouble Trunc(TLongDouble X)
	{
		return truncl(X);
	}

	static TRNT_FORCE_INLINE TFloat Fractional(TFloat X)
	{
		return X - truncf(X);
	}

	static TRNT_FORCE_INLINE TDouble Fractional(TDouble X)
	{
		return X - trunc(X);
	}

	static TRNT_FORCE_INLINE TLongDouble Fractional(TLongDouble X)
	{
		return X - truncl(X);
	}

	static TRNT_FORCE_INLINE TFloat Frac(TFloat X)
	{
		return X - floorf(X);
	}

	static TRNT_FORCE_INLINE TDouble Frac(TDouble X)
	{
		return X - floor(X);
	}

	static TRNT_FORCE_INLINE TLongDouble Frac(TLongDouble X)
	{
		return X - floorl(X);
	}

	/**
		Abs functions
	**/
	template<typename T>
	static TRNT_FORCE_INLINE T Abs(T X)
	{
		if constexpr (TAreTheSameType<T, TFloat>::Value)
		{
			return fabsf(X);
		}
		else if constexpr (TAreTheSameType<T, TDouble>::Value)
		{
			return fabs(X);
		}
		else if constexpr (TAreTheSameType<T, TLongDouble>::Value)
		{
			return fabsl(X);
		}
		else
		{
			return (X > static_cast<T>(0)) ? X : -X;
		}
	}

	/**
		Other functions
	**/
	template<typename T>
	static TRNT_CONSTEXPR TRNT_FORCE_INLINE TBool IsPowerOfTwo(T X)
	{
		return ((X & (X - 1)) == (T)0);
	}

	template<typename T>
	static TRNT_CONSTEXPR TRNT_FORCE_INLINE TBool IsNaN(T X)
	{
		if constexpr (TIsFloatingPoint<T>::Value)
		{
			return X != X;
		}
		else
		{
			return std::isnan<T>(X);
		}
	}

	template<typename T>
	static TRNT_CONSTEXPR TRNT_FORCE_INLINE TBool IsInfinite(T X)
	{
		return std::isinf<T>(X);
	}

	template<typename T>
	static TRNT_CONSTEXPR TRNT_FORCE_INLINE TBool IsFinite(T X)
	{
		return std::isfinite<T>(X);
	}

	template<typename T>
	static TRNT_CONSTEXPR TRNT_FORCE_INLINE T Clamp(T X, T MinVal, T MaxVal)
	{
		return X < MinVal ? MinVal : X < MaxVal ? X
												: MaxVal;
	}

	template<typename T>
	static TRNT_CONSTEXPR TRNT_FORCE_INLINE T Max(T X, T Y)
	{
		return X > Y ? X : Y;
	}

	template<typename T, typename... Rest>
	static TRNT_CONSTEXPR TRNT_FORCE_INLINE T Max(T X, T Y, Rest... R)
	{
		return Max<T, Rest...>(Max<T>(X, Y), R...);
	}

	template<typename T>
	static TRNT_CONSTEXPR TRNT_FORCE_INLINE T Min(T X, T Y)
	{
		return X > Y ? Y : X;
	}

	template<typename T, typename... Rest>
	static TRNT_CONSTEXPR TRNT_FORCE_INLINE T Min(T X, T Y, Rest... R)
	{
		return Min<T, Rest...>(Min<T>(X, Y), R...);
	}

	template<typename T>
	static TRNT_CONSTEXPR TRNT_FORCE_INLINE T Sign(T X)
	{
		return (X > static_cast<T>(0)) ? static_cast<T>(1) : (X < 0 ? static_cast<T>(-1) : 0);
	}

	template<typename T>
	static TRNT_FORCE_INLINE void Swap(T& A, T& B)
	{
		T Tmp = A;
		A = B;
		B = Tmp;
	}

	template<typename T>
	static TRNT_CONSTEXPR TRNT_FORCE_INLINE T Square(T X)
	{
		return X * X;
	}

	template<typename T>
	static TRNT_CONSTEXPR TRNT_FORCE_INLINE T Cube(T X)
	{
		return X * X * X;
	}

	template<typename T>
	static TRNT_CONSTEXPR TRNT_FORCE_INLINE T RadiansToDegrees(T Radian)
	{
		static TRNT_CONSTEXPR T Mult = static_cast<T>(180.0) / TNsMath::Pi<T>;
		return Radian * Mult;
	}

	template<typename T>
	static TRNT_CONSTEXPR TRNT_FORCE_INLINE T DegreesToRadians(T Degree)
	{
		static TRNT_CONSTEXPR T Mult = TNsMath::Pi<T> / static_cast<T>(180.0);
		return Degree * Mult;
	}

	template<typename T, typename U>
	static TRNT_CONSTEXPR TRNT_FORCE_INLINE T Lerp(const T& From, const T& To, const U& Amount)
	{
		return static_cast<T>(From + Amount * (To - From));
	}

	template<typename T>
	static TRNT_CONSTEXPR TRNT_FORCE_INLINE TBool IsInRange(const T X, const T Min, const T Max)
	{
		return X >= Min && X <= Max;
	}

	template<typename T>
	static TRNT_CONSTEXPR TRNT_FORCE_INLINE TBool IsNotInRange(const T X, const T Min, const T Max)
	{
		return X < Min || X > Max;
	}

	template<typename T>
	static TRNT_FORCE_INLINE TBool IsNearlyEqual(T X, T Y, T Tolerance = TNsMath::SmallTolerance<T>)
	{
		return Abs<T>(X - Y) <= Tolerance;
	}

	template<typename T>
	static TRNT_FORCE_INLINE TBool IsNearlyZero(T X, T Tolerance = TNsMath::SmallTolerance<T>)
	{
		return Abs<T>(X) <= Tolerance;
	}

	static TRNT_FORCE_INLINE TInt32 Rand()
	{
		return rand();
	}

	static TRNT_FORCE_INLINE void SRand(TUInt32 Seeds)
	{
		srand(Seeds);
	}

	template<typename T>
	static TRNT_FORCE_INLINE T RandRange(T Min, T Max)
	{
		return static_cast<T>(Min + (((T)rand()) % (Max - Min + (T)1)));
	}

	template<typename T>
	static TRNT_CONSTEXPR TRNT_FORCE_INLINE T Saturate(T X)
	{
		return X < 0 ? 0 : X < 1 ? X
								 : 1;
	}
} // namespace TNsMath