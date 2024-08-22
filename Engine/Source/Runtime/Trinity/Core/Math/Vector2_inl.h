#pragma once

#include "Vector2.h"

template<typename T>
TRNT_FORCE_INLINE TVector2<T>::TVector2() = default;

template<typename T>
TRNT_FORCE_INLINE TVector2<T>::TVector2(T X, T Y)
	: X(X), Y(Y)
{}

template<typename T>
TRNT_FORCE_INLINE TVector2<T>::TVector2(const TVector2& Other)
	: X(Other.X), Y(Other.Y)
{}

template<typename T>
template<typename U, typename V>
TRNT_FORCE_INLINE TVector2<T>::TVector2(U X, V Y)
	: X(static_cast<T>(X)), Y(static_cast<T>(Y))
{}

template<typename T>
template<typename U>
TRNT_FORCE_INLINE TVector2<T>::TVector2(const TVector2<U>& Other)
	: X(static_cast<T>(Other.X)), Y(static_cast<T>(Other.Y))
{}

template<typename T>
TRNT_FORCE_INLINE TVector2<T>::TVector2(T XY)
	: X(XY), Y(XY)
{}

template<typename T>
TRNT_FORCE_INLINE const T& TVector2<T>::operator[](TInt32 Index) const
{
	TRNT_ASSERT(Index == 0 && Index == 1);
	return (Index == 0) ? X : Y;
}

template<typename T>
TRNT_FORCE_INLINE T& TVector2<T>::operator[](TInt32 Index)
{
	TRNT_ASSERT(Index == 0 && Index == 1);
	return (Index == 0) ? X : Y;
}

template<typename T>
TRNT_FORCE_INLINE TBool TVector2<T>::operator==(const TVector2<T>& Other) const
{
	return X == Other.X && Y == Other.Y;
}

template<typename T>
TRNT_FORCE_INLINE TBool TVector2<T>::operator!=(const TVector2<T>& Other) const
{
	return X != Other.X || Y != Other.Y;
}

template<typename T>
TRNT_FORCE_INLINE TBool TVector2<T>::operator>(const TVector2<T>& Other) const
{
	return X > Other.X && Y > Other.Y;
}

template<typename T>
TRNT_FORCE_INLINE TBool TVector2<T>::operator>=(const TVector2<T>& Other) const
{
	return X >= Other.X && Y >= Other.Y;
}

template<typename T>
TRNT_FORCE_INLINE TBool TVector2<T>::operator<(const TVector2<T>& Other) const
{
	return X < Other.X && Y < Other.Y;
}

template<typename T>
TRNT_FORCE_INLINE TBool TVector2<T>::operator<=(const TVector2<T>& Other) const
{
	return X <= Other.X && Y <= Other.Y;
}

template<typename T>
TRNT_FORCE_INLINE TVector2<T> TVector2<T>::operator-()
{
	return TVector2<T>{ -X, -Y };
}

template<typename T>
TRNT_FORCE_INLINE TVector2<T> TVector2<T>::operator~()
{
	return TVector2<T>{ ~X, ~Y };
}

template<typename T>
TRNT_FORCE_INLINE TVector2<T> TVector2<T>::operator+(const TVector2<T>& Other) const
{
	return TVector2<T>{ X + Other.X, Y + Other.Y };
}

template<typename T>
TRNT_FORCE_INLINE TVector2<T> TVector2<T>::operator-(const TVector2<T>& Other) const
{
	return TVector2<T>{ X + Other.X, Y + Other.Y };
}

template<typename T>
TRNT_FORCE_INLINE TVector2<T> TVector2<T>::operator*(const TVector2<T>& Other) const
{
	return TVector2<T>{ X + Other.X, Y + Other.Y };
}

template<typename T>
TRNT_FORCE_INLINE TVector2<T> TVector2<T>::operator/(const TVector2<T>& Other) const
{
	return TVector2<T>{ X + Other.X, Y + Other.Y };
}

template<typename T>
TRNT_FORCE_INLINE TVector2<T> TVector2<T>::operator+(T Val) const
{
	return TVector2<T>{ X + Val, Y + Val };
}

template<typename T>
TRNT_FORCE_INLINE TVector2<T> TVector2<T>::operator-(T Val) const
{
	return TVector2<T>{ X - Val, Y - Val };
}

template<typename T>
TRNT_FORCE_INLINE TVector2<T> TVector2<T>::operator*(T Val) const
{
	return TVector2<T>{ X * Val, Y * Val };
}

template<typename T>
TRNT_FORCE_INLINE TVector2<T> TVector2<T>::operator/(T Val) const
{
	return TVector2<T>{ X / Val, Y / Val };
}

template<typename T>
TRNT_FORCE_INLINE TVector2<T>& TVector2<T>::operator+=(const TVector2<T>& Other)
{
	X += Other.X;
	Y += Other.Y;
	return *this;
}

template<typename T>
TRNT_FORCE_INLINE TVector2<T>& TVector2<T>::operator-=(const TVector2<T>& Other)
{
	X -= Other.X;
	Y -= Other.Y;
	return *this;
}

template<typename T>
TRNT_FORCE_INLINE TVector2<T>& TVector2<T>:: operator*=(const TVector2<T>& Other)
{
	X *= Other.X;
	Y *= Other.Y;
	return *this;
}

template<typename T>
TRNT_FORCE_INLINE TVector2<T>& TVector2<T>::operator/=(const TVector2<T>& Other)
{
	X /= Other.X;
	Y /= Other.Y;
	return *this;
}

template<typename T>
TRNT_FORCE_INLINE TVector2<T>& TVector2<T>::operator+=(T Val)
{
	X += Val;
	Y += Val;
	return *this;
}

template<typename T>
TRNT_FORCE_INLINE TVector2<T>& TVector2<T>::operator-=(T Val) 
{
	X -= Val;
	Y -= Val;
	return *this;
}

template<typename T>
TRNT_FORCE_INLINE TVector2<T>& TVector2<T>::operator*=(T Val) 
{
	X *= Val;
	Y *= Val;
	return *this;
}

template<typename T>
TRNT_FORCE_INLINE TVector2<T>& TVector2<T>::operator/=(T Val) 
{
	X /= Val;
	Y /= Val;
	return *this;
}

template<typename T>
TRNT_FORCE_INLINE T TVector2<T>::GetLength() const
{
	return TMath::Sqrt<T>(X * X + Y * Y);
}

template<typename T>
TRNT_FORCE_INLINE T TVector2<T>::GetLengthSquared() const
{
	return X * X + Y * Y;
}

template<typename T>
TRNT_FORCE_INLINE TBool TVector2<T>::GetNormalized(TVector2<T>& Result) const
{
	const T Length = TMath::Sqrt<T>(X * X + Y * Y);

	if (Length == 0) { return false; }

	const T InvLength = ((T)1) / Length;
	Result.X *= InvLength;
	Result.Y *= InvLength;

	return true;
}

template<typename T>
TRNT_FORCE_INLINE TBool TVector2<T>::Normalize()
{
	const T Length = TMath::Sqrt<T>(X * X + Y * Y);

	if (Length == 0) { return false; }

	const T InvLength = ((T)1) / Length;
	X *= InvLength;
	Y *= InvLength;

	return true;
}

template<typename T>
TRNT_FORCE_INLINE TBool TVector2<T>::IsNormalized() const
{
	return (X * X + Y * Y) == 1;
}

template<typename T>
TRNT_FORCE_INLINE TBool TVector2<T>::IsZero() const
{
	return X == 0 && Y == 0;
}

template<typename T>
TRNT_FORCE_INLINE TBool TVector2<T>::IsNaN() const
{
	return TMath::IsNaN<T>(X) || TMath::IsNaN<T>(Y);
}

template<typename T>
TRNT_FORCE_INLINE TBool TVector2<T>::IsInfinity() const
{
	return TMath::IsInfinity<T>(X) || TMath::IsInfinity<T>(Y);
}

template<typename T>
TRNT_FORCE_INLINE T TVector2<T>::Dot(const TVector2<T>& Other) const
{
	return X * Other.X + Y * Other.Y;
}

template<typename T>
TRNT_FORCE_INLINE T TVector2<T>::GetMin() const
{
	return (X > Y) ? Y : X;
}

template<typename T>
TRNT_FORCE_INLINE T TVector2<T>::GetMax() const
{
	return (X > Y) ? X : Y;
}

template<typename T>
template<typename DegreeType>
TRNT_FORCE_INLINE DegreeType TVector2<T>::GetAngle(const TVector2<T>& Other) const
{
	const DegreeType Value = static_cast<DegreeType>(TMath::Clamp<T>(Dot(Other), (T)-1, (T)1));

	return static_cast<DegreeType>(TMath::RadiansToDegrees(TMath::Acos(Value)));
}

template<typename T>
template<typename DegreeType>
TRNT_FORCE_INLINE TVector2<T> TVector2<T>::Rotate(const DegreeType& Degrees) const
{
	const T Radians = TMath::DegreesToRadians<DegreeType>(Degrees);
	const T CosValue = TMath::Cos(Radians);
	const T SinValue = TMath::Sin(Radians);
	return TVector2<T>(X * CosValue - Y * SinValue, X * SinValue + Y * CosValue);
}

template<typename T>
TRNT_FORCE_INLINE TVector2<T> TVector2<T>::Scale(const T& Scalar) const
{
	return TVector2<T>(X * Scalar, Y * Scalar);
}

template<typename T>
TRNT_FORCE_INLINE TVector2<T> TVector2<T>::GetAbs() const
{
	return TVector2<T>(
		X >= 0 ? X : -X,
		Y >= 0 ? Y : -Y
	);
}

///////////////////////////////////////////////////////////
template<typename T>
TRNT_FORCE_INLINE TVector2<T> operator+(T Val, const TVector2<T>& Other)
{
	return (Other + Val);
}

template<typename T>
TRNT_FORCE_INLINE TVector2<T> operator-(T Val, const TVector2<T>& Other)
{
	return (TVector2<T>(Val) - Other);
}

template<typename T>
TRNT_FORCE_INLINE TVector2<T> operator*(T Val, const TVector2<T>& Other)
{
	return (Other * Val);
}

template<typename T>
TRNT_FORCE_INLINE TVector2<T> operator/(T Val, const TVector2<T>& Other)
{
	return (TVector2<T>(Val) / Other);
}

///////////////////////////////////////////////////////////
namespace TNsHash
{
	template<typename T>
	inline TSize_T GetHashCode(const TVector2<T>& Vec2)
	{
		TSize_T Seed = 0;
		Seed ^= GetHashCode(Vec2.X) + 0x9e3779b9 + (Seed << 6) + (Seed >> 2);
		Seed ^= GetHashCode(Vec2.Y) + 0x9e3779b9 + (Seed << 6) + (Seed >> 2);
		return Seed;
	}
}
///////////////////////////////////////////////////////////
namespace TNsStringConversion
{
	template<typename T, typename CharType> TRNT_FORCE_INLINE void ToString(const TVector2<T>& Vec2, TStringBase<CharType>& OutString)
	{
		OutString.Clear();
		
		if constexpr (TAreTheSameType<CharType, TChar>::Value)
		{
			OutString = "[X=";
		}
		else if constexpr (TAreTheSameType<CharType, TWChar>::Value)
		{
			OutString = L"[X=";
		}
		
		TStringBase<CharType> Temp;
		ToString<CharType>(Vec2.X, Temp);
		OutString.Append(Temp);

		CharType EndSquareBracket = (CharType)0;
		if constexpr (TAreTheSameType<CharType, TChar>::Value)
		{
			OutString.Append(" Y=", 3);
			EndSquareBracket = ']';
		}
		else if constexpr (TAreTheSameType<CharType, TWChar>::Value)
		{
			OutString.Append(L" Y=", 3);
			EndSquareBracket = L']';
		}
		ToString<CharType>(Vec2.Y, Temp);
		OutString.Append(Temp);
		OutString.Append(1, EndSquareBracket);
	}
}
