#pragma once

#include "Trinity/Core/Hash/HashFunctions.h"
#include "Trinity/Core/String/StringConversion.h"
#include "Trinity/Core/TypeTraits/PrimaryTypes.h"

#include "Math.h"

template<typename T>
class TVector2
{
public:
	static_assert(TIsArithmetic<T>::Value, "Type must be arithmetic");

	using ComponentType = T;
	static TRNT_CONSTEXPR TInt32 ComponentCount = 2;

public:
	TRNT_FORCE_INLINE TVector2();

	TRNT_FORCE_INLINE TVector2(T X, T Y);

	TRNT_FORCE_INLINE TVector2(const TVector2& Other);

	template<typename U, typename V>
	TRNT_FORCE_INLINE TVector2(U X, V Y);

	template<typename U>
	TRNT_FORCE_INLINE TVector2(const TVector2<U>& Other);

	explicit TRNT_FORCE_INLINE TVector2(T XY);

public:
	TRNT_FORCE_INLINE const T& operator[](TInt32 Index) const;
	TRNT_FORCE_INLINE T& operator[](TInt32 Index);

	TRNT_FORCE_INLINE TBool operator==(const TVector2<T>& Other) const;
	TRNT_FORCE_INLINE TBool operator!=(const TVector2<T>& Other) const;
	TRNT_FORCE_INLINE TBool operator>(const TVector2<T>& Other) const;
	TRNT_FORCE_INLINE TBool operator>=(const TVector2<T>& Other) const;
	TRNT_FORCE_INLINE TBool operator<(const TVector2<T>& Other) const;
	TRNT_FORCE_INLINE TBool operator<=(const TVector2<T>& Other) const;

	TRNT_FORCE_INLINE TVector2<T> operator-();
	TRNT_FORCE_INLINE TVector2<T> operator~();

	TRNT_FORCE_INLINE TVector2<T> operator+(const TVector2<T>& Other) const;
	TRNT_FORCE_INLINE TVector2<T> operator-(const TVector2<T>& Other) const;
	TRNT_FORCE_INLINE TVector2<T> operator*(const TVector2<T>& Other) const;
	TRNT_FORCE_INLINE TVector2<T> operator/(const TVector2<T>& Other) const;

	TRNT_FORCE_INLINE TVector2<T> operator+(T Val) const;
	TRNT_FORCE_INLINE TVector2<T> operator-(T Val) const;
	TRNT_FORCE_INLINE TVector2<T> operator*(T Val) const;
	TRNT_FORCE_INLINE TVector2<T> operator/(T Val) const;

	TRNT_FORCE_INLINE TVector2<T>& operator+=(const TVector2<T>& Other);
	TRNT_FORCE_INLINE TVector2<T>& operator-=(const TVector2<T>& Other);
	TRNT_FORCE_INLINE TVector2<T>& operator*=(const TVector2<T>& Other);
	TRNT_FORCE_INLINE TVector2<T>& operator/=(const TVector2<T>& Other);

	TRNT_FORCE_INLINE TVector2<T>& operator+=(T Val);
	TRNT_FORCE_INLINE TVector2<T>& operator-=(T Val);
	TRNT_FORCE_INLINE TVector2<T>& operator*=(T Val);
	TRNT_FORCE_INLINE TVector2<T>& operator/=(T Val);

public:
	TRNT_FORCE_INLINE T GetLength() const;
	TRNT_FORCE_INLINE T GetLengthSquared() const;

	TRNT_FORCE_INLINE TBool GetNormalized(TVector2<T>& Result) const;
	TRNT_FORCE_INLINE TBool Normalize();
	TRNT_FORCE_INLINE TBool IsNormalized() const;

	TRNT_FORCE_INLINE TBool IsZero() const;
	TRNT_FORCE_INLINE TBool IsNaN() const;
	TRNT_FORCE_INLINE TBool IsInfinity() const;

	TRNT_FORCE_INLINE T Dot(const TVector2<T>& Other) const;

	TRNT_FORCE_INLINE T GetMin() const;
	TRNT_FORCE_INLINE T GetMax() const;
	TRNT_FORCE_INLINE TVector2<T> GetAbs() const;

	template<typename DegreeType = double>
	TRNT_FORCE_INLINE DegreeType GetAngle(const TVector2<T>& Other) const;

	template<typename DegreeType = double>
	TRNT_FORCE_INLINE TVector2<T> Rotate(const DegreeType& Degrees) const;
	TRNT_FORCE_INLINE TVector2<T> Scale(const T& Scalar) const;
public:
	T X;
	T Y;
};

#include "Vector2_inl.h"