#include "SharedPtrTest.h"

#include <Trinity/Core/Types/SharedPtr.h>

class Foo
{
public:
	TRNT_FORCE_INLINE Foo() = default;
	TRNT_FORCE_INLINE Foo(TInt32 I32, TFloat F32) : I32(I32), F32(F32) {}
	Foo(const Foo&) = default;
	Foo(Foo&&) noexcept = default;
	~Foo() = default;

	Foo& operator=(const Foo&) = default;
	Foo& operator=(Foo&&) noexcept = default;

	TInt32 I32 = 0;
	TFloat F32 = 0.0f;
};

TRNT_IMPL_TEST_CASE(Types, TSharedPtr_Object)
{
	using SharedPtr = TSharedPtr<Foo>;
	static_assert(TAreTheSameType<typename SharedPtr::ElementType, Foo>::Value);
	static_assert(TAreTheSameType<typename SharedPtr::PointerType, Foo*>::Value);
	static_assert(TAreTheSameType<typename SharedPtr::DeleterType, TDefaultDeleter<Foo>>::Value);

	{
		SharedPtr sp{};
		TRNT_TEST_EXPECT_TRUE(sp.Get() == nullptr);
		TRNT_TEST_EXPECT_TRUE(!sp.IsUnique());
		TRNT_TEST_EXPECT_TRUE(sp.GetSharedReferenceCount() == 0);
 
		SharedPtr sp1{ nullptr };
		TRNT_TEST_EXPECT_TRUE(sp1.Get() == nullptr);
		TRNT_TEST_EXPECT_TRUE(!sp1.IsUnique());
		TRNT_TEST_EXPECT_TRUE(sp1.GetSharedReferenceCount() == 0);

		SharedPtr sp2 = MakeShared<Foo>(1, 2.0f); // ref count = 1
		TRNT_TEST_EXPECT_TRUE(sp2->I32 == 1 && sp2->F32 == 2.0f);
		TRNT_TEST_EXPECT_TRUE(sp2.IsUnique());
		TRNT_TEST_EXPECT_TRUE(sp2.GetSharedReferenceCount() == 1);

		SharedPtr sp2_copy{ sp2 }; // ref count = 2
		TRNT_TEST_EXPECT_TRUE(sp2_copy->I32 == 1 && sp2_copy->F32 == 2.0f);
		TRNT_TEST_EXPECT_TRUE(!sp2_copy.IsUnique());
		TRNT_TEST_EXPECT_TRUE(sp2_copy.GetSharedReferenceCount() == 2);

		SharedPtr sp2_move{ Move(sp2_copy) }; // ref count = 2
		TRNT_TEST_EXPECT_TRUE(!sp2_move.IsUnique());
		TRNT_TEST_EXPECT_TRUE(sp2_move.GetSharedReferenceCount() == 2);

		sp2_move.Reset();

		TRNT_TEST_EXPECT_TRUE(sp2_move.Get() == nullptr);
		TRNT_TEST_EXPECT_TRUE(sp2_move.GetSharedReferenceCount() == 0);

		TRNT_TEST_EXPECT_TRUE(sp2.IsUnique());

		if (sp2_move) // operator bool()
		{
			TRNT_TEST_EXPECT_TRUE(true);
		}
	}

	{
		SharedPtr sp = MakeShared<Foo>(1, 2.0f); // ref count = 1
		TRNT_TEST_EXPECT_TRUE(sp.IsUnique());


		SharedPtr other{ nullptr };
		other = sp; // ref count = 2
		TRNT_TEST_EXPECT_TRUE(other.GetSharedReferenceCount() == 2);

		SharedPtr sp1 = MakeShared<Foo>(2, 3.14f);
		other = Move(sp1);
		TRNT_TEST_EXPECT_TRUE(other.GetSharedReferenceCount() == 1);
		TRNT_TEST_EXPECT_TRUE(sp.IsUnique());
	}

	{
		SharedPtr sp1 = MakeShared<Foo>(1, 2.0f);
		SharedPtr sp2 = MakeShared<Foo>(1, 2.0f);
		SharedPtr sp3 { nullptr };

		TRNT_TEST_EXPECT_TRUE(sp1 != nullptr);
		TRNT_TEST_EXPECT_TRUE(sp2 != nullptr);
		TRNT_TEST_EXPECT_TRUE(sp3 == nullptr);

		TRNT_TEST_EXPECT_TRUE(sp1 != sp2);
		TRNT_TEST_EXPECT_TRUE(sp1 != sp2.Get());
		TRNT_TEST_EXPECT_TRUE(sp3.Get() != sp1);

		SharedPtr sp1_copy = sp1;
		TRNT_TEST_EXPECT_TRUE(sp1 == sp1_copy);
		TRNT_TEST_EXPECT_TRUE(nullptr != sp1);
	}
}

TRNT_IMPL_TEST_CASE(Types, TSharedPtr_Array)
{
	using SharedPtr = TSharedPtr<Foo[]>;
	static_assert(TAreTheSameType<typename SharedPtr::ElementType, Foo>::Value);
	static_assert(TAreTheSameType<typename SharedPtr::PointerType, Foo*>::Value);
	static_assert(TAreTheSameType<typename SharedPtr::DeleterType, TDefaultDeleter<Foo[]>>::Value);

	{
		SharedPtr sp{};
		TRNT_TEST_EXPECT_TRUE(sp.Get() == nullptr);
		TRNT_TEST_EXPECT_TRUE(!sp.IsUnique());
		TRNT_TEST_EXPECT_TRUE(sp.GetSharedReferenceCount() == 0);

		SharedPtr sp1{ nullptr };
		TRNT_TEST_EXPECT_TRUE(sp1.Get() == nullptr);
		TRNT_TEST_EXPECT_TRUE(!sp1.IsUnique());
		TRNT_TEST_EXPECT_TRUE(sp1.GetSharedReferenceCount() == 0);

		SharedPtr sp2 = MakeShared<Foo[]>(1); // ref count = 1
		TRNT_TEST_EXPECT_TRUE(sp2[0].I32 == 0 && sp2[0].F32 == 0.0f);
		TRNT_TEST_EXPECT_TRUE(sp2.IsUnique());
		TRNT_TEST_EXPECT_TRUE(sp2.GetSharedReferenceCount() == 1);

		SharedPtr sp2_copy{ sp2 }; // ref count = 2
		TRNT_TEST_EXPECT_TRUE(!sp2_copy.IsUnique());
		TRNT_TEST_EXPECT_TRUE(sp2_copy.GetSharedReferenceCount() == 2);

		SharedPtr sp2_move{ Move(sp2_copy) }; // ref count = 2
		TRNT_TEST_EXPECT_TRUE(!sp2_move.IsUnique());
		TRNT_TEST_EXPECT_TRUE(sp2_move.GetSharedReferenceCount() == 2);

		sp2_move.Reset();

		TRNT_TEST_EXPECT_TRUE(sp2_move.Get() == nullptr);
		TRNT_TEST_EXPECT_TRUE(sp2_move.GetSharedReferenceCount() == 0);

		TRNT_TEST_EXPECT_TRUE(sp2.IsUnique());

		if (sp2_move) // operator bool()
		{
			TRNT_TEST_EXPECT_TRUE(true);
		}
	}

	{
		SharedPtr sp = MakeShared<Foo[]>(3); // ref count = 1
		TRNT_TEST_EXPECT_TRUE(sp.IsUnique());


		SharedPtr other{ nullptr };
		other = sp; // ref count = 2
		TRNT_TEST_EXPECT_TRUE(other.GetSharedReferenceCount() == 2);

		SharedPtr sp1 = MakeShared<Foo[]>(2);
		other = Move(sp1);
		TRNT_TEST_EXPECT_TRUE(other.GetSharedReferenceCount() == 1);
		TRNT_TEST_EXPECT_TRUE(sp.IsUnique());
	}

	{
		SharedPtr sp1 = MakeShared<Foo[]>(2);
		SharedPtr sp2 = MakeShared<Foo[]>(2);
		SharedPtr sp3 { nullptr };

		TRNT_TEST_EXPECT_TRUE(sp1 != nullptr);
		TRNT_TEST_EXPECT_TRUE(sp2 != nullptr);
		TRNT_TEST_EXPECT_TRUE(sp3 == nullptr);

		TRNT_TEST_EXPECT_TRUE(sp1 != sp2);
		TRNT_TEST_EXPECT_TRUE(sp1 != sp2.Get());
		TRNT_TEST_EXPECT_TRUE(sp3.Get() != sp1);

		SharedPtr sp1_copy = sp1;
		TRNT_TEST_EXPECT_TRUE(sp1 == sp1_copy);
		TRNT_TEST_EXPECT_TRUE(nullptr != sp1);
		TRNT_TEST_EXPECT_TRUE(sp1[0].I32 == sp1_copy[0].I32 && sp1[0].F32 == sp1_copy[0].F32);
	}
}