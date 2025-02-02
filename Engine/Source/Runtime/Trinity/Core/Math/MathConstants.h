#pragma once

#ifdef PI
	#undef PI
#endif

#include "Trinity/Core/Defines.h"
#include "Trinity/Core/Types/DataTypes.h"

namespace TNsMath
{
	template<typename T>
	static TRNT_CONSTEXPR T Pi = static_cast<T>(3.1415926535897932384626433832795028841971693993751);

	template<typename T>
	static TRNT_CONSTEXPR T TwoPi = static_cast<T>(T(2) * Pi<T>);

	template<typename T>
	static TRNT_CONSTEXPR T InvPi = static_cast<T>(T(1) / Pi<T>);

	template<typename T>
	static TRNT_CONSTEXPR T PiSquared = static_cast<T>(Pi<T> * Pi<T>);

	template<typename T>
	static TRNT_CONSTEXPR T E = static_cast<T>(2.7182818284590452353602874713527);

	template<typename T>
	static TRNT_CONSTEXPR T SmallTolerance = static_cast<T>(1E-16f);

	template<TInt64 Num, typename T = TFloat>
	static TRNT_CONSTEXPR T Invert = static_cast<T>((T)1 / (T)Num);
}; // namespace TNsMath