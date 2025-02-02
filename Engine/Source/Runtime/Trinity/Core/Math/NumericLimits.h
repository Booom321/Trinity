#pragma once

#include "Trinity/Core/Defines.h"

#include <limits>

namespace TNsMath
{
	template<typename T>
	class TNumericLimits
	{
	public:
		static TRNT_CONSTEXPR T Max()
		{
			return std::numeric_limits<T>::max();
		}

		static TRNT_CONSTEXPR T Min()
		{
			return std::numeric_limits<T>::min();
		}

		static TRNT_CONSTEXPR T Lowest()
		{
			return std::numeric_limits<T>::lowest();
		}
	};

	template<typename T>
	class TNumericLimits<const T> : public TNumericLimits<T>
	{};

	template<typename T>
	class TNumericLimits<volatile T> : public TNumericLimits<T>
	{};

	template<typename T>
	class TNumericLimits<const volatile T> : public TNumericLimits<T>
	{};
} // namespace TNsMath