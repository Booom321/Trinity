#pragma once

#include <atomic>

#include "Trinity\Core\TypeTraits\TypeChooser.h"
#include "UniquePtr.h"

namespace TNsSharedPtrDetails
{
	template<TBool EnableThreadSafety>
	class TReferenceCounterBase
	{
	public:
		using IntegralType = TInt32;
		using CounterType = typename TTypeChooser<EnableThreadSafety, std::atomic<IntegralType>, IntegralType>::Type;

	public:
		virtual void Destroy() noexcept = 0;
		virtual void* GetDeleter() noexcept = 0;

	public:
		TRNT_FORCE_INLINE TReferenceCounterBase() = default;

		TRNT_FORCE_INLINE TReferenceCounterBase(const TReferenceCounterBase&) = default;
		TRNT_FORCE_INLINE TReferenceCounterBase(TReferenceCounterBase&&) noexcept = default;
		TRNT_FORCE_INLINE TReferenceCounterBase& operator=(const TReferenceCounterBase&) = default;
		TRNT_FORCE_INLINE TReferenceCounterBase& operator=(TReferenceCounterBase&&) noexcept = default;

		TRNT_FORCE_INLINE ~TReferenceCounterBase()
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

		TRNT_FORCE_INLINE IntegralType ReleaseSharedReference()
		{
			if constexpr (EnableThreadSafety)
			{
				IntegralType Count = SharedRefCount.fetch_sub(1, std::memory_order_acq_rel) - 1;

				if (Count == 0)
				{
					Destroy();
				}

				return Count;
			}
			else
			{
				--SharedRefCount;

				if (SharedRefCount == 0)
				{
					Destroy();
				}

				return SharedRefCount;
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
	//////////////////////////////////////////////////////////////////////////////////////////
	template<typename T, TBool EnableThreadSafety>
	class TDefaultReferenceCounter : public TReferenceCounterBase<EnableThreadSafety>
	{
	public:
		using BaseType = TReferenceCounterBase<EnableThreadSafety>;
		using PointerType = typename TRemoveExtent<T>::Type*;

		explicit TDefaultReferenceCounter(PointerType Ptr) : BaseType(), Ptr(Ptr) {}

		void Destroy() noexcept override
		{
			if (Ptr)
			{
				if constexpr (TIsArray<T>::Value)
				{
					delete[] Ptr;
				}
				else
				{
					delete Ptr;
				}
			}
		}

		void* GetDeleter() noexcept override
		{
			return nullptr;
		}

	private:
		PointerType Ptr;
	};

	template<typename T, typename DeleterType, TBool EnableThreadSafety>
	class TReferenceCounterWithDeleter : public TReferenceCounterBase<EnableThreadSafety>
	{
	public:
		using BaseType = TReferenceCounterBase<EnableThreadSafety>;
		using PointerType = typename TRemoveExtent<T>::Type*;

		TReferenceCounterWithDeleter(PointerType Ptr, DeleterType&& Deleter) : BaseType(), Pair(Move(Deleter), Ptr) {}

		void Destroy() noexcept override
		{
			Pair.GetFirstValue()(Pair.SecondValue);
		}

		void* GetDeleter() noexcept override
		{
			return (void*)(&(Pair.GetFirstValue()));
		}

	private:
		TCompressedPair<DeleterType, PointerType> Pair;
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

template<typename T, TBool EnableThreadSafety = false>
class TSharedPtr
{
public:
	using ElementType = typename TRemoveExtent<T>::Type;
	using PointerType = ElementType*;

	using ReferenceCounterType = TNsSharedPtrDetails::TReferenceCounterBase<EnableThreadSafety>;

	template<typename OtherType, TBool OtherEnableThreadSafety>
	friend class TSharedPtr;

public:
	TRNT_FORCE_INLINE TSharedPtr()
		: RefCounter(nullptr), Ptr(nullptr)
	{}

	explicit TRNT_FORCE_INLINE TSharedPtr(TNullPtr)
		: RefCounter(nullptr), Ptr(nullptr)
	{}

	template<
		typename OtherType,
		typename TEnableIf<!TIsArray<OtherType>::Value&& TNsSharedPtrDetails::TSharedPtrConvertible<OtherType, T>::Value, int>::Type = 0
	>
	explicit TRNT_FORCE_INLINE TSharedPtr(OtherType* Ptr)
		: RefCounter(new TNsSharedPtrDetails::TDefaultReferenceCounter<OtherType, EnableThreadSafety>(Ptr)), Ptr(Ptr)
	{
		RefCounter->AddSharedReference();
	}

	template<
		typename OtherType,
		typename OtherDeleterType,
		typename TEnableIf<!TIsArray<OtherType>::Value&& TNsSharedPtrDetails::TSharedPtrConvertible<OtherType, T>::Value, int>::Type = 0
	>
	explicit TRNT_FORCE_INLINE TSharedPtr(OtherType* Ptr, OtherDeleterType&& OtherDeleter) noexcept
		: RefCounter(new TNsSharedPtrDetails::TReferenceCounterWithDeleter<OtherType, OtherDeleterType, EnableThreadSafety>(Ptr, Move(OtherDeleter))), Ptr(Ptr)
	{
		RefCounter->AddSharedReference();
	}

	template<
		typename OtherType,
		typename OtherDeleterType,
		typename TEnableIf<TNsSharedPtrDetails::TSharedPtrConvertible<typename TRemoveExtent<OtherType>::Type, T>::Value, int>::Type = 0
	>
	TRNT_FORCE_INLINE TSharedPtr(const TSharedPtr<OtherType, EnableThreadSafety>& Other)
		: RefCounter(Other.RefCounter), Ptr(Other.Ptr)
	{
		RefCounter->AddSharedReference();
	}

	template<
		typename OtherType,
		typename OtherDeleterType,
		typename TEnableIf<TNsSharedPtrDetails::TSharedPtrConvertible<typename TRemoveExtent<OtherType>::Type, T>::Value, int>::Type = 0
	>
	TRNT_FORCE_INLINE TSharedPtr(TSharedPtr<OtherType, EnableThreadSafety>&& Other) noexcept
		: RefCounter(Other.RefCounter), Ptr(Other.Ptr)
	{
		Other.RefCounter = nullptr;
		Other.Ptr = nullptr;
	}

	TRNT_FORCE_INLINE TSharedPtr(const TSharedPtr& Other)
		: RefCounter(Other.RefCounter), Ptr(Other.Ptr)
	{
		RefCounter->AddSharedReference();
	}

	TRNT_FORCE_INLINE TSharedPtr(TSharedPtr&& Other) noexcept
		: RefCounter(Other.RefCounter), Ptr(Other.Ptr)
	{
		Other.RefCounter = nullptr;
		Other.Ptr = nullptr;
	}

	~TSharedPtr()
	{
		ReleaseSharedReferenceInternal();
	}

public:
	TRNT_NODISCARD TRNT_FORCE_INLINE PointerType Get() const
	{
		return Ptr;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE void* GetRawDeleter()
	{
		return RefCounter->GetDeleter();
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE const void* GetRawDeleter() const
	{
		return RefCounter->GetDeleter();
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
		return Ptr != nullptr;
	}

	template<typename OtherT = T, typename TEnableIf<!TAreTheSameType<OtherT, void>::Value && !TIsArray<OtherT>::Value, int>::Type = 0>
	TRNT_NODISCARD TRNT_FORCE_INLINE PointerType operator->() const
	{
		return Ptr;
	}

	template<typename OtherT = T, typename TEnableIf<!TAreTheSameType<OtherT, void>::Value && !TIsArray<OtherT>::Value, int>::Type = 0>
	TRNT_NODISCARD TRNT_FORCE_INLINE decltype(auto) operator*() const noexcept
	{
		return *Ptr;
	}

	template<typename OtherT = T, typename TEnableIf<!TAreTheSameType<OtherT, void>::Value&& TIsArray<OtherT>::Value, int>::Type = 0>
	TRNT_NODISCARD TRNT_FORCE_INLINE decltype(auto) operator[](TSize_T Index) const
	{
		return Ptr[Index];
	}

public:
	TSharedPtr& operator=(TNullPtr)
	{
		ReleaseSharedReferenceInternal();

		RefCounter = nullptr;
		Ptr = nullptr;

		return *this;
	}

	template<
		typename OtherType,
		typename TEnableIf<TNsSharedPtrDetails::TSharedPtrConvertible<typename TRemoveExtent<OtherType>::Type, T>::Value, int>::Type = 0
	>
	TSharedPtr& operator=(const TSharedPtr<OtherType, EnableThreadSafety>& Other)
	{
		ReleaseSharedReferenceInternal();

		RefCounter = Other.RefCounter;
		Ptr = Other.Ptr;

		RefCounter->AddSharedReference();

		return *this;
	}

	template<
		typename OtherType,
		typename TEnableIf<TNsSharedPtrDetails::TSharedPtrConvertible<typename TRemoveExtent<OtherType>::Type, T>::Value, int>::Type = 0
	>
	TSharedPtr& operator=(TSharedPtr<OtherType, EnableThreadSafety>&& Other) noexcept
	{
		ReleaseSharedReferenceInternal();

		RefCounter = Other.RefCounter;
		Ptr = Other.Ptr;

		Other.RefCounter = nullptr;
		Other.Ptr = nullptr;

		return *this;
	}

	TSharedPtr& operator=(const TSharedPtr& Other)
	{
		if (this != &Other)
		{
			ReleaseSharedReferenceInternal();

			RefCounter = Other.RefCounter;
			Ptr = Other.Ptr;

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
			Ptr = Other.Ptr;

			Other.RefCounter = nullptr;
			Other.Ptr = nullptr;
		}
		return *this;
	}


public:
	TRNT_FORCE_INLINE void Reset()
	{
		ReleaseSharedReferenceInternal();

		RefCounter = nullptr;
		Ptr = nullptr;
	}


private:
	TRNT_FORCE_INLINE void ReleaseSharedReferenceInternal()
	{
		if (RefCounter != nullptr && Ptr != nullptr)
		{
			if (RefCounter->ReleaseSharedReference() == 0)
			{
				delete RefCounter;
				RefCounter = nullptr;
			}
		}
	}

private:
	ReferenceCounterType* RefCounter;
	PointerType Ptr;
};

namespace TNsHash
{
	template<typename T, TBool EnableThreadSafety>
	inline TSize_T GetHashCode(const TSharedPtr<T, EnableThreadSafety>& SharedPtr)
	{
		return GetHashCode<typename TSharedPtr<T, EnableThreadSafety>::PointerType>(SharedPtr.Get());
	}
}

template<typename T, TBool EnableThreadSafety, typename... ArgsType>
TRNT_NODISCARD TRNT_FORCE_INLINE typename TEnableIf<!TIsArray<T>::Value, TSharedPtr<T, EnableThreadSafety>>::Type MakeSharedTS(ArgsType&&... Args)
{
	return TSharedPtr<T, EnableThreadSafety>(new T(Forward<ArgsType>(Args)...));
}

template<typename T, TBool EnableThreadSafety>
TRNT_NODISCARD TRNT_FORCE_INLINE typename TEnableIf<TIsUnboundedArray<T>::Value, TSharedPtr<T, EnableThreadSafety>>::Type MakeSharedTS(TSize_T Num)
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

template<typename T, TBool EnableThreadSafety, typename... ArgsType>
typename TEnableIf<TIsBoundedArray<T>::Value>::Type MakeSharedTS(ArgsType&&...) = delete;

template<typename T, typename... ArgsType>
typename TEnableIf<TIsBoundedArray<T>::Value>::Type MakeShared(ArgsType&&...) = delete;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Type, TBool EnableThreadSafety>
TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator==(TNullPtr, const TSharedPtr<Type, EnableThreadSafety>& Rhs)
{
	return nullptr == Rhs.Get();
}

template<typename Type, TBool EnableThreadSafety>
TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator!=(TNullPtr, const TSharedPtr<Type, EnableThreadSafety>& Rhs)
{
	return nullptr != Rhs.Get();
}

template<typename Type, TBool EnableThreadSafety>
TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator==(const TSharedPtr<Type, EnableThreadSafety>& Lhs, TNullPtr)
{
	return Lhs.Get() == nullptr;
}

template<typename Type, TBool EnableThreadSafety>
TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator!=(const TSharedPtr<Type, EnableThreadSafety>& Lhs, TNullPtr)
{
	return Lhs.Get() != nullptr;
}

template<typename Type, TBool EnableThreadSafety, typename OtherType>
TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator==(const Type* Lhs, const TSharedPtr<OtherType, EnableThreadSafety>& Rhs)
{
	return Lhs == Rhs.Get();
}

template<typename Type, TBool EnableThreadSafety, typename OtherType>
TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator!=(const Type* Lhs, const TSharedPtr<OtherType, EnableThreadSafety>& Rhs)
{
	return Lhs != Rhs.Get();
}

template<typename Type, TBool EnableThreadSafety, typename OtherType>
TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator==(const TSharedPtr<Type, EnableThreadSafety>& Lhs, const OtherType* Rhs)
{
	return Lhs.Get() == Rhs;
}

template<typename Type, TBool EnableThreadSafety, typename OtherType>
TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator!=(const TSharedPtr<Type, EnableThreadSafety>& Lhs, const OtherType* Rhs)
{
	return Lhs.Get() != Rhs;
}

template<typename Type, TBool EnableThreadSafety, typename OtherType, TBool OtherEnableThreadSafety>
TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator==(const TSharedPtr<Type, EnableThreadSafety>& Lhs, const TSharedPtr<OtherType, OtherEnableThreadSafety>& Rhs)
{
	return Lhs.Get() == Rhs.Get();
}

template<typename Type, TBool EnableThreadSafety, typename OtherType, TBool OtherEnableThreadSafety>
TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator!=(const TSharedPtr<Type, EnableThreadSafety>& Lhs, const TSharedPtr<OtherType, OtherEnableThreadSafety>& Rhs)
{
	return Lhs.Get() != Rhs.Get();
}