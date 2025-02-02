#pragma once

#include "Math.h"
#include "Trinity/Core/TypeTraits/EnableIf.h"

namespace TNsMath
{
	template<typename T>
	class TVector2
	{
	public:
		using ComponentType = T;

		static TRNT_CONSTEXPR TInt32 VecSize = 2;

	public:
		// default constructor
		TRNT_FORCE_INLINE TVector2() = default;

		// copy & move constructors
		TRNT_FORCE_INLINE TVector2(const TVector2&) = default;
		TRNT_FORCE_INLINE TVector2(TVector2&&) noexcept = default;

		// other constructors
		TRNT_FORCE_INLINE TVector2(T X, T Y)
			: X(X), Y(Y)
		{}

		template<typename U, typename TEnableIf<TNot<TAreTheSameType<T, U>>::Value, int>::Type = 0>
		explicit TRNT_FORCE_INLINE TVector2(const TVector2<U>& Vec)
			: X((T)Vec.X), Y((T)Vec.Y)
		{}

		explicit TRNT_FORCE_INLINE TVector2(T XY)
			: X(XY), Y(XY)
		{}

		explicit TRNT_FORCE_INLINE TVector2(const T* XY)
			: X(XY[0]), Y(XY[1])
		{}

		// destructor
		~TVector2() = default;

	public:
		TRNT_FORCE_INLINE TVector2& operator=(const TVector2&) = default;
		TRNT_FORCE_INLINE TVector2& operator=(TVector2&&) noexcept = default;

	public:
		TRNT_FORCE_INLINE T GetX() const
		{
			return X;
		}

		TRNT_FORCE_INLINE T GetY() const
		{
			return Y;
		}

		TRNT_FORCE_INLINE T& GetX()
		{
			return X;
		}

		TRNT_FORCE_INLINE T& GetY()
		{
			return Y;
		}

		TRNT_FORCE_INLINE void SetX(T Val)
		{
			X = Val;
		}

		TRNT_FORCE_INLINE void SetY(T Val)
		{
			Y = Val;
		}

		TRNT_FORCE_INLINE void Set(T Val)
		{
			X = Val, Y = Val;
		}

		TRNT_FORCE_INLINE void Set(T X, T Y)
		{
			this->X = X, this->Y = Y;
		}

		TRNT_FORCE_INLINE T* GetData()
		{
			return XY;
		}

		TRNT_FORCE_INLINE const T* GetData() const
		{
			return XY;
		}

		TRNT_FORCE_INLINE T operator[](TInt32 Idx) const
		{
			return XY[Idx];
		}

		TRNT_FORCE_INLINE T& operator[](TInt32 Idx)
		{
			return XY[Idx];
		}

		TRNT_FORCE_INLINE TBool operator==(const TVector2& Vec) const
		{
			return X == Vec.X && Y == Vec.Y;
		}

		TRNT_FORCE_INLINE TBool operator!=(const TVector2& Vec) const
		{
			return X != Vec.X || Y != Vec.Y;
		}

		TRNT_FORCE_INLINE TBool operator>(const TVector2& Vec) const
		{
			return X > Vec.X && Y > Vec.Y;
		}

		TRNT_FORCE_INLINE TBool operator>=(const TVector2& Vec) const
		{
			return X >= Vec.X && Y >= Vec.Y;
		}

		TRNT_FORCE_INLINE TBool operator<(const TVector2& Vec) const
		{
			return X < Vec.X && Y < Vec.Y;
		}

		TRNT_FORCE_INLINE TBool operator<=(const TVector2& Vec) const
		{
			return X <= Vec.X && Y <= Vec.Y;
		}

		TRNT_FORCE_INLINE TVector2 operator-() const
		{
			return TVector2(-X, -Y);
		}

		TRNT_FORCE_INLINE TVector2 operator+(const TVector2& Vec) const
		{
			return TVector2(X + Vec.X, Y + Vec.Y);
		}

		TRNT_FORCE_INLINE TVector2 operator-(const TVector2& Vec) const
		{
			return TVector2(X - Vec.X, Y - Vec.Y);
		}

		TRNT_FORCE_INLINE TVector2 operator*(const TVector2& Vec) const
		{
			return TVector2(X * Vec.X, Y * Vec.Y);
		}

		TRNT_FORCE_INLINE TVector2 operator/(const TVector2& Vec) const
		{
			return TVector2(X / Vec.X, Y / Vec.Y);
		}

		TRNT_FORCE_INLINE TVector2 operator+(T Val) const
		{
			return TVector2(X + Val, Y + Val);
		}

		TRNT_FORCE_INLINE TVector2 operator-(T Val) const
		{
			return TVector2(X - Val, Y - Val);
		}

		TRNT_FORCE_INLINE TVector2 operator*(T Val) const
		{
			return TVector2(X * Val, Y * Val);
		}

		TRNT_FORCE_INLINE TVector2 operator/(T Val) const
		{
			return TVector2(X / Val, Y / Val);
		}

		TRNT_FORCE_INLINE TVector2& operator+=(const TVector2& Vec) const
		{
			X += Vec.X, Y += Vec.Y;
			return *this;
		}

		TRNT_FORCE_INLINE TVector2& operator-=(const TVector2& Vec) const
		{
			X -= Vec.X, Y -= Vec.Y;
			return *this;
		}

		TRNT_FORCE_INLINE TVector2& operator*=(const TVector2& Vec) const
		{
			X *= Vec.X, Y *= Vec.Y;
			return *this;
		}

		TRNT_FORCE_INLINE TVector2& operator/=(const TVector2& Vec) const
		{
			X /= Vec.X, Y /= Vec.Y;
			return *this;
		}

		TRNT_FORCE_INLINE TVector2& operator+=(T Val) const
		{
			X += Val, Y += Val;
			return *this;
		}

		TRNT_FORCE_INLINE TVector2& operator-=(T Val) const
		{
			X -= Val, Y -= Val;
			return *this;
		}

		TRNT_FORCE_INLINE TVector2& operator*=(T Val) const
		{
			X *= Val, Y *= Val;
			return *this;
		}

		TRNT_FORCE_INLINE TVector2& operator/=(T Val) const
		{
			X /= Val, Y /= Val;
			return *this;
		}

	public:
		TRNT_FORCE_INLINE TBool IsNormalized() const
		{
			return TNsMath::Abs<TFloat>((X * X + Y * Y) - 1.0f) <= TNsMath::SmallTolerance<TFloat>;
		}

		TRNT_FORCE_INLINE TBool IsZero() const
		{
			return X == (T)0 && Y == (T)0;
		}

		TRNT_FORCE_INLINE TBool IsNearlyZero(T Tolerance = TNsMath::SmallTolerance<T>) const
		{
			return TNsMath::Abs<T>(X) <= Tolerance && TNsMath::Abs<T>(Y) <= Tolerance;
		}

		TRNT_FORCE_INLINE TBool IsNaN() const
		{
			return TNsMath::IsNaN<T>(X) || TNsMath::IsNaN<T>(Y);
		}

		TRNT_FORCE_INLINE TBool IsInfinite() const
		{
			return TNsMath::IsInfinite<T>(X) || TNsMath::IsInfinite<T>(Y);
		}

		TRNT_FORCE_INLINE TBool IsFinite() const
		{
			return TNsMath::IsFinite<T>(X) || TNsMath::IsFinite<T>(Y);
		}

		TRNT_FORCE_INLINE TBool IsEquals(const TVector2& Vec, T Tolerance = TNsMath::SmallTolerance<T>) const
		{
			return TNsMath::Abs<T>(X - Vec.X) <= Tolerance && TNsMath::Abs<T>(Y - Vec.Y) <= Tolerance;
		}

	public:
		TRNT_FORCE_INLINE T GetAverage() const
		{
			return (X + Y) * TNsMath::Invert<VecSize, T>;
		}

		TRNT_FORCE_INLINE T GetSumValues() const
		{
			return X + Y;
		}

		TRNT_FORCE_INLINE T GetLength() const
		{
			return TNsMath::Sqrt<T>(X * X + Y * Y);
		}

		TRNT_FORCE_INLINE T GetLengthSquared() const
		{
			return X * X + Y * Y;
		}

		TRNT_FORCE_INLINE T GetMaxValue() const
		{
			return X > Y ? X : Y;
		}

		TRNT_FORCE_INLINE T GetMinValue() const
		{
			return X < Y ? X : Y;
		}

		TRNT_FORCE_INLINE TVector2 GetAbsoluteVector() const
		{
			return TVector2(X > 0 ? X : -X, Y > 0 ? Y : -Y);
		}

		TRNT_FORCE_INLINE TVector2 GetNormalizedVector() const
		{
			TVector2 Ret(X, Y);
			Ret.Normalize();
			return Ret;
		}

		void Normalize() const
		{
			const T VecLength = TNsMath::Sqrt<T>(X * X + Y * Y);
			
			if (VecLength >= TNsMath::SmallTolerance<T>)
			{
				const T Scale = (T)1.0f / VecLength;
				X *= Scale;
				Y *= Scale;
				return;
			}
		}

		void NormalizeFast() const
		{
			const T Scale = (T)1.0f / TNsMath::Sqrt<T>(X * X + Y * Y);
			X *= Scale;
			Y *= Scale;
			return;
		}

	public:
		static const TVector2 ZeroVec;
		static const TVector2 OneVec;
		static const TVector2 UnitXVec;
		static const TVector2 UnitYVec;

	public:
		static TRNT_INLINE TVector2 Zero()
		{
			return ZeroVec;
		}

		static TRNT_INLINE TVector2 One()
		{
			return OneVec;
		}

		static TRNT_INLINE TVector2 UnitX()
		{
			return UnitXVec;
		}

		static TRNT_INLINE TVector2 UnitY()
		{
			return UnitYVec;
		}

	public:
		static TRNT_FORCE_INLINE T Dot(const TVector2& VecA, const TVector2& VecB)
		{
			return VecA.X * VecB.X + VecA.Y * VecB.Y;
		}

		static TRNT_FORCE_INLINE T Cross(const TVector2& VecA, const TVector2& VecB)
		{
			return VecA.X * VecB.X - VecA.Y * VecB.Y;
		}

		static TRNT_FORCE_INLINE TVector2 Min(const TVector2& VecA, const TVector2& VecB)
		{
			return TVector2(VecA.X < VecB.X ? VecA.X : VecB.X, VecA.Y < VecB.Y ? VecA.Y : VecB.Y);
		}

		static TRNT_FORCE_INLINE TVector2 Max(const TVector2& VecA, const TVector2& VecB)
		{
			return TVector2(VecA.X > VecB.X ? VecA.X : VecB.X, VecA.Y > VecB.Y ? VecA.Y : VecB.Y);
		}

		static TRNT_FORCE_INLINE TVector2 Clamp(const TVector2& Vec, const TVector2& MinV, const TVector2& MaxV)
		{
			return TVector2<T>(TNsMath::Clamp<T>(Vec.X, MinV.X, MaxV.X), TNsMath::Clamp<T>(Vec.Y, MinV.Y, MaxV.Y));
		}

		static T GetDistanceSquared(const TVector2& VecA, const TVector2& VecB)
		{
			const T OffsetX = VecB.X - VecA.X;
			const T OffsetY = VecB.Y - VecA.Y;
			return OffsetX * OffsetX + OffsetY * OffsetY;
		}

		static T GetDistance(const TVector2& VecA, const TVector2& VecB)
		{
			const T OffsetX = VecB.X - VecA.X;
			const T OffsetY = VecB.Y - VecA.Y;
			return TNsMath::Sqrt<T>(OffsetX * OffsetX + OffsetY * OffsetY);
		}

		template<typename FPType>
		static FPType GetAngleAsRadian(const TVector2& A, const TVector2& B)
		{
			return TNsMath::Acos(static_cast<FPType>(TNsMath::Clamp<T>(A.Dot(B), (T)-1, (T)1)));
		}

		template<typename FPType>
		static FPType GetAngleAsDegree(const TVector2& A, const TVector2& B)
		{
			return TNsMath::RadiansToDegrees<FPType>(TNsMath::Acos(static_cast<FPType>(TNsMath::Clamp<T>(A.Dot(B), (T)-1, (T)1))));
		}

		static TVector2 Rotate(const TVector2& Vec, const T Radian)
		{
			const auto C = std::cos(Radian);
			const auto S = std::sin(Radian);

			return TVector2(static_cast<T>(Vec.X * C - Vec.Y * S), static_cast<T>(Vec.X * S + Vec.Y * C));
		}

		template<typename U = TFloat>
		static TRNT_FORCE_INLINE TVector2 Lerp(const TVector2& From, const TVector2& To, U Amount)
		{
			return TVector2(
				TNsMath::Lerp<T, U>(From.X, To.X, Amount),
				TNsMath::Lerp<T, U>(From.Y, To.Y, Amount));
		}

	public:
		union
		{
			struct
			{
				T X;
				T Y;
			};

			T XY[VecSize];
		};
	};

	template<typename T>
	const TVector2<T> TVector2<T>::ZeroVec = TVector2<T>{ (T)0, (T)0 };
	template<typename T>
	const TVector2<T> TVector2<T>::OneVec = TVector2<T>{ (T)1, (T)1 };
	template<typename T>
	const TVector2<T> TVector2<T>::UnitXVec = TVector2<T>{ (T)1, (T)0 };
	template<typename T>
	const TVector2<T> TVector2<T>::UnitYVec = TVector2<T>{ (T)0, (T)1 };

	using TVector2f = TVector2<TFloat>;
	using TVector2d = TVector2<TDouble>;
	using TVector2ld = TVector2<TLongDouble>;

	using TVector2i = TVector2<TInt32>;
	using TVector2u = TVector2<TUInt32>;
} // namespace TNsMath

template<>
class TIsPODType<TNsMath::TVector2f> : public TTrueType
{};

template<>
class TIsPODType<TNsMath::TVector2d> : public TTrueType
{};

template<>
class TIsPODType<TNsMath::TVector2ld> : public TTrueType
{};

template<>
class TIsPODType<TNsMath::TVector2i> : public TTrueType
{};

template<>
class TIsPODType<TNsMath::TVector2u> : public TTrueType
{};

namespace TNsHash
{
	// ==> https://github.com/FlaxEngine/FlaxEngine/blob/master/Source/Engine/Core/Math/Vector2.h#L629
	template<typename T>
	inline TSize_T GetHashCode(const TNsMath::TVector2<T>& Vec)
	{
		return static_cast<TSize_T>((*(TUInt32*)&Vec.X * 397) ^ *(TUInt32*)&Vec.Y);
	}
} // namespace TNsHash