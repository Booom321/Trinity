#pragma once

#include "Trinity/Core/TypeTraits/EnableIf.h"
#include "Trinity/Core/TypeTraits/IsBitwiseConstructible.h"
#include "Trinity/Core/TypeTraits/RemoveExtent.h"
#include "Trinity/Core/Types/CompressedPair.h"

template<typename T>
class TDefaultDeleter
{
public:
	TDefaultDeleter() noexcept = default;
	TDefaultDeleter(const TDefaultDeleter&) = default;
	TDefaultDeleter(TDefaultDeleter&&) noexcept = default;
	TDefaultDeleter& operator=(const TDefaultDeleter&) = default;
	TDefaultDeleter& operator=(TDefaultDeleter&&) noexcept = default;
	~TDefaultDeleter() = default;

	template<typename U>
	TRNT_FORCE_INLINE TDefaultDeleter(const TDefaultDeleter<U>&)
	{}

	template<typename U>
	TRNT_FORCE_INLINE TDefaultDeleter& operator=(const TDefaultDeleter<U>&)
	{
		return *this;
	}

public:
	TRNT_FORCE_INLINE void operator()(typename TRemoveExtent<T>::Type* Ptr)
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
};

template<typename Type, typename Deleter = TDefaultDeleter<Type>>
class TUniquePtr
{
public:
	static_assert(!TAreTheSameType<Type, void>::Value, "Void type is not allowed!");

	using ElementType = Type;
	using PointerType = ElementType*;

	using DeleterType = Deleter;

	template<typename OtherType, typename OtherDeleterType>
	friend class TUniquePtr;

public:
	TRNT_FORCE_INLINE TUniquePtr() noexcept
		: Pair(DeleterType(), nullptr)
	{}

	explicit TRNT_FORCE_INLINE TUniquePtr(TNullPtr) noexcept
		: Pair(DeleterType(), nullptr)
	{}

	template<
		typename OtherType,
		typename TEnableIf<TIsConvertible<typename TUniquePtr<OtherType, DeleterType>::PointerType, PointerType>::Value, int>::Type = 0>
	explicit TRNT_FORCE_INLINE TUniquePtr(OtherType* Ptr)
		: Pair(DeleterType(), Ptr)
	{}

	template<
		typename OtherType,
		typename OtherDeleterType,
		typename TEnableIf<TIsConvertible<typename TUniquePtr<OtherType, OtherDeleterType>::PointerType, PointerType>::Value, int>::Type = 0>
	explicit TRNT_FORCE_INLINE TUniquePtr(OtherType* Ptr, const OtherDeleterType& OtherDeleter)
		: Pair(OtherDeleter, Ptr)
	{}

	template<
		typename OtherType,
		typename OtherDeleterType,
		typename TEnableIf<TIsConvertible<typename TUniquePtr<OtherType, OtherDeleterType>::PointerType, PointerType>::Value, int>::Type = 0>
	explicit TRNT_FORCE_INLINE TUniquePtr(OtherType* Ptr, OtherDeleterType&& OtherDeleter) noexcept
		: Pair(Move(OtherDeleter), Ptr)
	{}

	template<
		typename OtherType,
		typename OtherDeleterType,
		typename TEnableIf<TIsConvertible<typename TUniquePtr<OtherType, OtherDeleterType>::PointerType, PointerType>::Value, int>::Type = 0>
	TRNT_FORCE_INLINE TUniquePtr(TUniquePtr<OtherType, OtherDeleterType>&& Other) noexcept
		: Pair(Move(Other.GetDeleter()), Other.Pair.SecondValue)
	{
		Other.Pair.SecondValue = nullptr;
	}

	TUniquePtr(const TUniquePtr&) = delete;
	TUniquePtr& operator=(const TUniquePtr&) = delete;

	~TUniquePtr()
	{
		Pair.GetFirstValue()(Pair.SecondValue);
	}

public:
	TRNT_FORCE_INLINE TUniquePtr& operator=(TUniquePtr&& Other) noexcept
	{
		if (this != &Other)
		{
			DeleterType& SelfDeleter = Pair.GetFirstValue();
			SelfDeleter(Pair.SecondValue);
			Pair.SecondValue = Other.Pair.SecondValue;
			Other.Pair.SecondValue = nullptr;
			SelfDeleter = Move(Other.GetDeleter());
		}

		return *this;
	}

	template<
		typename OtherType,
		typename OtherDeleterType,
		typename TEnableIf<TIsConvertible<typename TUniquePtr<OtherType, OtherDeleterType>::PointerType, PointerType>::Value, int>::Type = 0>
	TRNT_FORCE_INLINE TUniquePtr& operator=(TUniquePtr<OtherType, OtherDeleterType>&& Other) noexcept
	{
		DeleterType& SelfDeleter = Pair.GetFirstValue();
		SelfDeleter(Pair.SecondValue);
		Pair.SecondValue = Other.Pair.SecondValue;
		Other.Pair.SecondValue = nullptr;
		SelfDeleter = Move(Other.GetDeleter());

		return *this;
	}

	TRNT_FORCE_INLINE TUniquePtr& operator=(TNullPtr)
	{
		Pair.GetFirstValue()(Pair.SecondValue);
		Pair.SecondValue = nullptr;
		return *this;
	}

public:
	TRNT_NODISCARD TRNT_FORCE_INLINE DeleterType& GetDeleter() noexcept
	{
		return Pair.GetFirstValue();
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE const DeleterType& GetDeleter() const noexcept
	{
		return Pair.GetFirstValue();
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE PointerType Get() const
	{
		return Pair.SecondValue;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE PointerType Release()
	{
		PointerType Tmp = Pair.SecondValue;
		Pair.SecondValue = nullptr;
		return Tmp;
	}

	TRNT_FORCE_INLINE void Reset(PointerType NewPtr = nullptr) noexcept
	{
		if (Pair.SecondValue != NewPtr)
		{
			Pair.GetFirstValue()(Pair.SecondValue);
			Pair.SecondValue = NewPtr;
		}
	}

	template<
		typename OtherType,
		typename TEnableIf<TIsConvertible<typename TUniquePtr<OtherType, DeleterType>::PointerType, PointerType>::Value, int>::Type = 0>
	TRNT_FORCE_INLINE void Reset(OtherType* NewPtr = nullptr) noexcept
	{
		Pair.GetFirstValue()(NewPtr);
		Pair.SecondValue = NewPtr;
	}

public:
	TRNT_NODISCARD TRNT_FORCE_INLINE explicit operator bool() const
	{
		return Pair.SecondValue != nullptr;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE PointerType operator->() const
	{
		return Pair.SecondValue;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE ElementType& operator*() const
	{
		return *Pair.SecondValue;
	}

private:
	TCompressedPair<DeleterType, PointerType> Pair;
};

template<typename Type, typename Deleter>
class TUniquePtr<Type[], Deleter>
{
public:
	static_assert(!TAreTheSameType<Type, void>::Value, "Void type is not allowed!");

	using ElementType = Type;
	using DeleterType = Deleter;

	using PointerType = ElementType*;

	template<typename OtherType, typename OtherDeleterType>
	friend class TUniquePtr;

public:
	TRNT_FORCE_INLINE TUniquePtr() noexcept
		: Pair(DeleterType(), nullptr)
	{}

	explicit TRNT_FORCE_INLINE TUniquePtr(TNullPtr) noexcept
		: Pair(DeleterType(), nullptr)
	{}

	template<
		typename OtherType,
		typename TEnableIf<TIsConvertible<typename TUniquePtr<OtherType, DeleterType>::ElementType (*)[], ElementType (*)[]>::Value, int>::Type = 0>
	explicit TRNT_FORCE_INLINE TUniquePtr(OtherType* Ptr)
		: Pair(DeleterType(), Ptr)
	{}

	template<
		typename OtherType,
		typename OtherDeleterType,
		typename TEnableIf<TIsConvertible<typename TUniquePtr<OtherType, OtherDeleterType>::ElementType (*)[], ElementType (*)[]>::Value, int>::Type = 0>
	explicit TRNT_FORCE_INLINE TUniquePtr(OtherType* Ptr, const OtherDeleterType& OtherDeleter)
		: Pair(OtherDeleter, Ptr)
	{}

	template<
		typename OtherType,
		typename OtherDeleterType,
		typename TEnableIf<TIsConvertible<typename TUniquePtr<OtherType, OtherDeleterType>::ElementType (*)[], ElementType (*)[]>::Value, int>::Type = 0>
	explicit TRNT_FORCE_INLINE TUniquePtr(OtherType* Ptr, OtherDeleterType&& OtherDeleter) noexcept
		: Pair(Move(OtherDeleter), Ptr)
	{}

	template<
		typename OtherType,
		typename OtherDeleterType,
		typename TEnableIf<TIsConvertible<typename TUniquePtr<OtherType, OtherDeleterType>::ElementType (*)[], ElementType (*)[]>::Value, int>::Type = 0>
	TRNT_FORCE_INLINE TUniquePtr(TUniquePtr<OtherType, OtherDeleterType>&& Other) noexcept
		: Pair(Move(Other.GetDeleter()), Other.Pair.SecondValue)
	{
		Other.Pair.SecondValue = nullptr;
	}

	TUniquePtr(const TUniquePtr&) = delete;
	TUniquePtr& operator=(const TUniquePtr&) = delete;

	~TUniquePtr()
	{
		Pair.GetFirstValue()(Pair.SecondValue);
	}

public:
	TRNT_FORCE_INLINE TUniquePtr& operator=(TUniquePtr&& Other) noexcept
	{
		if (this != &Other)
		{
			DeleterType& SelfDeleter = Pair.GetFirstValue();
			SelfDeleter(Pair.SecondValue);
			Pair.SecondValue = Other.Pair.SecondValue;
			Other.Pair.SecondValue = nullptr;
			SelfDeleter = Move(Other.GetDeleter());
		}

		return *this;
	}

	template<
		typename OtherType,
		typename OtherDeleterType,
		typename TEnableIf<TIsConvertible<typename TUniquePtr<OtherType, OtherDeleterType>::ElementType (*)[], ElementType (*)[]>::Value, int>::Type = 0>
	TRNT_FORCE_INLINE TUniquePtr& operator=(TUniquePtr<OtherType, OtherDeleterType>&& Other) noexcept
	{
		DeleterType& SelfDeleter = Pair.GetFirstValue();
		SelfDeleter(Pair.SecondValue);
		Pair.SecondValue = Other.Pair.SecondValue;
		Other.Pair.SecondValue = nullptr;
		SelfDeleter = Move(Other.GetDeleter());

		return *this;
	}

	TRNT_FORCE_INLINE TUniquePtr& operator=(TNullPtr)
	{
		Pair.GetFirstValue()(Pair.SecondValue);
		Pair.SecondValue = nullptr;
		return *this;
	}

public:
	TRNT_NODISCARD TRNT_FORCE_INLINE DeleterType& GetDeleter() noexcept
	{
		return Pair.GetFirstValue();
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE const DeleterType& GetDeleter() const noexcept
	{
		return Pair.GetFirstValue();
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE PointerType Get() const
	{
		return Pair.SecondValue;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE PointerType Release()
	{
		PointerType Tmp = Pair.SecondValue;
		Pair.SecondValue = nullptr;
		return Tmp;
	}

	TRNT_FORCE_INLINE void Reset(PointerType NewPtr = nullptr) noexcept
	{
		if (Pair.SecondValue != NewPtr)
		{
			Pair.GetFirstValue()(Pair.SecondValue);
			Pair.SecondValue = NewPtr;
		}
	}

	template<
		typename OtherType,
		typename TEnableIf<TIsConvertible<typename TUniquePtr<OtherType, DeleterType>::ElementType (*)[], ElementType (*)[]>::Value, int>::Type = 0>
	TRNT_FORCE_INLINE void Reset(OtherType* NewPtr = nullptr) noexcept
	{
		Pair.GetFirstValue()(Pair.SecondValue);
		Pair.SecondValue = NewPtr;
	}

public:
	TRNT_NODISCARD TRNT_FORCE_INLINE explicit operator bool() const
	{
		return Pair.SecondValue != nullptr;
	}

	TRNT_NODISCARD TRNT_FORCE_INLINE ElementType& operator[](TSize_T Index) const noexcept
	{
		return Pair.SecondValue[Index];
	}

private:
	TCompressedPair<DeleterType, PointerType> Pair;
};

template<typename T, typename... ArgsType>
TRNT_NODISCARD TRNT_FORCE_INLINE typename TEnableIf<!TIsArray<T>::Value, TUniquePtr<T>>::Type MakeUnique(ArgsType&&... Args)
{
	return TUniquePtr<T>(new T(Forward<ArgsType>(Args)...));
}

template<typename T>
TRNT_NODISCARD TRNT_FORCE_INLINE typename TEnableIf<TIsUnboundedArray<T>::Value, TUniquePtr<T>>::Type MakeUnique(TSize_T Num)
{
	return TUniquePtr<T>(new typename TRemoveExtent<T>::Type[Num]());
}

template<typename T, typename... Args>
typename TEnableIf<TIsBoundedArray<T>::Value>::Type MakeUnique(Args&&...) = delete;

namespace TNsHash
{
	template<typename T, typename Deleter>
	inline TSize_T GetHashCode(const TUniquePtr<T, Deleter>& UniquePtr)
	{
		return GetHashCode<typename TUniquePtr<T, Deleter>::PointerType>(UniquePtr.Get());
	}
} // namespace TNsHash

// first: nullptr, second: TUnique
template<typename Type, typename DeleterType>
TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator==(TNullPtr, const TUniquePtr<Type, DeleterType>& Rhs)
{
	return nullptr == Rhs.Get();
}

template<typename Type, typename DeleterType>
TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator!=(TNullPtr, const TUniquePtr<Type, DeleterType>& Rhs)
{
	return nullptr != Rhs.Get();
}

// first: TUnique, second: nullptr
template<typename Type, typename DeleterType>
TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator==(const TUniquePtr<Type, DeleterType>& Lhs, TNullPtr)
{
	return Lhs.Get() == nullptr;
}

template<typename Type, typename DeleterType>
TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator!=(const TUniquePtr<Type, DeleterType>& Lhs, TNullPtr)
{
	return Lhs.Get() != nullptr;
}

// first: pointer, second: TUnique
template<typename Type, typename OtherType, typename DeleterType>
TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator==(const Type* Lhs, const TUniquePtr<OtherType, DeleterType>& Rhs)
{
	return Lhs == Rhs.Get();
}

template<typename Type, typename OtherType, typename DeleterType>
TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator!=(const Type* Lhs, const TUniquePtr<OtherType, DeleterType>& Rhs)
{
	return Lhs != Rhs.Get();
}

// first: TUnique, second: pointer
template<typename Type, typename OtherType, typename DeleterType>
TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator==(const TUniquePtr<OtherType, DeleterType>& Lhs, const Type* Rhs)
{
	return Lhs.Get() == Rhs;
}

template<typename Type, typename OtherType, typename DeleterType>
TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator!=(const TUniquePtr<OtherType, DeleterType>& Lhs, const Type* Rhs)
{
	return Lhs.Get() != Rhs;
}

// first: TUnique, second: TUnique
template<typename Type, typename DeleterType, typename OtherType, typename OtherDeleterType>
TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator==(const TUniquePtr<Type, DeleterType>& Lhs, const TUniquePtr<OtherType, OtherDeleterType>& Rhs)
{
	return Lhs.Get() == Rhs.Get();
}

template<typename Type, typename DeleterType, typename OtherType, typename OtherDeleterType>
TRNT_NODISCARD TRNT_FORCE_INLINE TBool operator!=(const TUniquePtr<Type, DeleterType>& Lhs, const TUniquePtr<OtherType, OtherDeleterType>& Rhs)
{
	return Lhs.Get() != Rhs.Get();
}