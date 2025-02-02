#pragma once

#include "Math.h"
#include "Trinity/Core/TypeTraits/EnableIf.h"

namespace TNsMath
{
	template<typename T>
	class TVector3
	{
	public:
		using ComponentType = T;

		static TRNT_CONSTEXPR TInt32 VecSize = 3;

	public:
		// default constructor
		TRNT_FORCE_INLINE TVector3() = default;

		// copy & move constructors
		TRNT_FORCE_INLINE TVector3(const TVector3&) = default;
		TRNT_FORCE_INLINE TVector3(TVector3&&) noexcept = default;

		// other constructors
		TRNT_FORCE_INLINE TVector3(T X, T Y, T Z)
			: X(X), Y(Y), Z(Z)
		{}

		TRNT_FORCE_INLINE TVector3(const TVector2<T>& XY, T Z)
			: X(XY.X), Y(XY.Y), Z(Z)
		{}

		template<typename U, typename TEnableIf<TNot<TAreTheSameType<T, U>>::Value, int>::Type = 0>
		explicit TRNT_FORCE_INLINE TVector3(const TVector3<U>& Vec)
			: X((T)Vec.X), Y((T)Vec.Y), Z((T)Vec.Z)
		{}

		explicit TRNT_FORCE_INLINE TVector3(T XYZ)
			: X(XYZ), Y(XYZ), Z(XYZ)
		{}

		explicit TRNT_FORCE_INLINE TVector3(const T* XYZ)
			: X(XYZ[0]), Y(XYZ[1]), Z(XYZ[2])
		{}

		// destructor
		~TVector3() = default;

	public:
		TRNT_FORCE_INLINE TVector3& operator=(const TVector3&) = default;
		TRNT_FORCE_INLINE TVector3& operator=(TVector3&&) noexcept = default;

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

		TRNT_FORCE_INLINE void Set(T Val)
		{
			X = Val, Y = Val, Z = Val;
		}

		TRNT_FORCE_INLINE void Set(T X, T Y, T Z)
		{
			this->X = X, this->Y = Y, this->Z = Z;
		}

		TRNT_FORCE_INLINE T* GetData()
		{
			return XYZ;
		}

		TRNT_FORCE_INLINE const T* GetData() const
		{
			return XYZ;
		}

		TRNT_FORCE_INLINE T operator[](TInt32 Idx) const
		{
			return XYZ[Idx];
		}

		TRNT_FORCE_INLINE T& operator[](TInt32 Idx)
		{
			return XYZ[Idx];
		}

		TRNT_FORCE_INLINE TBool operator==(const TVector3& Vec) const
		{
			return X == Vec.X && Y == Vec.Y && Z == Vec.Z;
		}

		TRNT_FORCE_INLINE TBool operator!=(const TVector3& Vec) const
		{
			return X != Vec.X || Y != Vec.Y || Z != Vec.Z;
		}

		TRNT_FORCE_INLINE TBool operator>(const TVector3& Vec) const
		{
			return X > Vec.X && Y > Vec.Y && Z > Vec.Z;
		}

		TRNT_FORCE_INLINE TBool operator>=(const TVector3& Vec) const
		{
			return X >= Vec.X && Y >= Vec.Y && Z >= Vec.Z;
		}

		TRNT_FORCE_INLINE TBool operator<(const TVector3& Vec) const
		{
			return X < Vec.X && Y < Vec.Y && Z < Vec.Z;
		}

		TRNT_FORCE_INLINE TBool operator<=(const TVector3& Vec) const
		{
			return X <= Vec.X && Y <= Vec.Y && Z <= Vec.Z;
		}

		TRNT_FORCE_INLINE TVector3 operator-() const
		{
			return TVector3(-X, -Y, -Z);
		}

		TRNT_FORCE_INLINE TVector3 operator+(const TVector3& Vec) const
		{
			return TVector3(X + Vec.X, Y + Vec.Y, Z + Vec.Z);
		}

		TRNT_FORCE_INLINE TVector3 operator-(const TVector3& Vec) const
		{
			return TVector3(X - Vec.X, Y - Vec.Y, Z - Vec.Z);
		}

		TRNT_FORCE_INLINE TVector3 operator*(const TVector3& Vec) const
		{
			return TVector3(X * Vec.X, Y * Vec.Y, Z * Vec.Z);
		}

		TRNT_FORCE_INLINE TVector3 operator/(const TVector3& Vec) const
		{
			return TVector3(X / Vec.X, Y / Vec.Y, Z / Vec.Z);
		}

		TRNT_FORCE_INLINE TVector3 operator+(T Val) const
		{
			return TVector3(X + Val, Y + Val, Z + Val);
		}

		TRNT_FORCE_INLINE TVector3 operator-(T Val) const
		{
			return TVector3(X - Val, Y - Val, Z - Val);
		}

		TRNT_FORCE_INLINE TVector3 operator*(T Val) const
		{
			return TVector3(X * Val, Y * Val, Z * Val);
		}

		TRNT_FORCE_INLINE TVector3 operator/(T Val) const
		{
			return TVector3(X / Val, Y / Val, Z / Val);
		}

		TRNT_FORCE_INLINE TVector3& operator+=(const TVector3& Vec) const
		{
			X += Vec.X, Y += Vec.Y, Z += Vec.Z;
			return *this;
		}

		TRNT_FORCE_INLINE TVector3& operator-=(const TVector3& Vec) const
		{
			X -= Vec.X, Y -= Vec.Y, Z -= Vec.Z;
			return *this;
		}

		TRNT_FORCE_INLINE TVector3& operator*=(const TVector3& Vec) const
		{
			X *= Vec.X, Y *= Vec.Y, Z *= Vec.Z;
			return *this;
		}

		TRNT_FORCE_INLINE TVector3& operator/=(const TVector3& Vec) const
		{
			X /= Vec.X, Y /= Vec.Y, Z /= Vec.Z;
			return *this;
		}

		TRNT_FORCE_INLINE TVector3& operator+=(T Val) const
		{
			X += Val, Y += Val, Z += Val;
			return *this;
		}

		TRNT_FORCE_INLINE TVector3& operator-=(T Val) const
		{
			X -= Val, Y -= Val, Z -= Val;
			return *this;
		}

		TRNT_FORCE_INLINE TVector3& operator*=(T Val) const
		{
			X *= Val, Y *= Val, Z *= Val;
			return *this;
		}

		TRNT_FORCE_INLINE TVector3& operator/=(T Val) const
		{
			X /= Val, Y /= Val, Z /= Val;
			return *this;
		}

	public:
		TRNT_FORCE_INLINE TBool IsNormalized() const
		{
			return TNsMath::Abs<TFloat>((X * X + Y * Y + Z * Z) - 1.0f) <= TNsMath::SmallTolerance<TFloat>;
		}

		TRNT_FORCE_INLINE TBool IsZero() const
		{
			return X == (T)0 && Y == (T)0 && Z == (T)0;
		}

		TRNT_FORCE_INLINE TBool IsNearlyZero(T Tolerance = TNsMath::SmallTolerance<T>) const
		{
			return TNsMath::Abs<T>(X) <= Tolerance && TNsMath::Abs<T>(Y) <= Tolerance && TNsMath::Abs<T>(Z) <= Tolerance;
		}

		TRNT_FORCE_INLINE TBool IsNaN() const
		{
			return TNsMath::IsNaN<T>(X) || TNsMath::IsNaN<T>(Y) || TNsMath::IsNaN<T>(Z);
		}

		TRNT_FORCE_INLINE TBool IsInfinite() const
		{
			return TNsMath::IsInfinite<T>(X) || TNsMath::IsInfinite<T>(Y) || TNsMath::IsInfinite<T>(Z);
		}

		TRNT_FORCE_INLINE TBool IsFinite() const
		{
			return TNsMath::IsFinite<T>(X) || TNsMath::IsFinite<T>(Y) || TNsMath::IsFinite<T>(Z);
		}

		TRNT_FORCE_INLINE TBool IsEquals(const TVector3& Vec, T Tolerance = TNsMath::SmallTolerance<T>) const
		{
			return TNsMath::Abs<T>(X - Vec.X) <= Tolerance && TNsMath::Abs<T>(Y - Vec.Y) <= Tolerance && TNsMath::Abs<T>(Z - Vec.Z) <= Tolerance;
		}

	public:
		TRNT_FORCE_INLINE T GetAverage() const
		{
			return (X + Y + Z) * TNsMath::Invert<VecSize, T>;
		}

		TRNT_FORCE_INLINE T GetSumValues() const
		{
			return X + Y + Z;
		}

		TRNT_FORCE_INLINE T GetLength() const
		{
			return TNsMath::Sqrt<T>(X * X + Y * Y + Z * Z);
		}

		TRNT_FORCE_INLINE T GetLengthSquared() const
		{
			return X * X + Y * Y + Z * Z;
		}

		TRNT_FORCE_INLINE T GetMaxValue() const
		{
			return TNsMath::Max<T>(X, Y, Z);
		}

		TRNT_FORCE_INLINE T GetMinValue() const
		{
			return TNsMath::Min<T>(X, Y, Z);
		}

		TRNT_FORCE_INLINE TVector3 GetAbsoluteVector() const
		{
			return TVector3(X > 0 ? X : -X, Y > 0 ? Y : -Y, Z > 0 ? Z : -Z);
		}

		TRNT_FORCE_INLINE TVector3 GetNormalizedVector() const
		{
			TVector3 Ret(X, Y, Z);
			Ret.Normalize();
			return Ret;
		}

		void Normalize() const
		{
			const T VecLength = TNsMath::Sqrt<T>(X * X + Y * Y + Z * Z);
			
			if (VecLength >= TNsMath::SmallTolerance<T>)
			{
				const T Scale = (T)1.0f / VecLength;
				X *= Scale;
				Y *= Scale;
				Z *= Scale;
			}
		}

		void NormalizeFast() const
		{
			const T Scale = (T)1.0f / TNsMath::Sqrt<T>(X * X + Y * Y + Z * Z);
			X *= Scale;
			Y *= Scale;
			Z *= Scale;
			return;
		}

		TRNT_FORCE_INLINE TVector3 Reflect(const TVector3& Normal) const
		{
			return ((*this) - ((T)2 * Dot(*this, Normal) * Normal));
		}

	public:
		static const TVector3 ZeroVec;
		static const TVector3 OneVec;
		static const TVector3 UnitXVec;
		static const TVector3 UnitYVec;
		static const TVector3 UnitZVec;

		static const TVector3 UpVec;
		static const TVector3 DownVec;
		static const TVector3 RightVec;
		static const TVector3 LeftVec;
		static const TVector3 ForwardVec;
		static const TVector3 BackwardVec;

	public:
		static TRNT_INLINE TVector3 Zero()
		{
			return ZeroVec;
		}

		static TRNT_INLINE TVector3 One()
		{
			return OneVec;
		}

		static TRNT_INLINE TVector3 UnitX()
		{
			return UnitXVec;
		}

		static TRNT_INLINE TVector3 UnitY()
		{
			return UnitYVec;
		}

		static TRNT_INLINE TVector3 UnitZ()
		{
			return UnitZVec;
		}

		static TRNT_INLINE TVector3 Up()
		{
			return UpVec;
		}

		static TRNT_INLINE TVector3 Down()
		{
			return DownVec;
		}

		static TRNT_INLINE TVector3 Right()
		{
			return RightVec;
		}

		static TRNT_INLINE TVector3 Left()
		{
			return LeftVec;
		}

		static TRNT_INLINE TVector3 Forward()
		{
			return ForwardVec;
		}

		static TRNT_INLINE TVector3 Backward()
		{
			return BackwardVec;
		}

	public:
		static TRNT_FORCE_INLINE T Dot(const TVector3& VecA, const TVector3& VecB)
		{
			return VecA.X * VecB.X + VecA.Y * VecB.Y + VecA.Z * VecB.Z;
		}

		static TRNT_FORCE_INLINE TVector3 Cross(const TVector3& VecA, const TVector3& VecB)
		{
			return TVector3(
				VecA.Y * VecB.Z - VecA.Z * VecB.Y,
				VecA.Z * VecB.X - VecA.X * VecB.Z,
				VecA.X * VecB.Y - VecA.Y * VecB.X);
		}

		static TRNT_FORCE_INLINE TVector3 Min(const TVector3& VecA, const TVector3& VecB)
		{
			return TVector3(
				VecA.X < VecB.X ? VecA.X : VecB.X,
				VecA.Y < VecB.Y ? VecA.Y : VecB.Y,
				VecA.Z < VecB.Z ? VecA.Z : VecB.Z);
		}

		static TRNT_FORCE_INLINE TVector3 Max(const TVector3& VecA, const TVector3& VecB)
		{
			return TVector3(
				VecA.X > VecB.X ? VecA.X : VecB.X,
				VecA.Y > VecB.Y ? VecA.Y : VecB.Y,
				VecA.Z > VecB.Z ? VecA.Z : VecB.Z);
		}

		static TRNT_FORCE_INLINE TVector3 Clamp(const TVector3& Vec, const TVector3& MinV, const TVector3& MaxV)
		{
			return TVector3<T>(
				TNsMath::Clamp<T>(Vec.X, MinV.X, MaxV.X),
				TNsMath::Clamp<T>(Vec.Y, MinV.Y, MaxV.Y),
				TNsMath::Clamp<T>(Vec.Z, MinV.Z, MaxV.Z));
		}

		static T GetDistanceSquared(const TVector3& VecA, const TVector3& VecB)
		{
			const T OffsetX = VecB.X - VecA.X;
			const T OffsetY = VecB.Y - VecA.Y;
			const T OffsetZ = VecB.Z - VecA.Z;
			return OffsetX * OffsetX + OffsetY * OffsetY + OffsetZ * OffsetZ;
		}

		static T GetDistance(const TVector3& VecA, const TVector3& VecB)
		{
			const T OffsetX = VecB.X - VecA.X;
			const T OffsetY = VecB.Y - VecA.Y;
			const T OffsetZ = VecB.Z - VecA.Z;
			return TNsMath::Sqrt<T>(OffsetX * OffsetX + OffsetY * OffsetY + OffsetZ * OffsetZ);
		}

		template<typename FPType>
		static FPType GetAngleAsRadian(const TVector3& A, const TVector3& B)
		{
			return TNsMath::Acos(static_cast<FPType>(TNsMath::Clamp<T>(A.Dot(B), (T)-1, (T)1)));
		}

		template<typename FPType>
		static FPType GetAngleAsDegree(const TVector3& A, const TVector3& B)
		{
			return TNsMath::RadiansToDegrees<FPType>(TNsMath::Acos(static_cast<FPType>(TNsMath::Clamp<T>(A.Dot(B), (T)-1, (T)1))));
		}

		static TVector3 Rotate(const TVector3& Vec, const TVector3& Axis, const T Radian)
		{
			const auto C = std::cos(Radian);
			const auto S = std::sin(Radian);
			return (Vec * C) + (S * Cross(Axis, Vec)) + (Axis * Dot(Axis, Vec)) * (1 - C);
		}

		template<typename U = TFloat>
		static TRNT_FORCE_INLINE TVector3 Lerp(const TVector3& From, const TVector3& To, U Amount)
		{
			return TVector3(
				TNsMath::Lerp<T, U>(From.X, To.X, Amount),
				TNsMath::Lerp<T, U>(From.Y, To.Y, Amount),
				TNsMath::Lerp<T, U>(From.Z, To.Z, Amount))
		}

	public:
		union
		{
			struct
			{
				T X;
				T Y;
				T Z;
			};

			T XYZ[VecSize];
		};
	};

	template<typename T>
	const TVector3<T> ZeroVec = TVector3<T>{ (T)0, (T)0, (T)0 };
	template<typename T>
	const TVector3<T> OneVec = TVector3<T>{ (T)1, (T)1, (T)1 };
	template<typename T>
	const TVector3<T> UnitXVec = TVector3<T>{ (T)1, (T)0, (T)0 };
	template<typename T>
	const TVector3<T> UnitYVec = TVector3<T>{ (T)0, (T)1, (T)0 };
	template<typename T>
	const TVector3<T> UnitZVec = TVector3<T>{ (T)0, (T)0, (T)1 };

	template<typename T>
	const TVector3<T> UpVec = TVector3<T>{ (T)0, (T)1, (T)0 };
	template<typename T>
	const TVector3<T> DownVec = TVector3<T>{ (T)0, (T)-1, (T)0 };
	template<typename T>
	const TVector3<T> RightVec = TVector3<T>{ (T)1, (T)0, (T)0 };
	template<typename T>
	const TVector3<T> LeftVec = TVector3<T>{ (T)-1, (T)0, (T)0 };
	template<typename T>
	const TVector3<T> ForwardVec = TVector3<T>{ (T)0, (T)0, (T)1 };
	template<typename T>
	const TVector3<T> BackwardVec = TVector3<T>{ (T)0, (T)0, (T)-1 };

	using TVector3f = TVector3<TFloat>;
	using TVector3d = TVector3<TDouble>;
	using TVector3ld = TVector3<TLongDouble>;

	using TVector3i = TVector3<TInt32>;
	using TVector3u = TVector3<TUInt32>;
} // namespace TNsMath

template<>
class TIsPODType<TNsMath::TVector3f> : public TTrueType
{};

template<>
class TIsPODType<TNsMath::TVector3d> : public TTrueType
{};

template<>
class TIsPODType<TNsMath::TVector3ld> : public TTrueType
{};

template<>
class TIsPODType<TNsMath::TVector3i> : public TTrueType
{};

template<>
class TIsPODType<TNsMath::TVector3u> : public TTrueType
{};

namespace TNsHash
{
	// ==> https://github.com/FlaxEngine/FlaxEngine/blob/master/Source/Engine/Core/Math/Vector3.h#L891
	template<typename T>
	inline TSize_T GetHashCode(const TNsMath::TVector3<T>& Vec)
	{
		return static_cast<TSize_T>((((*(TUInt32*)&Vec.X * 397) ^ *(TUInt32*)&Vec.Y) * 397) ^ *(TUInt32*)&Vec.Z);
	}
} // namespace TNsHash