#pragma once

#include "Trinity/Core/Defines.h"

class TMath
{
public:
	template<typename NumberType>
	static TRNT_FORCE_INLINE NumberType Max(NumberType A, NumberType B)
	{
		return (A > B) ? A : B;
	}

	template<typename NumberType>
	static TRNT_FORCE_INLINE NumberType Min(NumberType A, NumberType B)
	{
		return (A > B) ? B : A;
	}
};