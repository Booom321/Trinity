#pragma once

#include "Math.h"
#include "Trinity/Core/TypeTraits/EnableIf.h"

namespace TNsMath
{
	template<typename T>
	class TVector4
	{
	public:
		using ComponentType = T;

		static TRNT_CONSTEXPR TInt32 VecSize = 4;

	public:
		// default constructor
		TRNT_FORCE_INLINE TVector4() = default;

		// copy & move constructors
		TRNT_FORCE_INLINE TVector4(const TVector4&) = default;
		TRNT_FORCE_INLINE TVector4(TVector4&&) noexcept = default;

		// other constructors
		TRNT_FORCE_INLINE TVector4(T X, T Y, T Z, T W)
			: X(X), Y(Y), Z(Z), W(W)
		{}

		TRNT_FORCE_INLINE TVector4(const TVector2<T>& XY, T Z, T W)
			: X(XY.X), Y(XY.Y), Z(Z), W(W)
		{}

		TRNT_FORCE_INLINE TVector4(const TVector2<T>& XY, const TVector2<T>& ZW)
			: X(XY.X), Y(XY.Y), Z(ZW.X), W(ZW.Y)
		{}

		TRNT_FORCE_INLINE TVector4(const TVector3<T>& XYZ, T W)
			: X(XYZ.X), Y(XYZ.Y), Z(XYZ.Z), W(W)
		{}

		template<typename U, typename TEnableIf<TNot<TAreTheSameType<T, U>>::Value, int>::Type = 0>
		explicit TRNT_FORCE_INLINE TVector4(const TVector4<U>& Vec)
			: X((T)Vec.X), Y((T)Vec.Y), Z((T)Vec.Z), W((T)Vec.W)
		{}

		explicit TRNT_FORCE_INLINE TVector4(T XYZW)
			: X(XYZW), Y(XYZW), Z(XYZW), W(XYZW)
		{}

		explicit TRNT_FORCE_INLINE TVector4(const T* XYZW)
			: X(XYZW[0]), Y(XYZW[1]), Z(XYZW[2]), W(XYZW[3])
		{}

		// destructor
		~TVector4() = default;

	public:
		TRNT_FORCE_INLINE TVector4& operator=(const TVector4&) = default;
		TRNT_FORCE_INLINE TVector4& operator=(TVector4&&) noexcept = default;

	public:
		TRNT_FORCE_INLINE T GetX() const
		{
			return X;
		}

		TRNT_FORCE_INLINE T GetY() const
		{
			return Y;
		}

		TRNT_FORCE_INLINE T GetZ() const
		{
			return Z;
		}

		TRNT_FORCE_INLINE T GetW() const
		{
			return W;
		}

		TRNT_FORCE_INLINE T& GetX()
		{
			return X;
		}

		TRNT_FORCE_INLINE T& GetY()
		{
			return Y;
		}

		TRNT_FORCE_INLINE T& GetZ()
		{
			return Z;
		}

		TRNT_FORCE_INLINE T& GetW()
		{
			return W;
		}

		TRNT_FORCE_INLINE void SetX(T Val)
		{
			X = Val;
		}

		TRNT_FORCE_INLINE void SetY(T Val)
		{
			Y = Val;
		}

		TRNT_FORCE_INLINE void SetZ(T Val)
		{
			Z = Val;
		}

		TRNT_FORCE_INLINE void SetW(T Val)
		{
			W = Val;
		}

		TRNT_FORCE_INLINE void Set(T Val)
		{
			X = Val, Y = Val, Z = Val, W = Val;
		}

		TRNT_FORCE_INLINE void Set(T X, T Y, T Z, T W)
		{
			this->X = X, this->Y = Y, this->Z = Z, this->W = W;
		}

		TRNT_FORCE_INLINE T* GetData()
		{
			return XYZW;
		}

		TRNT_FORCE_INLINE const T* GetData() const
		{
			return XYZW;
		}

		TRNT_FORCE_INLINE T operator[](TInt32 Idx) const
		{
			return XYZW[Idx];
		}

		TRNT_FORCE_INLINE T& operator[](TInt32 Idx)
		{
			return XYZW[Idx];
		}

		TRNT_FORCE_INLINE TBool operator==(const TVector4& Vec) const
		{
			return X == Vec.X && Y == Vec.Y && Z == Vec.Z && W == Vec.W;
		}

		TRNT_FORCE_INLINE TBool operator!=(const TVector4& Vec) const
		{
			return X != Vec.X || Y != Vec.Y || Z != Vec.Z || W != Vec.W;
		}

		TRNT_FORCE_INLINE TBool operator>(const TVector4& Vec) const
		{
			return X > Vec.X && Y > Vec.Y && Z > Vec.Z && W > Vec.W;
		}

		TRNT_FORCE_INLINE TBool operator>=(const TVector4& Vec) const
		{
			return X >= Vec.X && Y >= Vec.Y && Z >= Vec.Z && W >= Vec.W;
		}

		TRNT_FORCE_INLINE TBool operator<(const TVector4& Vec) const
		{
			return X < Vec.X && Y < Vec.Y && Z < Vec.Z && W < Vec.W;
		}

		TRNT_FORCE_INLINE TBool operator<=(const TVector4& Vec) const
		{
			return X <= Vec.X && Y <= Vec.Y && Z <= Vec.Z && W <= Vec.W;
		}

		TRNT_FORCE_INLINE TVector4 operator-() const
		{
			return TVector4(-X, -Y, -Z, -W);
		}

		TRNT_FORCE_INLINE TVector4 operator+(const TVector4& Vec) const
		{
			return TVector4(X + Vec.X, Y + Vec.Y, Z + Vec.Z, W + Vec.W);
		}

		TRNT_FORCE_INLINE TVector3 operator-(const TVector4& Vec) const
		{
			return TVector4(X - Vec.X, Y - Vec.Y, Z - Vec.Z, W - Vec.W);
		}

		TRNT_FORCE_INLINE TVector4 operator*(const TVector4& Vec) const
		{
			return TVector4(X * Vec.X, Y * Vec.Y, Z * Vec.Z, W * Vec.W);
		}

		TRNT_FORCE_INLINE TVector4 operator/(const TVector4& Vec) const
		{
			return TVector4(X / Vec.X, Y / Vec.Y, Z / Vec.Z, W / Vec.W);
		}

		TRNT_FORCE_INLINE TVector4 operator+(T Val) const
		{
			return TVector4(X + Val, Y + Val, Z + Val, W + Val);
		}

		TRNT_FORCE_INLINE TVector4 operator-(T Val) const
		{
			return TVector4(X - Val, Y - Val, Z - Val, W - Val);
		}

		TRNT_FORCE_INLINE TVector4 operator*(T Val) const
		{
			return TVector4(X * Val, Y * Val, Z * Val, W * Val);
		}

		TRNT_FORCE_INLINE TVector4 operator/(T Val) const
		{
			return TVector4(X / Val, Y / Val, Z / Val, W / Val);
		}

		TRNT_FORCE_INLINE TVector4& operator+=(const TVector4& Vec) const
		{
			X += Vec.X, Y += Vec.Y, Z += Vec.Z, W += Vec.W;
			return *this;
		}

		TRNT_FORCE_INLINE TVector4& operator-=(const TVector4& Vec) const
		{
			X -= Vec.X, Y -= Vec.Y, Z -= Vec.Z, W -= Vec.W;
			return *this;
		}

		TRNT_FORCE_INLINE TVector4& operator*=(const TVector4& Vec) const
		{
			X *= Vec.X, Y *= Vec.Y, Z *= Vec.Z, W *= Vec.W;
			return *this;
		}

		TRNT_FORCE_INLINE TVector4& operator/=(const TVector4& Vec) const
		{
			X /= Vec.X, Y /= Vec.Y, Z /= Vec.Z, W /= Vec.W;
			return *this;
		}

		TRNT_FORCE_INLINE TVector4& operator+=(T Val) const
		{
			X += Val, Y += Val, Z += Val, W += Val;
			return *this;
		}

		TRNT_FORCE_INLINE TVector4& operator-=(T Val) const
		{
			X -= Val, Y -= Val, Z -= Val, W -= Val;
			return *this;
		}

		TRNT_FORCE_INLINE TVector4& operator*=(T Val) const
		{
			X *= Val, Y *= Val, Z *= Val, W *= Val;
			return *this;
		}

		TRNT_FORCE_INLINE TVector4& operator/=(T Val) const
		{
			X /= Val, Y /= Val, Z /= Val, W /= Val;
			return *this;
		}

	public:
		TRNT_FORCE_INLINE TBool IsNormalized() const
		{
			return TNsMath::Abs<TFloat>((X * X + Y * Y + Z * Z + W * W) - 1.0f) < TNsMath::SmallTolerance<TFloat>;
		}

		TRNT_FORCE_INLINE TBool IsZero() const
		{
			return X == (T)0 && Y == (T)0 && Z == (T)0 && W == (T)0;
		}

		TRNT_FORCE_INLINE TBool IsNearlyZero(T Tolerance = TNsMath::SmallTolerance<T>) const
		{
			return TNsMath::Abs<T>(X) <= Tolerance && TNsMath::Abs<T>(Y) <= Tolerance && TNsMath::Abs<T>(Z) <= Tolerance && TNsMath::Abs<T>(W) <= Tolerance;
		}

		TRNT_FORCE_INLINE TBool IsNaN() const
		{
			return TNsMath::IsNaN<T>(X) || TNsMath::IsNaN<T>(Y) || TNsMath::IsNaN<T>(Z) || TNsMath::IsNaN<T>(W);
		}

		TRNT_FORCE_INLINE TBool IsInfinite() const
		{
			return TNsMath::IsInfinite<T>(X) || TNsMath::IsInfinite<T>(Y) || TNsMath::IsInfinite<T>(Z) || TNsMath::IsInfinite<T>(W);
		}

		TRNT_FORCE_INLINE TBool IsFinite() const
		{
			return TNsMath::IsFinite<T>(X) || TNsMath::IsFinite<T>(Y) || TNsMath::IsFinite<T>(Z) || TNsMath::IsFinite<T>(W);
		}

		TRNT_FORCE_INLINE TBool IsEquals(const TVector4& Vec, T Tolerance = TNsMath::SmallTolerance<T>) const
		{
			return TNsMath::Abs<T>(X - Vec.X) <= Tolerance && TNsMath::Abs<T>(Y - Vec.Y) <= Tolerance && TNsMath::Abs<T>(Z - Vec.Z) <= Tolerance && TNsMath::Abs<T>(W - Vec.W) <= Tolerance;
		}

	public:
		TRNT_FORCE_INLINE T GetAverage() const
		{
			return (X + Y + Z + W) * TNsMath::Invert<VecSize, T>;
		}

		TRNT_FORCE_INLINE T GetSumValues() const
		{
			return X + Y + Z + W;
		}

		TRNT_FORCE_INLINE T GetLength() const
		{
			return TNsMath::Sqrt<T>(X * X + Y * Y + Z * Z + W * W);
		}

		TRNT_FORCE_INLINE T GetLengthSquared() const
		{
			return X * X + Y * Y + Z * Z + W * W;
		}

		TRNT_FORCE_INLINE T GetMaxValue() const
		{
			return TNsMath::Max<T>(X, Y, Z, W);
		}

		TRNT_FORCE_INLINE T GetMinValue() const
		{
			return TNsMath::Min<T>(X, Y, Z, W);
		}

		TRNT_FORCE_INLINE TVector4 GetAbsoluteVector() const
		{
			return TVector4(X > 0 ? X : -X, Y > 0 ? Y : -Y, Z > 0 ? Z : -Z, W > 0 ? W : -W);
		}

		TRNT_FORCE_INLINE TVector4 GetNormalizedVector() const
		{
			TVector4 Ret(X, Y, Z, W);
			Ret.Normalize();
			return Ret;
		}

		void Normalize() const
		{
			const T VecLength = TNsMath::Sqrt<T>(X * X + Y * Y + Z * Z + W * W);

			if (VecLength >= TNsMath::SmallTolerance<T>)
			{
				const T Scale = (T)1.0f / VecLength;
				X *= Scale;
				Y *= Scale;
				Z *= Scale;
				W *= Scale;
			}
		}

		void NormalizeFast() const
		{
			const T Scale = (T)1.0f / TNsMath::Sqrt<T>(X * X + Y * Y + Z * Z + W * W);
			X *= Scale;
			Y *= Scale;
			Z *= Scale;
			W *= Scale;
			return;
		}

		TRNT_FORCE_INLINE TVector4 Reflect(const TVector4& Normal) const
		{
			return ((*this) - ((T)2 * Dot(*this, Normal) * Normal));
		}

	public:
		static const TVector4 ZeroVec;
		static const TVector4 OneVec;
		static const TVector4 UnitXVec;
		static const TVector4 UnitYVec;
		static const TVector4 UnitZVec;
		static const TVector4 UnitWVec;

		static TRNT_INLINE TVector4 Zero()
		{
			return ZeroVec;
		}

		static TRNT_INLINE TVector4 One()
		{
			return OneVec;
		}

		static TRNT_INLINE TVector4 UnitX()
		{
			return UnitXVec;
		}

		static TRNT_INLINE TVector4 UnitY()
		{
			return UnitYVec;
		}

		static TRNT_INLINE TVector4 UnitZ()
		{
			return UnitZVec;
		}

		static TRNT_INLINE TVector4 UnitW()
		{
			return UnitWVec;
		}

	public:
		static TRNT_FORCE_INLINE T Dot(const TVector4& VecA, const TVector4& VecB)
		{
			return VecA.X * VecB.X + VecA.Y * VecB.Y + VecA.Z * VecB.Z + VecA.W * VecB.W;
		}

		static TRNT_FORCE_INLINE TVector4 Min(const TVector4& VecA, const TVector4& VecB)
		{
			return TVector4(
				VecA.X < VecB.X ? VecA.X : VecB.X,
				VecA.Y < VecB.Y ? VecA.Y : VecB.Y,
				VecA.Z < VecB.Z ? VecA.Z : VecB.Z,
				VecA.W < VecB.W ? VecA.W : VecB.W);
		}

		static TRNT_FORCE_INLINE TVector4 Max(const TVector4& VecA, const TVector4& VecB)
		{
			return TVector4(
				VecA.X > VecB.X ? VecA.X : VecB.X,
				VecA.Y > VecB.Y ? VecA.Y : VecB.Y,
				VecA.Z > VecB.Z ? VecA.Z : VecB.Z,
				VecA.W > VecB.W ? VecA.W : VecB.W);
		}

		static TRNT_FORCE_INLINE TVector4 Clamp(const TVector4& Vec, const TVector4& MinV, const TVector4& MaxV)
		{
			return TVector4<T>(
				TNsMath::Clamp<T>(Vec.X, MinV.X, MaxV.X),
				TNsMath::Clamp<T>(Vec.Y, MinV.Y, MaxV.Y),
				TNsMath::Clamp<T>(Vec.Z, MinV.Z, MaxV.Z),
				TNsMath::Clamp<T>(Vec.W, MinV.W, MaxV.W));
		}

		template<typename U = TFloat>
		static TRNT_FORCE_INLINE TVector4 Lerp(const TVector4& From, const TVector4& To, U Amount)
		{
			return TVector4(
				TNsMath::Lerp<T, U>(From.X, To.X, Amount),
				TNsMath::Lerp<T, U>(From.Y, To.Y, Amount),
				TNsMath::Lerp<T, U>(From.Z, To.Z, Amount),
				TNsMath::Lerp<T, U>(From.W, To.W, Amount));
		}

	public:
		union
		{
			struct
			{
				T X;
				T Y;
				T Z;
				T W;
			};

			T XYZW[VecSize];
		};
	};

	template<typename T>
	const TVector4<T> ZeroVec = TVector4<T>{ (T)0, (T)0, (T)0, (T)0 };
	template<typename T>
	const TVector4<T> OneVec = TVector4<T>{ (T)1, (T)1, (T)1, (T)1 };
	template<typename T>
	const TVector4<T> UnitXVec = TVector4<T>{ (T)1, (T)0, (T)0, (T)0 };
	template<typename T>
	const TVector4<T> UnitYVec = TVector4<T>{ (T)0, (T)1, (T)0, (T)0 };
	template<typename T>
	const TVector4<T> UnitZVec = TVector4<T>{ (T)0, (T)0, (T)1, (T)0 };
	template<typename T>
	const TVector4<T> UnitWVec = TVector4<T>{ (T)0, (T)0, (T)0, (T)1 };

	using TVector4f = TVector4<TFloat>;
	using TVector4d = TVector4<TDouble>;
	using TVector4ld = TVector4<TLongDouble>;

	using TVector4i = TVector4<TInt32>;
	using TVector4u = TVector4<TUInt32>;
} // namespace TNsMath

template<>
class TIsPODType<TNsMath::TVector4f> : public TTrueType
{};

template<>
class TIsPODType<TNsMath::TVector4d> : public TTrueType
{};

template<>
class TIsPODType<TNsMath::TVector4ld> : public TTrueType
{};

template<>
class TIsPODType<TNsMath::TVector4i> : public TTrueType
{};

template<>
class TIsPODType<TNsMath::TVector4u> : public TTrueType
{};

namespace TNsHash
{
	// ==> https://github.com/FlaxEngine/FlaxEngine/blob/master/Source/Engine/Core/Math/Vector4.h#L541
	template<typename T>
	inline TSize_T GetHashCode(const TNsMath::TVector4<T>& Vec)
	{
		return static_cast<TSize_T>((((((*(TUInt32*)&Vec.X * 397) ^ *(TUInt32*)&Vec.Y) * 397) ^ *(TUInt32*)&Vec.Z) * 397) ^ *(TUInt32*)&Vec.W);
	}
} // namespace TNsHash