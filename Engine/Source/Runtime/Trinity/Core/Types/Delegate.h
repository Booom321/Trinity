#pragma once

#include <utility>

#include "Trinity/Core/Types/DataTypes.h"
#include "Trinity/Core/Types/SharedPtr.h"

#include "Trinity/Core/Types/Pair.h"

#include "Trinity/Core/TypeTraits/EnableIf.h"
#include "Trinity/Core/TypeTraits/PrimaryTypes.h"
#include "Trinity/Core/TypeTraits/TypeRelationships.h"
#include "Trinity/Core/TypeTraits/Decay.h"

template<typename Type> class TDelegate;

template<typename ReturnType, typename... Arguments>
class TDelegate<ReturnType(Arguments...)>
{
    using StubPointerType = ReturnType(*)(void*, Arguments&&...);

    TRNT_FORCE_INLINE TDelegate(void* const ObjectPointer, StubPointerType const StubPointer) noexcept
        : ObjectPointer(ObjectPointer), StubPointer(StubPointer)
    {
    }

public:
    TRNT_FORCE_INLINE TDelegate() = default;

    TRNT_FORCE_INLINE TDelegate(TDelegate const&) = default;

    TRNT_FORCE_INLINE TDelegate(TDelegate&&) = default;

    TRNT_FORCE_INLINE TDelegate(std::nullptr_t const) noexcept : TDelegate()
    {
    }

    template<typename Class, typename = typename TEnableIf<TIsClass<Class>::Value>::Type>
    TRNT_FORCE_INLINE explicit TDelegate(Class const* const Object) noexcept
        : ObjectPointer(const_cast<Class*>(Object))
    {
    }

    template<typename Class, typename = typename TEnableIf<TIsClass<Class>::Value>::Type>
    TRNT_FORCE_INLINE explicit TDelegate(Class const& Object) noexcept
        : ObjectPointer(const_cast<Class*>(&Object))
    {
    }

    template<typename Class>
    TRNT_FORCE_INLINE TDelegate(Class* const ObjectPointer, ReturnType(Class::* const MethodPointer)(Arguments...))
    {
        *this = From(ObjectPointer, MethodPointer);
    }

    template<typename Class>
    TRNT_FORCE_INLINE TDelegate(Class* const ObjectPointer, ReturnType(Class::* const MethodPointer)(Arguments...) const)
    {
        *this = From(ObjectPointer, MethodPointer);
    }

    template<typename Class>
    TRNT_FORCE_INLINE TDelegate(Class& Object, ReturnType(Class::* const MethodPointer)(Arguments...))
    {
        *this = From(Object, MethodPointer);
    }

    template<typename Class>
    TRNT_FORCE_INLINE  TDelegate(Class const& Object, ReturnType(Class::* const MethodPointer)(Arguments...) const)
    {
        *this = From(Object, MethodPointer);
    }

    template<typename T, typename = typename TEnableIf<!TAreTheSameType<TDelegate, typename TDecay<T>::Type>::Value>::Type>
    TDelegate(T&& Func)
        : Store(operator new(sizeof(typename TDecay<T>::Type)), FunctorDeleter<typename TDecay<T>::Type>), StoreSize(sizeof(typename TDecay<T>::Type))
    {
        using FunctorType = typename TDecay<T>::Type;

        new (Store.get()) FunctorType(Forward<T>(Func));

        ObjectPointer = Store.get();

        StubPointer = FunctorStub<FunctorType>;

        Deleter = DeleterStub<FunctorType>;
    }

    TDelegate& operator=(TDelegate const&) = default;

    TDelegate& operator=(TDelegate&&) = default;

    template<typename Class>
    TRNT_FORCE_INLINE TDelegate& operator=(ReturnType(Class::* const Rhs)(Arguments...))
    {
        return *this = From(static_cast<Class*>(ObjectPointer), Rhs);
    }

    template<typename Class>
    TRNT_FORCE_INLINE TDelegate& operator=(ReturnType(Class::* const Rhs)(Arguments...) const)
    {
        return *this = From(static_cast<Class const*>(ObjectPointer), Rhs);
    }

    template<typename T, typename = typename TEnableIf<!TAreTheSameType<TDelegate, typename TDecay<T>::Type>::Value>::Type>
    TDelegate& operator=(T&& Func)
    {
        using FunctorType = typename TDecay<T>::Type;

        void* StorePointer = Store.get();

        if ((sizeof(FunctorType) > StoreSize) || !Store.unique())
        {
            Store.reset(operator new(sizeof(FunctorType)), FunctorDeleter<FunctorType>);

            StoreSize = sizeof(FunctorType);
        }
        else
        {
            Deleter(StorePointer);
        }

        new (StorePointer) FunctorType(Forward<T>(Func));

        ObjectPointer = StorePointer;

        StubPointer = FunctionStub<FunctorType>;

        Deleter = DeleterStub<FunctorType>;

        return *this;
    }

public:
    template<ReturnType(* const FunctionPointer)(Arguments...)>
    static TRNT_FORCE_INLINE TDelegate From() noexcept
    {
        return{ nullptr, FunctionStub<FunctionPointer> };
    }

    template<typename Class, ReturnType(Class::* const MethodPointer)(Arguments...)>
    static TRNT_FORCE_INLINE TDelegate From(Class* const ObjectPointer) noexcept
    {
        return{ ObjectPointer, MethodStub<Class, MethodPointer> };
    }

    template<typename Class, ReturnType(Class::* const MethodPointer)(Arguments...) const>
    static TRNT_FORCE_INLINE TDelegate From(Class const* const ObjectPointer) noexcept
    {
        return{ const_cast<Class*>(ObjectPointer), ConstMethodStub<Class, MethodPointer> };
    }

    template<typename Class, ReturnType(Class::* const MethodPointer)(Arguments...)>
    static TRNT_FORCE_INLINE TDelegate From(Class& Object) noexcept
    {
        return{ &Object, MethodStub<Class, MethodPointer> };
    }

    template<typename Class, ReturnType(Class::* const MethodPointer)(Arguments...) const>
    static TRNT_FORCE_INLINE TDelegate From(Class const& Object) noexcept
    {
        return{ const_cast<Class*>(&Object), ConstMethodStub<Class, MethodPointer> };
    }

    template<typename T>
    static TRNT_FORCE_INLINE TDelegate From(T&& Func)
    {
        return Forward<T>(Func);
    }

    static TRNT_FORCE_INLINE TDelegate From(ReturnType(* const FunctionPointer)(Arguments...))
    {
        return FunctionPointer;
    }

    template<typename Class>
    using MemberPair = TPair<Class* const, ReturnType(Class::* const)(Arguments...)>;

    template<typename Class>
    using ConstMemberPair = TPair<Class const* const, ReturnType(Class::* const)(Arguments...) const>;

    template<typename Class>
    static TRNT_FORCE_INLINE TDelegate From(Class* const ObjectPointer, ReturnType(Class::* const MethodPointer)(Arguments...))
    {
        return MemberPair<Class>(ObjectPointer, MethodPointer);
    }

    template<typename Class>
    static TRNT_FORCE_INLINE TDelegate From(Class const* const ObjectPointer, ReturnType(Class::* const MethodPointer)(Arguments...) const)
    {
        return ConstMemberPair<Class>(ObjectPointer, MethodPointer);
    }

    template<typename Class>
    static TRNT_FORCE_INLINE TDelegate From(Class& Object, ReturnType(Class::* const MethodPointer)(Arguments...))
    {
        return MemberPair<Class>(&Object, MethodPointer);
    }

    template<typename Class>
    static TRNT_FORCE_INLINE TDelegate From(Class const& Object, ReturnType(Class::* const MethodPointer)(Arguments...) const)
    {
        return ConstMemberPair<Class>(&Object, MethodPointer);
    }

public:
    TRNT_FORCE_INLINE void Reset() { StubPointer = nullptr; Store.reset(); }

    TRNT_FORCE_INLINE void ResetStub() noexcept { StubPointer = nullptr; }

    TRNT_FORCE_INLINE void Swap(TDelegate& Other) noexcept { std::swap(*this, Other); }

    bool operator==(TDelegate const& Rhs) const noexcept
    {
        if (StoreSize && Rhs.StoreSize && StoreSize == Rhs.StoreSize)
            return (std::memcmp(Store.get(), Rhs.Store.get(), StoreSize) == 0) && (StubPointer == Rhs.StubPointer);
        return (ObjectPointer == Rhs.ObjectPointer) && (StubPointer == Rhs.StubPointer);
    }

    TRNT_FORCE_INLINE bool operator!=(TDelegate const& Rhs) const noexcept
    {
        return !operator==(Rhs);
    }

    TRNT_FORCE_INLINE bool operator<(TDelegate const& Rhs) const noexcept
    {
        return (ObjectPointer < Rhs.ObjectPointer) || ((ObjectPointer == Rhs.ObjectPointer) && (StubPointer < Rhs.StubPointer));
    }

    TRNT_FORCE_INLINE bool operator==(std::nullptr_t const) const noexcept
    {
        return !StubPointer;
    }

    TRNT_FORCE_INLINE bool operator!=(std::nullptr_t const) const noexcept
    {
        return StubPointer;
    }

    TRNT_FORCE_INLINE explicit operator bool() const noexcept { return StubPointer; }

    ReturnType operator()(Arguments... Args) const
    {
        return StubPointer(ObjectPointer, Forward<Arguments>(Args)...);
    }

private:
    friend struct std::hash<TDelegate>;

    using DeleterType = void(*)(void*);

    void* ObjectPointer;
    StubPointerType StubPointer{};

    DeleterType Deleter;

    TSharedPtr<void> Store;
    TSize_T StoreSize = 0;

    template<typename T>
    static TRNT_FORCE_INLINE void FunctorDeleter(void* const Pointer)
    {
        static_cast<T*>(Pointer)->~T();

        operator delete(Pointer);
    }

    template<typename T>
    static TRNT_FORCE_INLINE void DeleterStub(void* const Pointer)
    {
        static_cast<T*>(Pointer)->~T();
    }

    template<ReturnType(*FunctionPointer)(Arguments...)>
    static ReturnType FunctionStub(void* const, Arguments&&... Args)
    {
        return FunctionPointer(Forward<Arguments>(Args)...);
    }

    template<typename Class, ReturnType(Class::* MethodPointer)(Arguments...)>
    static ReturnType MethodStub(void* const ObjectPointer, Arguments&&... Args)
    {
        return (static_cast<Class*>(ObjectPointer)->*MethodPointer)(Forward<Arguments>(Args)...);
    }

    template<typename Class, ReturnType(Class::* MethodPointer)(Arguments...) const>
    static ReturnType ConstMethodStub(void* const ObjectPointer, Arguments&&... Args)
    {
        return (static_cast<Class const*>(ObjectPointer)->*MethodPointer)(Forward<Arguments>(Args)...);
    }

    template<typename>
    class TIsMemberPair : public TFalseType
    {
    };

    template<typename Class>
    class TIsMemberPair<TPair<Class* const, ReturnType(Class::* const)(Arguments...)>> : public TTrueType
    {
    };

    template<typename>
    class TIsConstMemberPair : public TFalseType
    {
    };

    template<typename Class>
    class TIsConstMemberPair<TPair<Class const* const, ReturnType(Class::* const)(Arguments...) const>> : public TTrueType
    {
    };

    template<typename T>
    static typename TEnableIf<!(TIsMemberPair<T>::Value || TIsConstMemberPair<T>::Value), ReturnType>::Type FunctorStub(void* const ObjectPointer, Arguments&&... Args)
    {
        return (*static_cast<T*>(ObjectPointer))(Forward<Arguments>(Args)...);
    }

    template<typename T>
    static typename TEnableIf<TIsMemberPair<T>::Value || TIsConstMemberPair<T>::Value, ReturnType>::Type FunctorStub(void* const ObjectPointer, Arguments&&... Args)
    {
        return (static_cast<T*>(ObjectPointer)->First->*static_cast<T*>(ObjectPointer)->Second)(Forward<Arguments>(Args)...);
    }
};