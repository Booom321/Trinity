#include "DynamicArrayTest.h"

#include <Trinity/Core/Containers/DynamicArray.h>
#include <Trinity/Core/String/String.h>

static_assert(TIsTrivial<TInt32>::Value);
static_assert(TIsTriviallyCopyAssignable<TInt32>::Value);
static_assert(TIsTriviallyCopyConstructible<TInt32>::Value);
static_assert(TIsTriviallyDefaultConstructible<TInt32>::Value);
static_assert(TIsTriviallyMoveAssignable<TInt32>::Value);
static_assert(TIsTriviallyMoveConstructible<TInt32>::Value);
static_assert(TIsTriviallyDestructible<TInt32>::Value);

template<typename T>
TBool Check(const TDynamicArray<T>& X, const TDynamicArray<T>& Expected, TInt64 ExpectedLen)
{
	return (X == Expected) && (X.GetElementCount() == ExpectedLen) && (X.Capacity() >= ExpectedLen);
}

TRNT_IMPL_TEST_CASE(Containers, TDynamicArrayWithTrivialType)
{
	using Array = TDynamicArray<TInt32>;

	{
		Array a;
		TRNT_TEST_EXPECT_TRUE(a.IsEmpty());
		TRNT_TEST_EXPECT_TRUE(a.GetElementCount() == 0);
		TRNT_TEST_EXPECT_TRUE(a.Capacity() == 0);

		Array a1{ 1, 2, 3, 4, 5, 6, 7 };
		TRNT_TEST_EXPECT_TRUE(Check(a1, Array{ 1, 2, 3, 4, 5, 6, 7 }, 7));

		Array a2(a1);
		TRNT_TEST_EXPECT_TRUE(Check(a2, a1, 7));

		Array a3 = a1;

		TRNT_TEST_EXPECT_TRUE(a2 == a1);
		TRNT_TEST_EXPECT_TRUE(a3 == a1);

		Array a4(a1, 2, 5);
		// Array a4(a1, 2, -5);
		// failed: Array a4(a1, -2, 5);
		TRNT_TEST_EXPECT_TRUE(a4.GetData() != nullptr);
		TRNT_TEST_EXPECT_TRUE(Check(a4, Array{ 3, 4, 5, 6, 7 }, 5));

		Array a5(Move(a4));
		TRNT_TEST_EXPECT_TRUE(Check(a5, Array{ 3, 4, 5, 6, 7 }, 5));
		TRNT_TEST_EXPECT_TRUE(a5.GetData() != nullptr);
		TRNT_TEST_EXPECT_TRUE(a4.GetData() == nullptr);
		TRNT_TEST_EXPECT_TRUE(a4.GetElementCount() == 0 && a4.Capacity() == 0);

		Array a6(10);
		// failed: Array a6(-10);
		TRNT_TEST_EXPECT_TRUE(a6.GetElementCount() == 0 && a6.Capacity() == 10);

		Array a7(3, a1);
		// failed: Array a7(-3, a1);
		TRNT_TEST_EXPECT_TRUE(Check(a7, Array{ 1, 2, 3 }, 3));

		Array a8(4, a1.GetData());
		// failed: Array a8(-4, a1.GetData());
		TRNT_TEST_EXPECT_TRUE(Check(a8, Array{ 1, 2, 3, 4 }, 4));

		Array a9(5, a1.GetData(), 5);
		// failed: Array a9(-5, a1.GetData(), 5);
		// failed: Array a9(5, a1.GetData(), -5);
		TRNT_TEST_EXPECT_TRUE(a9.GetElementCount() == 5 && a9.Capacity() == 10);
		TRNT_TEST_EXPECT_TRUE(Check(a9, Array{ 1, 2, 3, 4, 5 }, 5));

		Array a10(a1.begin(), a1.begin() + 3);
		// failed: Array a10(a1.begin(), a1.begin() + -3);
		TRNT_TEST_EXPECT_TRUE(Check(a10, Array{ 1, 2, 3 }, 3));

		Array a11(a1.cbegin(), a1.cend());
		TRNT_TEST_EXPECT_TRUE(Check(a11, Array{ 1, 2, 3, 4, 5, 6, 7 }, 7));

		Array a12({}); // empty
		TRNT_TEST_EXPECT_TRUE(Check(a12, Array{}, 0));
	}

	{
		Array a1;

		Array a2{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		TRNT_TEST_EXPECT_TRUE(a2[0] == 1);
		TRNT_TEST_EXPECT_TRUE(a2[1] == 2);
		TRNT_TEST_EXPECT_TRUE(a2[2] == 3);
		TRNT_TEST_EXPECT_TRUE(a2[3] == 4);
		TRNT_TEST_EXPECT_TRUE(a2[4] == 5);
		TRNT_TEST_EXPECT_TRUE(a2[5] == 6);
		TRNT_TEST_EXPECT_TRUE(a2[6] == 7);
		TRNT_TEST_EXPECT_TRUE(a2[7] == 8);
		TRNT_TEST_EXPECT_TRUE(a2[8] == 9);

		TRNT_TEST_EXPECT_TRUE(a2.GetData() != nullptr);
		TRNT_TEST_EXPECT_TRUE(a2.GetElementCount() == 9);
		TRNT_TEST_EXPECT_TRUE(a2.Capacity() == 9);

		a1 = a2;
		TRNT_TEST_EXPECT_TRUE(a1 == a2);

		a1 = Move(a2);
		TRNT_TEST_EXPECT_TRUE(a1.GetData() != nullptr);
		TRNT_TEST_EXPECT_TRUE(a1.GetElementCount() == 9 && a1.Capacity() == 9);
		TRNT_TEST_EXPECT_TRUE(a2.GetData() == nullptr);
		TRNT_TEST_EXPECT_TRUE(a2.GetElementCount() == 0 && a2.Capacity() == 0);

		a1 = { 111, 222, 333, 444, 555, 66, 777, 8, 1111, 222, 3333 };
		TRNT_TEST_EXPECT_TRUE(Check(a1, Array{ 111, 222, 333, 444, 555, 66, 777, 8, 1111, 222, 3333 }, 11));

		a2 = { 1, 2, 3 };
		TRNT_TEST_EXPECT_TRUE(Check(a2, Array{ 1, 2, 3 }, 3));

		a1 = a2;
		TRNT_TEST_EXPECT_TRUE(Check(a1, Array{ 1, 2, 3 }, 3));

		a1 = {};
		TRNT_TEST_EXPECT_TRUE(Check(a1, Array{}, 0));
	}

	{
		Array a1, a2;
		for (TInt32 i = 0; i < 10; i++)
		{
			a1.Push(i % 5);
			a2.Push(i % 5);
		}

		TRNT_TEST_EXPECT_TRUE(a1 == a2);
		TRNT_TEST_EXPECT_TRUE(a1 == a1);
		TRNT_TEST_EXPECT_TRUE(a2 == a2);
		TRNT_TEST_EXPECT_TRUE(!(a1 != a2));

		a2.Push(1000);
		TRNT_TEST_EXPECT_TRUE(a2 != a1);
	}

	{
		Array a{ 1, 2, 3, 4, 5, 7, 8, 9, 1, 2, 3, 4 };
		TRNT_TEST_EXPECT_TRUE(Check(a, Array{ 1, 2, 3, 4, 5, 7, 8, 9, 1, 2, 3, 4 }, 12));

		TRNT_TEST_EXPECT_TRUE(a.FindElement(3) == 2);
		TRNT_TEST_EXPECT_TRUE(a.FindElement(-10) == Array::Npos);
		TRNT_TEST_EXPECT_TRUE(a.FindElementIf([](int a) -> bool
		{
			return a % 2 == 0;
		}) == 1);

		TRNT_TEST_EXPECT_TRUE(a.FindLastElement(3) == 10);
		TRNT_TEST_EXPECT_TRUE(a.FindLastElement(-10) == Array::Npos);
		TRNT_TEST_EXPECT_TRUE(a.FindLastElementIf([](int a) -> bool
		{
			return a % 2 == 0;
		}) == 11);

		TRNT_TEST_EXPECT_TRUE(a.Contains(7));
		TRNT_TEST_EXPECT_TRUE(!a.Contains(6));
		TRNT_TEST_EXPECT_TRUE(!a.ContainsIf([](int a) -> bool
		{
			return a == 99999;
		}));
	}

	{
		Array a{ 1, 2, 3, 4, 5, 6 };
		auto IsEven = [](int a) -> bool
		{
			return a % 2 == 0;
		};

		Array r = a.Filter(IsEven);
		TRNT_TEST_EXPECT_TRUE(Check(r, Array{ 2, 4, 6 }, 3));

		Array r1 = a.Filter([](int a) -> bool
		{
			return a < 0;
		});
		TRNT_TEST_EXPECT_TRUE(r1.IsEmpty());

		// for each
		auto IncByOne = [](int& a)
		{
			++a;
		};
		a.ForEachInternal(IncByOne);
		TRNT_TEST_EXPECT_TRUE(Check(a, Array{ 2, 3, 4, 5, 6, 7 }, 6));

		auto MultiplyBy2 = [](int& a)
		{
			a *= 2;
		};
		Array r2 = a.ForEach(MultiplyBy2);
		TRNT_TEST_EXPECT_TRUE(Check(r2, Array{ 4, 6, 8, 10, 12, 14 }, 6));
	}

	{
		Array a;
		// resize
		a.Resize(10);
		TRNT_TEST_EXPECT_TRUE(Check(a, Array{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 10));

		a.Resize(3);
		// failed: a.Resize(-3);
		TRNT_TEST_EXPECT_TRUE(Check(a, Array{ 0, 0, 0 }, 3));

		// clear
		a.Clear();
		TRNT_TEST_EXPECT_TRUE(a.IsEmpty());

		// reserve
		Array a1;

		a1.Reserve(10);
		TRNT_TEST_EXPECT_TRUE(a1.GetElementCount() == 0 && a1.Capacity() == 10);

		a1.Reserve(1);
		TRNT_TEST_EXPECT_TRUE(a1.GetElementCount() == 0 && a1.Capacity() == 10);

		a1.Resize(4);

		a1.Shrink();
		TRNT_TEST_EXPECT_TRUE(a1.GetElementCount() == 4 && a1.GetElementCount() == a1.Capacity());
	}

	{
		Array a{ 1, 2, 3, 4, 5, 6, 7 };
		TRNT_TEST_EXPECT_TRUE(!a.IsEmpty());
		TRNT_TEST_EXPECT_TRUE(a.GetElementCount() == 7);
		TRNT_TEST_EXPECT_TRUE(a.Capacity() == 7);
		TRNT_TEST_EXPECT_TRUE(a.RemainingCapacity() == 0);

		a.Reserve(10);
		TRNT_TEST_EXPECT_TRUE(a.RemainingCapacity() == 3);

		TRNT_TEST_EXPECT_TRUE(a.ElementAt(3) == 4);
		TRNT_TEST_EXPECT_TRUE(a[4] == 5);
		TRNT_TEST_EXPECT_TRUE(a.First() == 1);
		TRNT_TEST_EXPECT_TRUE(a.Last() == 7);
	}

	{
		Array a;
		TRNT_TEST_EXPECT_TRUE(a.IsEmpty());

		TInt32 item = 10;
		a.Add(item);
		a.Add(20);
		a.AddDefaulted(3);
		a.AddDefaulted();

		int& ref = a.AddDefaultedGetRef();
		ref += 1;

		int& ref1 = a.AddGetRef(100);
		ref1 += 100;

		int& ref2 = a.AddGetRef(item);
		ref2 += 1;

		a.AddUnique(-10);

		a.AddUnique(200);

		a.AddUnique(200);
		TRNT_TEST_EXPECT_TRUE(Check(a, Array{ 10, 20, 0, 0, 0, 0, 1, 200, 11, -10 }, 10));

		a.Clear();
		a.Shrink();
		TRNT_TEST_EXPECT_TRUE(a.GetElementCount() == 0 && a.Capacity() == 0);

		a.EmplaceBack(10);

		int& ref3 = a.EmplaceBackGetRef(100);
		ref3 += 100;

		a.Push(123);
		TRNT_TEST_EXPECT_TRUE(Check(a, Array{ 10, 200, 123 }, 3));
	}

	{
		Array a;
		for (TInt32 i = 1; i <= 30; ++i)
		{
			a.EmplaceBack(i % 5);
		}
		// a: 1 2 3 4 0 1 2 3 4 0 1 2 3 4 0 1 2 3 4 0 1 2 3 4 0 1 2 3 4 0

		// Pop
		int num = a.Pop();
		TRNT_TEST_EXPECT_TRUE(num == 0);

		// Remove
		a.Remove(1);

		a.RemoveAll(3);

		a.RemoveAllIf([](int a) -> bool
		{
			return a == 0;
		});

		a.RemoveAt(0, 3);
		// failed: a.RemoveAt(-1, -3);
		a.RemoveLast(4);

		a.RemoveLastIf([](int a) -> bool
		{
			return a % 2 == 1;
		});

		Array res{ 2, 4, 1, 2, 4, 1, 2, 4, 1, 2, 4, 2 };

		TRNT_TEST_EXPECT_TRUE(a == res);
	}

	{
		Array a;

		a.Insert(0, 10);

		int num = 100;
		a.Insert(0, num);
		a.InsertDefaulted(0);

		int& ref = a.InsertDefaultedGetRef(0);
		ref += 123;

		int& ref1 = a.InsertGetRef(3, 5);
		ref1 *= 2;

		num = 30;
		int& ref2 = a.InsertGetRef(5, num);
		ref2 += 10;

		a.Insert(4, { 12, 23, 34, 45, 56, 67 });
		a.EmplaceAt(2, 1000);

		int& ref3 = a.EmplaceAtGetRef(2, 330);
		ref3 += 70;

		TRNT_TEST_EXPECT_TRUE(Check(a, Array{ 123, 0, 400, 1000, 100, 10, 12, 23, 34, 45, 56, 67, 10, 40 }, 14));

		a.Clear();
		a.Shrink();

		Array b{ 1, 2, 3, 4, 5 };

		a.Insert(0, b);
		a.Insert(3, b.begin(), b.end());
		a.Insert(3, b.cbegin() + 3, b.cend());

		a.Insert(10, b.GetData(), 3);

		TRNT_TEST_EXPECT_TRUE(Check(a, Array{ 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5 }, 15));
	}

	{
		Array a;
		TRNT_TEST_EXPECT_TRUE(a.IsEmpty());

		Array b{ 1, 2, 3, 4, 5 };

		a.Append(b);
		a.Append(b.begin(), b.begin() + 3);
		a.Append(b.cbegin() + 3, b.cbegin() + 5);

		a.Append(5, 0);
		a.Append(3, b.GetData());

		a.Append({ 100, 200, 300, 400 });

		a.Append({});
		TRNT_TEST_EXPECT_TRUE(Check(a, Array{ 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 0, 0, 0, 0, 0, 1, 2, 3, 100, 200, 300, 400 }, 22));
	}
}

static_assert(!TIsTrivial<TString>::Value);

static_assert(!TIsTriviallyCopyAssignable<TString>::Value);
static_assert(!TIsTriviallyCopyConstructible<TString>::Value);
static_assert(!TIsTriviallyDefaultConstructible<TString>::Value);
static_assert(!TIsTriviallyMoveAssignable<TString>::Value);
static_assert(!TIsTriviallyMoveConstructible<TString>::Value);
static_assert(!TIsTriviallyDestructible<TString>::Value);

TRNT_IMPL_TEST_CASE(Containers, TDynamicArrayWithFString)
{
	using Array = TDynamicArray<TString>;

	{
		Array a;
		TRNT_TEST_EXPECT_TRUE(a.IsEmpty() && a.GetElementCount() == 0 && a.Capacity() == 0 && a.GetData() == nullptr);

		Array a1{ "aaa", "bbb", "ccc", "ddd", "eee", "111", "222", "333", "444" };
		TRNT_TEST_EXPECT_TRUE(Check(a1, Array{ "aaa", "bbb", "ccc", "ddd", "eee", "111", "222", "333", "444" }, 9));

		Array a2(a1);
		Array a3 = a1;
		TRNT_TEST_EXPECT_TRUE(a2 == a1 && a3 == a1);

		Array a4(a1, 0, 5);
		// failed: Array a4(a1, -1, 5);
		// failed: Array a4(a1, 1, -5);
		TRNT_TEST_EXPECT_TRUE(Check(a4, Array{ "aaa", "bbb", "ccc", "ddd", "eee" }, 5));

		Array a5(Array{ "1", "2", "3", "" });
		TRNT_TEST_EXPECT_TRUE(Check(a5, Array{ "1", "2", "3", "" }, 4));

		TRNT_TEST_EXPECT_TRUE(a4.GetData() != nullptr && a4.GetElementCount() == 5 && a4.Capacity() == 5);
		Array a6(Move(a4));
		TRNT_TEST_EXPECT_TRUE(a4.GetData() == nullptr && a4.GetElementCount() == 0 && a4.Capacity() == 0);
		TRNT_TEST_EXPECT_TRUE(a6.GetData() != nullptr && a6.GetElementCount() == 5 && a6.Capacity() == 5);
		TRNT_TEST_EXPECT_TRUE(Check(a6, Array{ "aaa", "bbb", "ccc", "ddd", "eee" }, 5));

		Array a7(10);
		// failed: Array a7(-10);
		TRNT_TEST_EXPECT_TRUE(a7.GetElementCount() == 0 && a7.Capacity() == 10);

		Array a_tmp{ "111", "222", "333", "444", "555", "666" };

		Array a8(a_tmp.begin(), a_tmp.end());
		TRNT_TEST_EXPECT_TRUE(Check(a8, Array{ "111", "222", "333", "444", "555", "666" }, 6));

		Array a9(a_tmp.cbegin(), a_tmp.cend());
		TRNT_TEST_EXPECT_TRUE(Check(a9, Array{ "111", "222", "333", "444", "555", "666" }, 6));

		Array a10(3, a_tmp);
		TRNT_TEST_EXPECT_TRUE(Check(a10, Array{ "111", "222", "333" }, 3));

		Array a11(4, a_tmp.GetData());
		TRNT_TEST_EXPECT_TRUE(Check(a11, Array{ "111", "222", "333", "444" }, 4));

		Array a12(4, a_tmp.GetData(), 3);
		// failed: Array a12(4, a_tmp.GetData(), -3);
		TRNT_TEST_EXPECT_TRUE(a12.Capacity() == 7);
		TRNT_TEST_EXPECT_TRUE(Check(a12, Array{ "111", "222", "333", "444" }, 4));

		Array a13({}); // empty std::initializer_list<TString>
		TRNT_TEST_EXPECT_TRUE(Check(a13, Array{}, 0));
	}

	{
		Array a1;
		TRNT_TEST_EXPECT_TRUE(a1.IsEmpty());

		Array a2{ "111", "222", "333" };

		a1 = a2;
		TRNT_TEST_EXPECT_TRUE(Check(a1, Array{ "111", "222", "333" }, 3));

		a1 = Array{ "aaa", "bbb", "ccc", "ddd", "eee", "fff" };
		TRNT_TEST_EXPECT_TRUE(Check(a1, Array{ "aaa", "bbb", "ccc", "ddd", "eee", "fff" }, 6));

		a1 = { "a", "b", "c" };
		TRNT_TEST_EXPECT_TRUE(Check(a1, Array{ "a", "b", "c" }, 3));

		TRNT_TEST_EXPECT_TRUE(a2.GetElementCount() == 3 && a2.Capacity() == 3 && a2.GetData() != nullptr);
		a1 = Move(a2);
		TRNT_TEST_EXPECT_TRUE(a2.GetElementCount() == 0 && a2.Capacity() == 0 && a2.GetData() == nullptr);
		TRNT_TEST_EXPECT_TRUE(a1.GetElementCount() == 3 && a1.Capacity() == 3 && a1.GetData() != nullptr);

		a1 = {};
		TRNT_TEST_EXPECT_TRUE(Check(a1, Array{}, 0));
	}

	{
		const TChar* cs = "aaabbbcccdddeee";
		TInt64 cslen = (TInt64)strlen(cs);

		Array x;
		Array y;

		for (TInt64 i = 0; i < cslen; ++i)
		{
			x.EmplaceBack(cs + i);
			y.EmplaceBack(cs + i);
		}

		TRNT_TEST_EXPECT_TRUE(x == y);
		TRNT_TEST_EXPECT_TRUE(x == x);
		TRNT_TEST_EXPECT_TRUE(y == x);
		TRNT_TEST_EXPECT_TRUE((x != y) == false);

		x.Push("");

		TRNT_TEST_EXPECT_TRUE(x != y);
	}

	{
		Array a{ "aaaa", "bbbb", "cccc", "dddd", "aaaa", "bbbb", "cccc" };

		TString s = "aaaa";

		TRNT_TEST_EXPECT_TRUE(a.Contains(s));
		TRNT_TEST_EXPECT_TRUE(!a.Contains("a123"));
		TRNT_TEST_EXPECT_TRUE(a.ContainsIf([](const TString& s)
		{
			return s.StartsWith("bb");
		}));

		TRNT_TEST_EXPECT_TRUE(a.FindElement(s) == 0);
		TRNT_TEST_EXPECT_TRUE(a.FindElement("ssss") == Array::Npos);
		TRNT_TEST_EXPECT_TRUE(a.FindElementIf([](const TString& s)
		{
			return s.StartsWith("c");
		}) == 2);
		TRNT_TEST_EXPECT_TRUE(a.FindElementIf([](const TString& s)
		{
			return s.StartsWith("e");
		}) == Array::Npos);

		TRNT_TEST_EXPECT_TRUE(a.FindLastElement(s) == 4);
		TRNT_TEST_EXPECT_TRUE(a.FindLastElement("ssss") == Array::Npos);
		TRNT_TEST_EXPECT_TRUE(a.FindLastElementIf([](const TString& s)
		{
			return s.StartsWith("c");
		}) == 6);
	}

	{
		Array a{ "aaaa", "bbbb", "cccc", "dddd", "eeee" };
		TRNT_TEST_EXPECT_TRUE(a.GetElementCount() == 5);

		a.Resize(3);
		TRNT_TEST_EXPECT_TRUE(Check(a, Array{ "aaaa", "bbbb", "cccc" }, 3));

		a.Resize(4);
		TRNT_TEST_EXPECT_TRUE(Check(a, Array{ "aaaa", "bbbb", "cccc", "" }, 4));

		a.Resize(10);
		TRNT_TEST_EXPECT_TRUE(Check(a, Array{ "aaaa", "bbbb", "cccc", "", "", "", "", "", "", "" }, 10));

		a.Reserve(20);
		TRNT_TEST_EXPECT_TRUE(a.Capacity() == 20);
		a.Reserve(10);
		TRNT_TEST_EXPECT_TRUE(a.Capacity() == 20);

		a.Shrink();
		TRNT_TEST_EXPECT_TRUE(a.Capacity() == a.GetElementCount());

		a.Clear();
		TRNT_TEST_EXPECT_TRUE(a.GetElementCount() == 0);
	}

	{
		Array a{ "aaaa", "bbbb", "cccc", "dddd", "eeee", "ffff" };
		auto func = [](const TString& a) -> bool
		{
			return a == "bbbb";
		};

		Array r = a.Filter(func);
		TRNT_TEST_EXPECT_TRUE(r.GetElementCount() == 1);

		Array r1 = a.Filter([](const TString& a) -> bool
		{
			return a > "zzzz";
		});
		TRNT_TEST_EXPECT_TRUE(r1.IsEmpty());

		// for each
		auto func1 = [](TString& a)
		{
			a.Append("aaaa");
		};
		a.ForEachInternal(func1);
		TRNT_TEST_EXPECT_TRUE(Check(a, Array{ "aaaaaaaa", "bbbbaaaa", "ccccaaaa", "ddddaaaa", "eeeeaaaa", "ffffaaaa" }, 6));

		auto func2 = [](TString& a)
		{
			a.Append("foo");
		};
		Array r2 = a.ForEach(func2);
		TRNT_TEST_EXPECT_TRUE(Check(r2, Array{ "aaaaaaaafoo", "bbbbaaaafoo", "ccccaaaafoo", "ddddaaaafoo", "eeeeaaaafoo", "ffffaaaafoo" }, 6));
	}

	{
		Array a{ "mango", "apple", "melon", "banana", "tomato", "avocado" };

		TRNT_TEST_EXPECT_TRUE(!a.IsEmpty());
		TRNT_TEST_EXPECT_TRUE(a.GetElementCount() == 6);
		TRNT_TEST_EXPECT_TRUE(a.Capacity() == 6);
		TRNT_TEST_EXPECT_TRUE(a.RemainingCapacity() == 0);

		a.Reserve(10);
		TRNT_TEST_EXPECT_TRUE(a.RemainingCapacity() == 4);

		TRNT_TEST_EXPECT_TRUE(a.ElementAt(3).Compare("banana") == 0);
		TRNT_TEST_EXPECT_TRUE(a[4].Compare("tomato") == 0);
		TRNT_TEST_EXPECT_TRUE(a.First().Compare("mango") == 0);
		TRNT_TEST_EXPECT_TRUE(a.Last().Compare("avocado") == 0);
	}

	{
		Array a;
		TString item = "aaaa";

		a.Add(item);
		a.Add(TString("bbbb"));

		a.AddDefaulted(3);

		a.AddUnique(item);
		a.AddUnique("cccc");

		TString& ref = a.AddDefaultedGetRef();
		ref = "dddd";

		item = "eeee";
		TString& ref1 = a.AddGetRef(item);
		ref1.Append("eeee");

		TString& ref2 = a.AddGetRef(Move(item));
		TRNT_TEST_EXPECT_TRUE(Check(a, Array{ "aaaa", "bbbb", "", "", "", "cccc", "dddd", "eeeeeeee", "eeee" }, 9));

		a.Clear();

		a.Push("aaaa");
		item = "BBBB";
		a.Push(item);

		a.EmplaceBack(4, 'c');							 // TString s(4, 'c');

		item = "aaaaddddeeee";
		TString& ref3 = a.EmplaceBackGetRef(item, 4, 4); // TString s(item, 4, 4);

		TRNT_TEST_EXPECT_TRUE(Check(a, Array{ "aaaa", "BBBB", "cccc", "dddd" }, 4));
	}

	{
		Array a{ "1234", "2314", "3214", "4312", "1341", "2342", "4321", "3421", "2314", "1341" };

		a.Remove("1234");
		a.RemoveAt(0, 2);
		a.RemoveAllIf([](const TString& s)
		{
			return s.StartsWith("4");
		});
		a.RemoveAll("1341");
		a.RemoveIf([](const TString& s)
		{
			return s.StartsWith("2");
		});

		TRNT_TEST_EXPECT_TRUE(Check(a, Array{ "3421", "2314" }, 2));

		a = { "1234", "1243", "1234", "1234", "4321" };

		a.RemoveLast("1234");
		a.RemoveLastIf([](const TString& s)
		{
			return s.EndsWith("4");
		});

		TRNT_TEST_EXPECT_TRUE(Check(a, Array{ "1234", "1243", "4321" }, 3));
		TRNT_TEST_EXPECT_TRUE(a.Pop().Compare("4321") == 0);
		TRNT_TEST_EXPECT_TRUE(Check(a, Array{ "1234", "1243" }, 2));
	}

	{
		Array a;

		a.Append({});
		a.Append({ "aaa", "bbb", "ccc", "ddd" });

		TString s("11");
		a.Append(3, s);

		Array b;
		a.Append(b);

		b.Add("mango");
		b.Add("melon");
		b.Add("apple");

		a.Append(b);

		TString arr[2];
		arr[0] = "foo";
		arr[1] = "bar";

		a.Append(2, arr);

		b = { "1", "2", "3", "4", "5", "6" };
		a.Append(b.begin(), b.end());
		// a.Append(b.cbegin(), b.cend());
		TRNT_TEST_EXPECT_TRUE(
			Check(a,
				Array{ "aaa", "bbb", "ccc", "ddd", "11", "11", "11", "mango", "melon", "apple", "foo", "bar", "1", "2", "3", "4", "5", "6" },
				18));
	}

	{
		Array a;

		// EmplaceAt
		a.EmplaceAt(0, 3, '3');

		TString& ref = a.EmplaceAtGetRef(0, "aaa");
		ref.Append("bbb");
		TRNT_TEST_EXPECT_TRUE(Check(a, Array{ "aaabbb", "333" }, 2));

		a.Clear();
		a.Shrink();

		TString item = "aaa";

		a.Insert(0, item);
		a.Insert(0, TString(3, 'b'));

		a.InsertDefaulted(2);

		TString& ref1 = a.InsertDefaultedGetRef(0);
		ref1 += "aaa";

		TString& ref2 = a.InsertGetRef(3, "ccc");
		ref2 += 'c';

		item = "ddd";
		TString& ref3 = a.InsertGetRef(3, item);
		ref3 += "eee";

		TRNT_TEST_EXPECT_TRUE(Check(a, Array{ "aaa", "bbb", "aaa", "dddeee", "cccc", "" }, 6));

		a.Clear();
		a.Shrink();

		a.Insert(0, {});
		a.Insert(0, { "12", "23", "34", "45" });

		Array b;

		a.Insert(2, b);

		b = { "33", "44", "55" };
		a.Insert(3, b);

		TString arr[3];
		arr[0] = "foo";
		arr[1] = "bar";
		arr[2] = "bar";

		a.Insert(3, arr, 3);
		a.Insert(0, arr, 3);

		TRNT_TEST_EXPECT_TRUE(Check(a, Array{ "foo", "bar", "bar", "12", "23", "34", "foo", "bar", "bar", "33", "44", "55", "45" }, 13));
	}
}