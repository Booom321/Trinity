#pragma once

#include <atomic>

#include "Trinity/Core/Types/UniquePtr.h"
#include "Trinity/Core/Assert/AssertionMacros.h"
#include "Trinity/Core/TypeTraits/TypeChooser.h"

namespace TNsSharedPtrDetails
{
	template<TBool EnableThreadSafety>
	class TReferenceCounter
	{
	public:
		using IntegralType = TInt32;
		using CounterType = typename TTypeChooser<EnableThreadSafety, std::atomic<IntegralType>, IntegralType>::Type;

		TRNT_FORCE_INLINE TReferenceCounter() = default;

		TRNT_FORCE_INLINE TReferenceCounter(const TReferenceCounter&) = default;
		TRNT_FORCE_INLINE TReferenceCounter(TReferenceCounter&&) noexcept = default;
		TRNT_FORCE_INLINE TReferenceCounter& operator=(const TReferenceCounter&) = default;
		TRNT_FORCE_INLINE TReferenceCounter& operator=(TReferenceCounter&&) noexcept = default;

		TRNT_FORCE_INLINE ~TReferenceCounter()
		{
#ifdef TRNT_DEBUG
			TRNT_ASSERT(SharedRefCount == 0);
#endif
		}

	public:
		TRNT_FORCE_INLINE void AddSharedReference()
		{
			if constexpr (EnableThreadSafety)
			{
				SharedRefCount.fetch_add(1, std::memory_order_relaxed);
			}
			else
			{
				++SharedRefCount;
			}
		}

		TRNT_FORCE_INLINE void ReleaseSharedReference()
		{
			if constexpr (EnableThreadSafety)
			{
				SharedRefCount.fetch_sub(1, std::memory_order_acq_rel);
			}
			else
			{
				--SharedRefCount;
			}
		}

		TRNT_FORCE_INLINE IntegralType GetSharedReferenceCount()
		{
			if constexpr (EnableThreadSafety)
			{
				return SharedRefCount.load(std::memory_order_relaxed);
			}
			else
			{
				return SharedRefCount;
			}
		}

	public:
		CounterType SharedRefCount{ 0 };
	};

	template<typename Lhs, typename Rhs>
	class TSharedPtrConvertible : public TBoolConstant<TIsConvertible<Lhs*, Rhs*>::Value>
	{};

	template<typename Lhs, typename Rhs>
	class TSharedPtrConvertible<Lhs, Rhs[]> : public TBoolConstant<TIsConvertible<Lhs(*)[], Rhs(*)[]>::Value>
	{};

	template<typename Lhs, typename Rhs, TSize_T Size>
	class TSharedPtrConvertible<Lhs, Rhs[Size]> : public TBoolConstant<TIsConvertible<Lhs(*)[Size], Rhs(*)[Size]>::Value>
	{};
}

template<typename T, TBool EnableThreadSafety = false, typename Deleter = TDefaultDeleter<T>>
class TSharedPtr
{
public:
	using ElementType = typename TRemoveExtent<T>::Type;
	using PointerType = ElementType*;

	using ReferenceCounterType = TNsSharedPtrDetails::TReferenceCounter<EnableThreadSafety>;
	using DeleterType = Deleter;

	template<typename OtherType, TBool EnableThreadSafety, typename OtherDeleterType>
	friend class TSharedPtr;

public:
	TRNT_FORCE_INLINE TSharedPtr()
		: RefCounter(nullptr), Pair(DeleterType(), nullptr)
	{}

	explicit TRNT_FORCE_INLINE TSharedPtr(TNullPtr)
		: RefCounter(nullptr), Pair(DeleterType(), nullptr)
	{}

	template<
		typename OtherType,
		typename TEnableIf<!TIsArray<OtherType>::Value && TNsSharedPtrDetails::TSharedPtrConvertible<OtherType, T>::Value, int>::Type = 0
	>
	explicit TRNT_FORCE_INLINE TSharedPtr(OtherType* Ptr)
		: RefCounter(new ReferenceCounterType()), Pair(DeleterType(), Ptr)
	{
		RefCounter->AddSharedReference();
	}

	template<
		typename OtherType,
		typename OtherDeleterType,
		typename TEnableIf<!TIsArray<OtherType>::Value && TNsSharedPtrDetails::TSharedPtrConvertible<OtherType, T>::Value, int>::Type = 0
	>
	explicit TRNT_FORCE_INLINE TSharedPtr(OtherType* Ptr, const OtherDeleterType& OtherDeleter)
		: RefCounter(new ReferenceCounterType()), Pair(OtherDeleter, Ptr)
	{
		RefCounter->AddSharedReference();
	}

	template<
		typename OtherType,
		typename OtherDeleterType,
		typename TEnableIf<!TIsArray<OtherType>::Value && TNsSharedPtrDetails::TSharedPtrConvertible<OtherType, T>::Value, int>::Type = 0
	>
	explicit TRNT_FORCE_INLINE TSharedPtr(OtherType* Ptr, OtherDeleterType&& OtherDeleter) noexcept
		: RefCounter(new ReferenceCounterType()), Pair(Move(OtherDeleter), Ptr)
	{
		RefCounter->AddSharedReference();
	}

	template<
		typename OtherType,
		typename OtherDeleterType,
		typename TEnableIf<TNsSharedPtrDetails::TSharedPtrConvertible<typename TRemoveExtent<OtherType>::Type, T>::Value, int>::Type = 0
	>
	TRNT_FORCE_INLINE TSharedPtr(const TSharedPtr<OtherType, EnableThreadSafety, OtherDeleterType>& Other)
		: RefCounter(Other.RefCounter), Pair(Other.Pair.GetFirstValue(), Other.Pair.SecondValue)
	{
		RefCounter->AddSharedReference();
	}

	template<
		typename OtherType,
		typename OtherDeleterType,
		typename TEnableIf<TNsSharedPtrDetails::TSharedPtrConvertible<typename TRemoveExtent<OtherType>::Type, T>::Value, int>::Type = 0
	>
	TRNT_FORCE_INLINE TSharedPtr(TSharedPtr<OtherType, EnableThreadSafety, OtherDeleterType>&& Other) noexcept
		: RefCounter(Other.RefCounter), Pair(Move(Other.Pair.GetFirstValue()), Other.Pair.SecondValue)
	{
		Other.RefCounter = nullptr;
		Other.Pair.SecondValue = nullptr;
	}

	TRNT_FORCE_INLINE TSharedPtr(const TSharedPtr& Other)
		: RefCounter(Other.RefCounter), Pair(Move(Other.Pair.GetFirstValue()), Move(Other.Pair.SecondValue))
	{
		RefCounter->AddSharedReference();
	}

	TRNT_FORCE_INLINE TSharedPtr(TSharedPtr&& Other) noexcept
		: RefCounter(Other.RefCounter), Pair(Move(Other.Pair.GetFirstValue()), Other.Pair.SecondValue)
	{
		Other.RefCounter = nullptr;
		Other.Pair.SecondValue = nullptr;
	}

	~TSharedPtr()
	{
		ReleaseSharedReferenceInternal();
	}

public:
	TSharedPtr& operator=(TNullPtr)
	{
		ReleaseSharedReferenceInternal();

		RefCounter = nullptr;
		Pair.SecondValue = nullptr;

		return *this;
	}

	template<
		typename OtherType,
		typename OtherDeleterType,
		typename TEnableIf<TNsSharedPtrDetails::TSharedPtrConvertible<typename TRemoveExtent<OtherType>::Type, T>::Value, int>::Type = 0
	>
	TSharedPtr& operator=(const TSharedPtr<OtherType, EnableThreadSafety, OtherDeleterType>& Other)
	{
		ReleaseSharedReferenceInternal();

		RefCounter = Other.RefCounter;
		Pair.GetFirstValue() = Other.Pair.GetFirstValue();
		Pair.SecondValue = Other.Pair.SecondValue;

		RefCounter->AddSharedReference();

		return *this;
	}

	template<
		typename OtherType,
		typename OtherDeleterType,
		typename TEnableIf<TNsSharedPtrDetails::TSharedPtrConvertible<typename TRemoveExtent<OtherType>::Type, T>::Value, int>::Type = 0
	>
	TSharedPtr& operator=(TSharedPtr<OtherType, EnableThreadSafety, OtherDeleterType>&& Other) noexcept
	{
		ReleaseSharedReferenceInternal();

		RefCounter = Other.RefCounter;
		Pair.GetFirstValue() = Move(Other.Pair.GetFirstValue());
		Pair.SecondValue = Other.Pair.SecondValue;

		Other.RefCounter = nullptr;
		Other.Pair.SecondValue = nullptr;

		return *this;
	}

	TSharedPtr& operator=(const TSharedPtr& Other)
	{
		if (this != &Other)
		{
			ReleaseSharedReferenceInternal();

			RefCounter = Other.RefCounter;
			Pair.GetFirstValue() = Other.Pair.GetFirstValue();
			Pair.SecondValue = Other.Pair.SecondValue;

			RefCounter->AddSharedReference();
		}
		return *this;
	}

	TSharedPtr& operator=(TSharedPtr&& Other) noexcept
	{
		if (this != &Other)
		{
			ReleaseSharedReferenceInternal();

			RefCounter = Other.RefCounter;
			Pair.GetFirstValue() = Move(Other.Pair.GetFirstValue());
			Pair.SecondValue = Other.Pair.SecondValue;

			Other.RefCounter = nullptr;
			Other.Pair.SecondValue = nullptr;
		}
		return *this;
	}

public:
	TRNT_NODISCARD TRNT_FORCE_INLINE PointerType Get() const
	{
		return Pair.SecondValue;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE DeleterType& GetDeleter() noexcept
	{
		return Pair.GetFirstValue();
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE const DeleterType& GetDeleter() const noexcept
	{
		return Pair.GetFirstValue();
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE typename ReferenceCounterType::IntegralType GetSharedReferenceCount() const
	{
		return (RefCounter != nullptr) ? RefCounter->GetSharedReferenceCount() : 0;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE TBool IsUnique() const
	{
		return (RefCounter != nullptr) ? RefCounter->GetSharedReferenceCount() == 1 : false;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE explicit operator bool() const
	{
		return Pair.SecondValue != nullptr;
	}

	template<typename OtherT = T, typename TEnableIf<!TAreTheSameType<OtherT, void>::Value && !TIsArray<OtherT>::Value, int>::Type = 0>
	TRNT_NODISCARD TRNT_FORCE_INLINE PointerType operator->() const
	{
		return Pair.SecondValue;
	}

	template<typename OtherT = T, typename TEnableIf<!TAreTheSameType<OtherT, void>::Value && !TIsArray<OtherT>::Value, int>::Type = 0>
	TRNT_NODISCARD TRNT_FORCE_INLINE decltype(auto) operator*() const noexcept
	{
		return *Pair.SecondValue;
	}

	template<typename OtherT = T, typename TEnableIf<!TAreTheSameType<OtherT, void>::Value&& TIsArray<OtherT>::Value, int>::Type = 0>
	TRNT_NODISCARD TRNT_FORCE_INLINE decltype(auto) operator[](TSize_T Index) const
	{
		return Pair.SecondValue[Index];
	}

public:
	TRNT_FORCE_INLINE void Reset()
	{
		ReleaseSharedReferenceInternal();

		RefCounter = nullptr;
		Pair.SecondValue = nullptr;
	}

private:
	TRNT_FORCE_INLINE void ReleaseSharedReferenceInternal()
	{
		if (RefCounter != nullptr && Pair.SecondValue != nullptr)
		{
			RefCounter->ReleaseSharedReference();

			if (RefCounter->GetSharedReferenceCount() == 0)
			{
				Pair.GetFirstValue()(Pair.SecondValue);
				Pair.SecondValue = nullptr;

				delete RefCounter;
				RefCounter = nullptr;
			}
		}
	}

private:
	ReferenceCounterType* RefCounter;
	TCompressedPair<DeleterType, PointerType> Pair;
};

namespace TNsHash
{
	template<typename T, TBool EnableThreadSafety, typename Deleter>
	inline TSize_T GetHashCode(const TSharedPtr<T, EnableThreadSafety, Deleter>& SharedPtr)
	{
		return GetHashCode<typename TSharedPtr<T, EnableThreadSafety, Deleter>::PointerType>(SharedPtr.Get());
	}
}

template<typename T, TBool EnableThreadSafety, typename... ArgsType>
TRNT_NODISCARD TRNT_FORCE_INLINE typename TEnableIf<!TIsArray<T>::Value, TSharedPtr<T, EnableThreadSafety>>::Type MakeShared(ArgsType&&... Args)
{
	return TSharedPtr<T, EnableThreadSafety>(new T(Forward<ArgsType>(Args)...));
}

template<typename T, TBool EnableThreadSafety>
TRNT_NODISCARD TRNT_FORCE_INLINE typename TEnableIf<TIsUnboundedArray<T>::Value, TSharedPtr<T, EnableThreadSafety>>::Type MakeShared(TSize_T Num)
{
	return TSharedPtr<T, EnableThreadSafety>(new typename TRemoveExtent<T>::Type[Num]());
}

template<typename T, typename... ArgsType>
TRNT_NODISCARD TRNT_FORCE_INLINE typename TEnableIf<!TIsArray<T>::Value, TSharedPtr<T>>::Type MakeShared(ArgsType&&... Args)
{
	return TSharedPtr<T>(new T(Forward<ArgsType>(Args)...));
}

template<typename T>
TRNT_NODISCARD TRNT_FORCE_INLINE typename TEnableIf<TIsUnboundedArray<T>::Value, TSharedPtr<T>>::Type MakeShared(TSize_T Num)
{
	return TSharedPtr<T>(new typename TRemoveExtent<T>::Type[Num]());
}

template<typename T, TBool EnableThreadSafety, typename... Args>
typename TEnableIf<TIsBoundedArray<T>::Value>::Type MakeShared(Args&&...) = delete;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// first: nullptr, second: TUnique
template<typename Type, TBool EnableThreadSafety, typename DeleterType>
TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator==(TNullPtr, const TSharedPtr<Type, EnableThreadSafety, DeleterType>& Rhs)
{
	return nullptr == Rhs.Get();
}

template<typename Type, TBool EnableThreadSafety, typename DeleterType>
TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator!=(TNullPtr, const TSharedPtr<Type, EnableThreadSafety, DeleterType>& Rhs)
{
	return nullptr != Rhs.Get();
}

// first: TUnique, second: nullptr
template<typename Type, TBool EnableThreadSafety, typename DeleterType>
TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator==(const TSharedPtr<Type, EnableThreadSafety, DeleterType>& Lhs, TNullPtr)
{
	return Lhs.Get() == nullptr;
}

template<typename Type, TBool EnableThreadSafety, typename DeleterType>
TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator!=(const TSharedPtr<Type, EnableThreadSafety, DeleterType>& Lhs, TNullPtr)
{
	return Lhs.Get() != nullptr;
}

// first: pointer, second: TUnique
template<typename Type, TBool EnableThreadSafety, typename OtherType, typename DeleterType>
TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator==(const Type* Lhs, const TSharedPtr<OtherType, EnableThreadSafety, DeleterType>& Rhs)
{
	return Lhs == Rhs.Get();
}

template<typename Type, TBool EnableThreadSafety, typename OtherType, typename DeleterType>
TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator!=(const Type* Lhs, const TSharedPtr<OtherType, EnableThreadSafety, DeleterType>& Rhs)
{
	return Lhs != Rhs.Get();
}

// first: TUnique, second: pointer
template<typename Type, TBool EnableThreadSafety, typename OtherType, typename DeleterType>
TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator==(const TSharedPtr<OtherType, EnableThreadSafety, DeleterType>& Lhs, const Type* Rhs)
{
	return Lhs.Get() == Rhs;
}

template<typename Type, TBool EnableThreadSafety, typename OtherType, typename DeleterType>
TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator!=(const TSharedPtr<OtherType, EnableThreadSafety, DeleterType>& Lhs, const Type* Rhs)
{
	return Lhs.Get() != Rhs;
}

// first: TUnique, second: TUnique
template<typename Type, TBool EnableThreadSafety, typename DeleterType, typename OtherType, typename OtherDeleterType>
TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator==(const TSharedPtr<Type, EnableThreadSafety, DeleterType>& Lhs, const TSharedPtr<OtherType, EnableThreadSafety, OtherDeleterType>& Rhs)
{
	return Lhs.Get() == Rhs.Get();
}

template<typename Type, TBool EnableThreadSafety, typename DeleterType, typename OtherType, typename OtherDeleterType>
TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator!=(const TSharedPtr<Type, EnableThreadSafety, DeleterType>& Lhs, const TSharedPtr<OtherType, EnableThreadSafety, OtherDeleterType>& Rhs)
{
	return Lhs.Get() != Rhs.Get();
}