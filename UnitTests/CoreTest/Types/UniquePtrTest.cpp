#include "UniquePtrTest.h"

#include <Trinity/Core/Memory/UniquePtr.h>

class Foo
{
public:
	TRNT_FORCE_INLINE Foo() = default;

	TRNT_FORCE_INLINE Foo(TInt32 I32, TFloat F32)
		: I32(I32), F32(F32)
	{}

	Foo(const Foo&) = default;
	Foo(Foo&&) noexcept = default;
	~Foo() = default;

	Foo& operator=(const Foo&) = default;
	Foo& operator=(Foo&&) noexcept = default;

	TInt32 I32 = 0;
	TFloat F32 = 0.0f;
};

TRNT_IMPL_TEST_CASE(Types, TUniquePtr_Object)
{
	using UniquePtr = TUniquePtr<Foo>;
	static_assert(TAreTheSameType<typename UniquePtr::ElementType, Foo>::Value);
	static_assert(TAreTheSameType<typename UniquePtr::PointerType, Foo*>::Value);
	static_assert(TAreTheSameType<typename UniquePtr::DeleterType, TDefaultDeleter<Foo>>::Value);
	// constructors
	{
		UniquePtr p1{};
		TRNT_TEST_EXPECT_EQ(p1, nullptr);

		UniquePtr p2{ nullptr };
		TRNT_TEST_EXPECT_EQ(p2, nullptr);

		UniquePtr p3{ new Foo() };
		TRNT_TEST_EXPECT_NEQ(p3, nullptr);
		TRNT_TEST_EXPECT_EQ(p3->I32, 0);
		TRNT_TEST_EXPECT_EQ(p3->F32, 0.0f);

		UniquePtr p4{ Move(p3) };
		TRNT_TEST_EXPECT_NEQ(p4, nullptr);
		TRNT_TEST_EXPECT_EQ(p3, nullptr);
		TRNT_TEST_EXPECT_EQ(p4->I32, 0);
		TRNT_TEST_EXPECT_EQ(p4->F32, 0.0f);
	}

	// operator=
	{
		UniquePtr p1{ new Foo() };
		TRNT_TEST_EXPECT_NEQ(p1, nullptr);
		TRNT_TEST_EXPECT_EQ(p1->I32, 0);
		TRNT_TEST_EXPECT_EQ(p1->F32, 0.0f);

		UniquePtr p2{};
		TRNT_TEST_EXPECT_EQ(p2, nullptr);
		p2 = Move(p1);
		TRNT_TEST_EXPECT_EQ(p1, nullptr);
		TRNT_TEST_EXPECT_NEQ(p2, nullptr);
		TRNT_TEST_EXPECT_EQ(p2->I32, 0);
		TRNT_TEST_EXPECT_EQ(p2->F32, 0.0f);
		p2 = nullptr;
		TRNT_TEST_EXPECT_EQ(p2, nullptr);
	}

	{
		UniquePtr p{ new Foo() };

		Foo* pp = p.Get();
		TRNT_TEST_EXPECT_TRUE(pp != nullptr);
		TRNT_TEST_EXPECT_TRUE(pp->I32 == 0 && pp->F32 == 0.0f);

		if (p) // operator bool()
		{
		}

		// p[0] -> failed :v, because TUniquePtr<Foo> with Type = Foo is not array type

		TRNT_TEST_EXPECT_TRUE(p->I32 == 0);
		TRNT_TEST_EXPECT_TRUE(p->F32 == 0.0f);

		TRNT_TEST_EXPECT_TRUE((*p).I32 == 0);
		TRNT_TEST_EXPECT_TRUE((*p).F32 == 0.0f);
		p->I32 = 1000;
		p->F32 = 3.14f;

		TRNT_TEST_EXPECT_TRUE(p->I32 == 1000);
		TRNT_TEST_EXPECT_TRUE(p->F32 == 3.14f);
	}

	{
		UniquePtr uptr{ new Foo() };

		Foo* p = uptr.Release();
		TRNT_TEST_EXPECT_EQ(uptr, nullptr);

		UniquePtr new_uptr{ p };
		TRNT_TEST_EXPECT_TRUE(new_uptr->I32 == 0);
		TRNT_TEST_EXPECT_TRUE(new_uptr->F32 == 0.0f);

		Foo* new_p = new Foo();
		new_p->I32 = 999;
		new_p->F32 = 111.222f;

		new_uptr.Reset(new_p);
		TRNT_TEST_EXPECT_TRUE(new_uptr->I32 == 999);
		TRNT_TEST_EXPECT_TRUE(new_uptr->F32 == 111.222f);

		new_uptr.Reset();
		TRNT_TEST_EXPECT_EQ(new_uptr, nullptr);
	}

	{
		UniquePtr p1{ new Foo() };
		UniquePtr p2{ new Foo() };

		TRNT_TEST_EXPECT_TRUE(p1 == p1);
		TRNT_TEST_EXPECT_TRUE(p1 != p2);
		TRNT_TEST_EXPECT_TRUE(p1 != nullptr);
		TRNT_TEST_EXPECT_TRUE(nullptr != p2);

		p2.Reset();
		TRNT_TEST_EXPECT_TRUE(nullptr == p2);
		TRNT_TEST_EXPECT_TRUE(p2 == nullptr);
	}

	{
		UniquePtr p = MakeUnique<Foo>(100, 90.0f);
		TRNT_TEST_EXPECT_NEQ(p, nullptr);
		TRNT_TEST_EXPECT_EQ(p->I32, 100);
		TRNT_TEST_EXPECT_EQ(p->F32, 90.0f);
	}

	class TBase
	{
	public:
		int v1 = 0xAAAAAAAA;
	};

	class TDerived : public TBase
	{
	public:
		int v2 = 0xBBBBBBBB;
	};

	static_assert(TIsConvertible<TDerived, TBase>::Value);
	static_assert(!TIsConvertible<TBase, TDerived>::Value);
	{
		TUniquePtr<TDerived> p1(new TDerived);
		TRNT_TEST_EXPECT_TRUE(p1->v2 == 0xBBBBBBBB && p1->v1 == 0xAAAAAAAA);
		TUniquePtr<TBase> p2(Move(p1));
		TRNT_TEST_EXPECT_TRUE(p2->v1 == 0xAAAAAAAA);
	}
}

TRNT_IMPL_TEST_CASE(Types, TUniquePtr_Array)
{
	using UniquePtr = TUniquePtr<Foo[]>;
	static_assert(TAreTheSameType<typename UniquePtr::ElementType, Foo>::Value);
	static_assert(TAreTheSameType<typename UniquePtr::PointerType, Foo*>::Value);
	static_assert(TAreTheSameType<typename UniquePtr::DeleterType, TDefaultDeleter<Foo[]>>::Value);
	{
		UniquePtr p1{};
		TRNT_TEST_EXPECT_EQ(p1, nullptr);

		UniquePtr p2{ nullptr };
		TRNT_TEST_EXPECT_EQ(p2, nullptr);

		UniquePtr p3 = MakeUnique<Foo[]>(3);
		TRNT_TEST_EXPECT_NEQ(p3, nullptr);
		TRNT_TEST_EXPECT_EQ(p3[0].I32, 0);
		TRNT_TEST_EXPECT_EQ(p3[0].F32, 0.0f);

		UniquePtr p4{ Move(p3) };
		TRNT_TEST_EXPECT_NEQ(p4, nullptr);
		TRNT_TEST_EXPECT_EQ(p3, nullptr);

		p4[1].I32 = 1;
		p4[1].F32 = 1.0f;

		TRNT_TEST_EXPECT_EQ(p4[1].I32, 1);
		TRNT_TEST_EXPECT_EQ(p4[1].F32, 1.0f);
	}

	// operator=
	{
		UniquePtr p1{ new Foo[3]{ Foo(1, 1.0f), Foo(2, 2.0f), Foo(3, 3.0f) } };
		TRNT_TEST_EXPECT_NEQ(p1, nullptr);
		TRNT_TEST_EXPECT_EQ(p1[0].I32, 1);
		TRNT_TEST_EXPECT_EQ(p1[0].F32, 1.0f);
		TRNT_TEST_EXPECT_EQ(p1[1].I32, 2);
		TRNT_TEST_EXPECT_EQ(p1[1].F32, 2.0f);
		TRNT_TEST_EXPECT_EQ(p1[2].I32, 3);
		TRNT_TEST_EXPECT_EQ(p1[2].F32, 3.0f);

		UniquePtr p2{};
		TRNT_TEST_EXPECT_EQ(p2, nullptr);
		p2 = Move(p1);
		TRNT_TEST_EXPECT_EQ(p1, nullptr);
		TRNT_TEST_EXPECT_NEQ(p2, nullptr);
		TRNT_TEST_EXPECT_EQ(p2[0].I32, 1);
		TRNT_TEST_EXPECT_EQ(p2[0].F32, 1.0f);
		TRNT_TEST_EXPECT_EQ(p2[1].I32, 2);
		TRNT_TEST_EXPECT_EQ(p2[1].F32, 2.0f);
		TRNT_TEST_EXPECT_EQ(p2[2].I32, 3);
		TRNT_TEST_EXPECT_EQ(p2[2].F32, 3.0f);
		p2 = nullptr;
		TRNT_TEST_EXPECT_EQ(p2, nullptr);
	}

	{
		UniquePtr p1 = MakeUnique<Foo[]>(1);
		UniquePtr p2 = MakeUnique<Foo[]>(2);

		TRNT_TEST_EXPECT_TRUE(p1 == p1);
		TRNT_TEST_EXPECT_TRUE(p1 != p2);
		TRNT_TEST_EXPECT_TRUE(p1 != nullptr);
		TRNT_TEST_EXPECT_TRUE(nullptr != p2);

		p2.Reset();
		TRNT_TEST_EXPECT_TRUE(nullptr == p2);
		TRNT_TEST_EXPECT_TRUE(p2 == nullptr);
	}

	{
		UniquePtr p = MakeUnique<Foo[]>(2);

		Foo* pp = p.Get();

		TRNT_TEST_EXPECT_TRUE(pp != nullptr);

		if (p) // operator bool()
		{
		}
	}

	{
		const TSize_T COUNT = 3;
		UniquePtr uptr = MakeUnique<Foo[]>(COUNT);

		for (TSize_T Index = 0; Index < COUNT; ++Index)
		{
			uptr[Index].I32 = TInt32(Index + 1);
			uptr[Index].F32 = TFloat(Index + 1);
		}

		Foo* p = uptr.Release();

		TRNT_TEST_EXPECT_EQ(uptr, nullptr);

		UniquePtr new_uptr{ p };
		TRNT_TEST_EXPECT_TRUE(new_uptr[0].I32 == 1);
		TRNT_TEST_EXPECT_TRUE(new_uptr[0].F32 == 1.0f);

		Foo* new_p = new Foo[2];
		new_p[0].I32 = 100;
		new_p[0].F32 = 100.0f;
		new_p[1].I32 = 200;
		new_p[1].F32 = 200.0f;

		new_uptr.Reset(new_p);
		TRNT_TEST_EXPECT_TRUE(new_uptr[0].I32 == 100);
		TRNT_TEST_EXPECT_TRUE(new_uptr[0].F32 == 100.0f);

		new_uptr.Reset();
		TRNT_TEST_EXPECT_EQ(new_uptr, nullptr);
	}
}