#pragma once

#include "Math.h"
#include "Vector3.h"

#include <cstring>

namespace TNsMath
{
	template<typename T>
	class TMatrix3x3
	{
	public:
		using ComponentType = T;

		static TRNT_CONSTEXPR TInt32 NumColumns = 3;
		static TRNT_CONSTEXPR TInt32 NumRows = 3;
		static TRNT_CONSTEXPR TInt32 ElementCount = NumColumns * NumRows;

	public:
		TRNT_FORCE_INLINE TMatrix3x3() = default;

		explicit TRNT_FORCE_INLINE TMatrix3x3(T Val)
			: M00(Val), M01(Val), M02(Val), M10(Val), M11(Val), M12(Val), M20(Val), M21(Val), M22(Val)
		{}

		TRNT_FORCE_INLINE TMatrix3x3(T M00, T M01, T M02, T M10, T M11, T M12, T M20, T M21, T M22)
			: M00(M00), M01(M01), M02(M02), M10(M10), M11(M11), M12(M12), M20(M20), M21(M21), M22(M22)
		{}

		TRNT_FORCE_INLINE TMatrix3x3(const TVector3<T>& Vec0, const TVector3<T>& Vec1, const TVector3<T>& Vec2)
			: M00(Vec0.X), M01(Vec0.Y), M02(Vec0.Z), M10(Vec1.X), M11(Vec1.Y), M12(Vec1.Z), M20(Vec2.X), M21(Vec2.Y), M22(Vec2.Z)
		{}

		TRNT_FORCE_INLINE TMatrix3x3(const TMatrix3x3&) = default;
		TRNT_FORCE_INLINE TMatrix3x3(TMatrix3x3&&) noexcept = default;

		TRNT_FORCE_INLINE TMatrix3x3& operator=(const TMatrix3x3&) = default;
		TRNT_FORCE_INLINE TMatrix3x3& operator=(TMatrix3x3&&) noexcept = default;

	public:
		TRNT_FORCE_INLINE T At(TInt32 Idx) const
		{
			return A[Idx];
		}

		TRNT_FORCE_INLINE T& At(TInt32 Idx)
		{
			return A[Idx];
		}

		TRNT_FORCE_INLINE T At(TInt32 Row, TInt32 Col) const
		{
			return M[Row][Col];
		}

		TRNT_FORCE_INLINE T& At(TInt32 Row, TInt32 Col)
		{
			return M[Row][Col];
		}

		TRNT_FORCE_INLINE T operator()(TInt32 Row, TInt32 Col) const
		{
			return M[Row][Col];
		}

		TRNT_FORCE_INLINE T& operator()(TInt32 Row, TInt32 Col)
		{
			return M[Row][Col];
		}

	public:
		static const TMatrix3x3 ZeroMat3x3;
		static const TMatrix3x3 IdentityMat3x3;

		TRNT_FORCE_INLINE void SetZero()
		{
			memset(A, 0, sizeof(A));
		}

		static TRNT_FORCE_INLINE TMatrix3x3<T> MakeZero()
		{
			return ZeroMat3x3;
		}

		TRNT_FORCE_INLINE void SetIdentity()
		{
			M00 = (T)1;
			M01 = (T)0;
			M02 = (T)0;
			M10 = (T)0;
			M11 = (T)1;
			M12 = (T)0;
			M20 = (T)0;
			M21 = (T)0;
			M22 = (T)1;
		}

		static TRNT_FORCE_INLINE TMatrix3x3<T> MakeIdentity()
		{
			return IdentityMat3x3;
		}

	public:
		TRNT_FORCE_INLINE T* GetData()
		{
			return A;
		}

		TRNT_FORCE_INLINE const T* GetData() const
		{
			return A;
		}

		TRNT_FORCE_INLINE void SetRow(TInt32 X, T MX0, T MX1, T MX2)
		{
			M[X][0] = MX0;
			M[X][1] = MX1;
			M[X][2] = MX2;
		}

		TRNT_FORCE_INLINE void SetRow(TInt32 X, const TVector3<T>& Vec)
		{
			M[X][0] = Vec.X;
			M[X][1] = Vec.Y;
			M[X][2] = Vec.Z;
		}

		TRNT_FORCE_INLINE TVector3<T> GetRow(TInt32 X) const
		{
			return TVector3<T>(M[X][0], M[X][1], M[X][2]);
		}

		TRNT_FORCE_INLINE void SetColumn(TInt32 X, T M0X, T M1X, T M2X)
		{
			M[0][X] = M0X;
			M[1][X] = M1X;
			M[2][X] = M2X;
		}

		TRNT_FORCE_INLINE void SetColumn(TInt32 X, const TVector3<T>& Vec)
		{
			M[0][X] = Vec.X;
			M[1][X] = Vec.Y;
			M[2][X] = Vec.Z;
		}

		TRNT_FORCE_INLINE TVector3<T> GetColumn(TInt32 X) const
		{
			return TVector3<T>(M[0][X], M[1][X], M[2][X]);
		}

	public:
		TRNT_FORCE_INLINE T GetDeterminant() const
		{
			return (M00 * M11 * M22) + (M01 * M12 * M20) + (M02 * M10 * M21) - (M02 * M11 * M20) - (M00 * M12 * M21) - (M01 * M10 * M22);
		}

		TRNT_FORCE_INLINE void Transpose()
		{
			T Tmp = M01;
			M01 = M10;
			M10 = Tmp;

			Tmp = M02;
			M02 = M20;
			M20 = Tmp;

			Tmp = M12;
			M12 = M21;
			M21 = Tmp;
		}

		TRNT_FORCE_INLINE TMatrix3x3 GetTransposed() const
		{
			TMatrix3x3 Res;
			Res.M00 = M00, Res.M01 = M10, Res.M02 = M20;
			Res.M10 = M01, Res.M11 = M11, Res.M12 = M21;
			Res.M20 = M02, Res.M21 = M12, Res.M22 = M22;
			return Res;
		}

		TBool Invert()
		{
			// Calculate determinant
			const T Det = (M00 * M11 * M22) + (M01 * M12 * M20) + (M02 * M10 * M21) - (M02 * M11 * M20) - (M00 * M12 * M21) - (M01 * M10 * M22);

			if (TNsMath::IsNearlyZero<T>(Det, 1E-20f))
			{
				return false;
			}

			const T InvDet = (T)1 / Det;

			TMatrix3x3 Res;
			////
			Res.M00 = (M11 * M22 - M21 * M12) * InvDet;
			Res.M01 = (M02 * M21 - M01 * M22) * InvDet;
			Res.M02 = (M01 * M12 - M02 * M11) * InvDet;
			////
			Res.M10 = (M12 * M20 - M10 * M22) * InvDet;
			Res.M11 = (M00 * M22 - M02 * M20) * InvDet;
			Res.M12 = (M10 * M02 - M00 * M12) * InvDet;
			////
			Res.M20 = (M10 * M21 - M20 * M11) * InvDet;
			Res.M21 = (M20 * M01 - M00 * M21) * InvDet;
			Res.M22 = (M00 * M11 - M10 * M01) * InvDet;
			////
			*this = Res;

			return true;
		}

		TRNT_FORCE_INLINE TMatrix3x3 GetInverted() const
		{
			const T Det = (M00 * M11 * M22) + (M01 * M12 * M20) + (M02 * M10 * M21) - (M02 * M11 * M20) - (M00 * M12 * M21) - (M01 * M10 * M22);

			if (TNsMath::IsNearlyZero<T>(Det, 1E-20f))
			{
				return *this;
			}

			const T InvDet = (T)1 / Det;

			TMatrix3x3 Res;
			////
			Res.M00 = (M11 * M22 - M21 * M12) * InvDet;
			Res.M01 = (M02 * M21 - M01 * M22) * InvDet;
			Res.M02 = (M01 * M12 - M02 * M11) * InvDet;
			////
			Res.M10 = (M12 * M20 - M10 * M22) * InvDet;
			Res.M11 = (M00 * M22 - M02 * M20) * InvDet;
			Res.M12 = (M10 * M02 - M00 * M12) * InvDet;
			////
			Res.M20 = (M10 * M21 - M20 * M11) * InvDet;
			Res.M21 = (M20 * M01 - M00 * M21) * InvDet;
			Res.M22 = (M00 * M11 - M10 * M01) * InvDet;
			////

			return Res;
		}

		TRNT_FORCE_INLINE void Adjoint()
		{
			TMatrix3x3 Tmp = *this;

			M00 = Tmp.M11 * Tmp.M22 - Tmp.M12 * Tmp.M21;
			M01 = Tmp.M12 * Tmp.M20 - Tmp.M10 * Tmp.M22;
			M02 = Tmp.M10 * Tmp.M21 - Tmp.M11 * Tmp.M20;
			M10 = Tmp.M21 * Tmp.M02 - Tmp.M22 * Tmp.M01;
			M11 = Tmp.M22 * Tmp.M00 - Tmp.M20 * Tmp.M02;
			M12 = Tmp.M20 * Tmp.M01 - Tmp.M21 * Tmp.M00;
			M20 = Tmp.M01 * Tmp.M12 - Tmp.M02 * Tmp.M11;
			M21 = Tmp.M02 * Tmp.M10 - Tmp.M00 * Tmp.M12;
			M22 = Tmp.M00 * Tmp.M11 - Tmp.M01 * Tmp.M10;
		}

		TRNT_FORCE_INLINE TMatrix3x3 GetAdjoint() const
		{
			TMatrix3x3 Res = *this;
			Res.M00 = M11 * M22 - M12 * M21;
			Res.M01 = M12 * M20 - M10 * M22;
			Res.M02 = M10 * M21 - M11 * M20;
			Res.M10 = M21 * M02 - M22 * M01;
			Res.M11 = M22 * M00 - M20 * M02;
			Res.M12 = M20 * M01 - M21 * M00;
			Res.M20 = M01 * M12 - M02 * M11;
			Res.M21 = M02 * M10 - M00 * M12;
			Res.M22 = M00 * M11 - M01 * M10;
			return Res;
		}

	public:
		TRNT_FORCE_INLINE TBool IsZero() const
		{
			for (TInt32 i = 0; i < ElementCount; ++i)
			{
				if (A[i] != (T)0)
				{
					return false;
				}
			}

			return true;
		}

		TRNT_FORCE_INLINE TBool IsNearlyZero(T Tolerance = TNsMath::SmallTolerance<T>) const
		{
			for (TInt32 i = 0; i < ElementCount; ++i)
			{
				if (!TNsMath::IsNearlyZero<T>(A[i], Tolerance))
				{
					return false;
				}
			}

			return true;
		}

		TRNT_FORCE_INLINE TBool IsNaN() const
		{
			for (TInt32 i = 0; i < ElementCount; ++i)
			{
				if (TNsMath::IsNaN<T>(A[i]))
				{
					return true;
				}
			}

			return false;
		}

		TRNT_FORCE_INLINE TBool IsInfinite() const
		{
			for (TInt32 i = 0; i < ElementCount; ++i)
			{
				if (TNsMath::IsInfinite<T>(A[i]))
				{
					return true;
				}
			}

			return false;
		}

		TRNT_FORCE_INLINE TBool IsFinite() const
		{
			for (TInt32 i = 0; i < ElementCount; ++i)
			{
				if (TNsMath::IsFinite(A[i]))
				{
					return true;
				}
			}

			return false;
		}

#define TRNT_NEARLY_EQ(T, X, Y, Tolerance) (TNsMath::Abs<T>(X - Y) <= Tolerance)

		TBool IsEquals(const TMatrix3x3& M, T Tolerance = TNsMath::SmallTolerance<T>) const
		{
			return TRNT_NEARLY_EQ(T, M00, M.M00, Tolerance) && TRNT_NEARLY_EQ(T, M01, M.M01, Tolerance) && TRNT_NEARLY_EQ(T, M02, M.M02, Tolerance) &&
				   TRNT_NEARLY_EQ(T, M10, M.M10, Tolerance) && TRNT_NEARLY_EQ(T, M11, M.M11, Tolerance) && TRNT_NEARLY_EQ(T, M12, M.M12, Tolerance) &&
				   TRNT_NEARLY_EQ(T, M20, M.M20, Tolerance) && TRNT_NEARLY_EQ(T, M21, M.M21, Tolerance) && TRNT_NEARLY_EQ(T, M22, M.M22, Tolerance)
		}

#undef TRNT_NEARLY_EQ

	public:
		TBool operator==(const TMatrix3x3& M) const;
		TBool operator!=(const TMatrix3x3& M) const;

		friend TMatrix3x3 operator+(const TMatrix3x3& Lhs, const TMatrix3x3& Rhs);
		friend TMatrix3x3 operator-(const TMatrix3x3& Lhs, const TMatrix3x3& Rhs);
		friend TMatrix3x3 operator*(const TMatrix3x3& Lhs, const TMatrix3x3& Rhs);
		friend TMatrix3x3 operator/(const TMatrix3x3& Lhs, const TMatrix3x3& Rhs);
		friend TMatrix3x3 operator*(const TMatrix3x3& Lhs, T Rhs);
		friend TMatrix3x3 operator/(const TMatrix3x3& Lhs, T Rhs);
		friend TMatrix3x3 operator*(T Lhs, const TMatrix3x3& Rhs);
		friend TMatrix3x3 operator/(T Lhs, const TMatrix3x3& Rhs);
		friend TVector3<T> operator*(const TMatrix3x3& M, const TVector3<T>& V);
		friend TVector3<T> operator*(const TVector3<T>& V, const TMatrix3x3& M);

		TMatrix3x3& operator+=(const TMatrix3x3& M);
		TMatrix3x3& operator-=(const TMatrix3x3& M);
		TMatrix3x3& operator*=(const TMatrix3x3& M);
		TMatrix3x3& operator*=(T Val);
		TMatrix3x3& operator/=(T Val);

		TMatrix3x3 operator-() const;

	public:
		union
		{
			struct
			{
				T M00, M01, M02;
				T M10, M11, M12;
				T M20, M21, M22;
			};

			T M[NumRows][NumColumns];
			T A[ElementCount];
		};
	};

	template<typename T>
	const TMatrix3x3<T> TMatrix3x3<T>::ZeroMat3x3 = TMatrix3x3<T>(0);

	template<typename T>
	const TMatrix3x3<T> TMatrix3x3<T>::ZeroMat3x3 = TMatrix3x3<T>((T)1, (T)0, (T)0, (T)0, (T)1, (T)0, (T)0, (T)0, (T)1);

	template<typename T>
	TBool TMatrix3x3<T>::operator==(const TMatrix3x3<T>& M) const
	{
		return M00 == M.M00 && M01 == M.M01 && M02 == M.M02 &&
			   M10 == M.M10 && M11 == M.M11 && M12 == M.M12 &&
			   M20 == M.M20 && M21 == M.M21 && M22 == M.M22
	}

	template<typename T>
	TBool TMatrix3x3<T>::operator!=(const TMatrix3x3& M) const
	{
		return !operator==(M);
	}

	template<typename T>
	TMatrix3x3<T> operator+(const TMatrix3x3<T>& Lhs, const TMatrix3x3<T>& Rhs)
	{
		return TMatrix3x3<T>(
			Lhs.M00 + Rhs.M00,
			Lhs.M01 + Rhs.M01,
			Lhs.M02 + Rhs.M02,
			Lhs.M10 + Rhs.M10,
			Lhs.M11 + Rhs.M11,
			Lhs.M12 + Rhs.M12,
			Lhs.M20 + Rhs.M20,
			Lhs.M21 + Rhs.M21,
			Lhs.M22 + Rhs.M22);
	}

	template<typename T>
	TMatrix3x3<T> operator-(const TMatrix3x3<T>& Lhs, const TMatrix3x3<T>& Rhs)
	{
		return TMatrix3x3<T>(
			Lhs.M00 - Rhs.M00,
			Lhs.M01 - Rhs.M01,
			Lhs.M02 - Rhs.M02,
			Lhs.M10 - Rhs.M10,
			Lhs.M11 - Rhs.M11,
			Lhs.M12 - Rhs.M12,
			Lhs.M20 - Rhs.M20,
			Lhs.M21 - Rhs.M21,
			Lhs.M22 - Rhs.M22);
	}

	template<typename T>
	TMatrix3x3<T> operator*(const TMatrix3x3<T>& Lhs, const TMatrix3x3<T>& Rhs)
	{
		return TMatrix3x3(
			Lhs.M00 * Rhs.M00 + Lhs.M01 * Rhs.M10 + Lhs.M02 * Rhs.M20,
			Lhs.M00 * Rhs.M01 + Lhs.M01 * Rhs.M11 + Lhs.M02 * Rhs.M21,
			Lhs.M00 * Rhs.M02 + Lhs.M01 * Rhs.M12 + Lhs.M02 * Rhs.M22,
			Lhs.M10 * Rhs.M00 + Lhs.M11 * Rhs.M10 + Lhs.M12 * Rhs.M20,
			Lhs.M10 * Rhs.M01 + Lhs.M11 * Rhs.M11 + Lhs.M12 * Rhs.M21,
			Lhs.M10 * Rhs.M02 + Lhs.M11 * Rhs.M12 + Lhs.M12 * Rhs.M22,
			Lhs.M20 * Rhs.M00 + Lhs.M21 * Rhs.M10 + Lhs.M22 * Rhs.M20,
			Lhs.M20 * Rhs.M01 + Lhs.M21 * Rhs.M11 + Lhs.M22 * Rhs.M21,
			Lhs.M20 * Rhs.M02 + Lhs.M21 * Rhs.M12 + Lhs.M22 * Rhs.M22);
	}

	template<typename T>
	TMatrix3x3<T> operator*(const TMatrix3x3<T>& Lhs, T Rhs)
	{
		return TMatrix3x3<T>(
			Lhs.M00 * Rhs,
			Lhs.M01 * Rhs,
			Lhs.M02 * Rhs,
			Lhs.M10 * Rhs,
			Lhs.M11 * Rhs,
			Lhs.M12 * Rhs,
			Lhs.M20 * Rhs,
			Lhs.M21 * Rhs,
			Lhs.M22 * Rhs);
	}

	template<typename T>
	TMatrix3x3<T> operator/(const TMatrix3x3<T>& Lhs, T Rhs)
	{
		const T InvRhs = (T)1.0 / Rhs;
		return TMatrix3x3<T>(
			Lhs.M00 * InvRhs,
			Lhs.M01 * InvRhs,
			Lhs.M02 * InvRhs,
			Lhs.M10 * InvRhs,
			Lhs.M11 * InvRhs,
			Lhs.M12 * InvRhs,
			Lhs.M20 * InvRhs,
			Lhs.M21 * InvRhs,
			Lhs.M22 * InvRhs);
	}

	template<typename T>
	TMatrix3x3<T> operator*(T Lhs, const TMatrix3x3<T>& Rhs)
	{
		return TMatrix3x3<T>(
			Lhs * Rhs.M00,
			Lhs * Rhs.M01,
			Lhs * Rhs.M02,
			Lhs * Rhs.M10,
			Lhs * Rhs.M11,
			Lhs * Rhs.M12,
			Lhs * Rhs.M20,
			Lhs * Rhs.M21,
			Lhs * Rhs.M22);
	}

	template<typename T>
	TMatrix3x3<T> operator/(T Lhs, const TMatrix3x3<T>& Rhs)
	{
		const T InvLhs = (T)1.0 / Lhs;
		return TMatrix3x3<T>(
			InvLhs * Rhs.M00,
			InvLhs * Rhs.M01,
			InvLhs * Rhs.M02,
			InvLhs * Rhs.M10,
			InvLhs * Rhs.M11,
			InvLhs * Rhs.M12,
			InvLhs * Rhs.M20,
			InvLhs * Rhs.M21,
			InvLhs * Rhs.M22);
	}

	template<typename T>
	TVector3<T> operator*(const TMatrix3x3<T>& M, const TVector3<T>& V)
	{
		return TVector3<T>(
			static_cast<T>(M.M00 * V.X + M.M01 * V.Y + M.M02 * V.Z),
			static_cast<T>(M.M10 * V.X + M.M11 * V.Y + M.M12 * V.Z),
			static_cast<T>(M.M20 * V.X + M.M21 * V.Y + M.M22 * V.Z));
	}

	template<typename T>
	TVector3<T> operator*(const TVector3<T>& V, const TMatrix3x3<T>& M)
	{
		return TVector3<T>(
			static_cast<T>(V.X * M.M00 + V.Y * M.M10 + V.Z * M.M20),
			static_cast<T>(V.X * M.M01 + V.Y * M.M11 + V.Z * M.M21),
			static_cast<T>(V.X * M.M02 + V.Y * M.M12 + V.Z * M.M22));
	}

	template<typename T>
	TMatrix3x3<T>& TMatrix3x3<T>::operator+=(const TMatrix3x3<T>& M)
	{
		M00 += M.M00, M01 += M.M01, M02 += M.M02;
		M10 += M.M10, M11 += M.M11, M12 += M.M12;
		M20 += M.M20, M21 += M.M21, M22 += M.M22;
		return *this;
	}

	template<typename T>
	TMatrix3x3<T>& TMatrix3x3<T>::operator-=(const TMatrix3x3<T>& M)
	{
		M00 -= M.M00, M01 -= M.M01, M02 -= M.M02;
		M10 -= M.M10, M11 -= M.M11, M12 -= M.M12;
		M20 -= M.M20, M21 -= M.M21, M22 -= M.M22;
		return *this;
	}

	template<typename T>
	TMatrix3x3<T>& TMatrix3x3<T>::operator*=(const TMatrix3x3<T>& M)
	{
		*this = TMatrix3x3<T>(
			M00 * M.M00 + M01 * M.M10 + M02 * M.M20,
			M00 * M.M01 + M01 * M.M11 + M02 * M.M21,
			M00 * M.M02 + M01 * M.M12 + M02 * M.M22,
			M10 * M.M00 + M11 * M.M10 + M12 * M.M20,
			M10 * M.M01 + M11 * M.M11 + M12 * M.M21,
			M10 * M.M02 + M11 * M.M12 + M12 * M.M22,
			M20 * M.M00 + M21 * M.M10 + M22 * M.M20,
			M20 * M.M01 + M21 * M.M11 + M22 * M.M21,
			M20 * M.M02 + M21 * M.M12 + M22 * M.M22);

		return *this;
	}

	template<typename T>
	TMatrix3x3<T>& TMatrix3x3<T>::operator*=(T Val)
	{
		M00 *= Val, M01 *= Val, M02 *= Val;
		M10 *= Val, M11 *= Val, M12 *= Val;
		M20 *= Val, M21 *= Val, M22 *= Val;
		return *this;
	}

	template<typename T>
	TMatrix3x3<T>& TMatrix3x3<T>::operator/=(T Val)
	{
		const T InvVal = (T)1.0 / Val;
		M00 *= InvVal, M01 *= InvVal, M02 *= InvVal;
		M10 *= InvVal, M11 *= InvVal, M12 *= InvVal;
		M20 *= InvVal, M21 *= InvVal, M22 *= InvVal;
		return *this;
	}

	template<typename T>
	TMatrix3x3<T> TMatrix3x3<T>::operator-() const
	{
		return TMatrix3x3<T>(-M00, -M01, -M02, -M10, -M11, -M12, -M20, -M21, -M22);
	}
} // namespace TNsMath
