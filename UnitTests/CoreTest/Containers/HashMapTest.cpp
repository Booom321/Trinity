#include "HashMapTest.h"

#include <Trinity/Core/Containers/LinkedList.h>
#include <Trinity/Core/Containers/HashMap.h>
#include <Trinity/Core/String/String.h>

template<typename HashMapType>
static TBool Check(const HashMapType& HashMap, std::initializer_list<typename HashMapType::PairType> ExpectedPairs, typename HashMapType::SizeType ExpectedElementCount)
{
	if (HashMap.GetElementCount() != ExpectedElementCount || ExpectedPairs.size() != ExpectedElementCount)
	{
		return false;
	}

	TRNT_ASSERT(HashMap.GetLoadFactor() <= HashMapType::MaxLoadFactor);

	const typename HashMapType::PairType* Begin = ExpectedPairs.begin(), * End = ExpectedPairs.end();

	while (Begin != End)
	{
		const typename HashMapType::ElementListType::NodeType* FoundNode = HashMap.Find(Begin->First);

		if (FoundNode == nullptr)
		{
			return false;
		}

		if (!(FoundNode->Value.Value == Begin->Second))
		{
			return false;
		}

		++Begin;
	}
	
	return true;
}

#define TRNT_TEST_CHECK_MAP(MapType, MapToCheck, ExpectedElementCount, ...) TRNT_TEST_EXPECT_TRUE(Check<MapType>(MapToCheck, { __VA_ARGS__ }, ExpectedElementCount))

TRNT_IMPL_TEST_CASE(Containers, THashMap)
{
	using TStringIDMap = THashMap<int, TString>;

	// constructor
	{
		TStringIDMap m1;
		TRNT_TEST_CHECK_MAP(TStringIDMap, m1, 0);

		TStringIDMap m2{ {1, "abc"}, {2, "def"}, {123, "aaa"}, {99, "hello"}, {1234, "aaabbb"}, {100, "world"} };
		TRNT_TEST_CHECK_MAP(TStringIDMap, m2, 6, { 99, "hello" }, { 1, "abc" }, { 123, "aaa" }, { 2, "def" }, { 100, "world" }, { 1234, "aaabbb" });

		TStringIDMap m3{ {1, "abc"}, {2, "def"}, {123, "aaa"}, {99, "hello"}, {1234, "aaabbb"}, {100, "world"}, {100, "lol"} }; 
		TRNT_TEST_CHECK_MAP(TStringIDMap, m3, 6, { 99, "hello" }, { 1, "abc" }, { 123, "aaa" }, { 2, "def" }, { 100, "world" }, { 1234, "aaabbb" });

		///////////////////////////// copy constructor /////////////////////////////
		TStringIDMap m4(m2);
		TStringIDMap m5 = m2;
		TRNT_TEST_CHECK_MAP(TStringIDMap, m4, 6, { 99, "hello" }, { 1, "abc" }, { 123, "aaa" }, { 2, "def" }, { 100, "world" }, { 1234, "aaabbb" });
		TRNT_TEST_EXPECT_TRUE(m4 == m2);
		TRNT_TEST_EXPECT_TRUE(m5 == m2);

		///////////////////////////// move constructor //////////////////////////////
		TRNT_TEST_EXPECT_TRUE(m2.GetElementCount() >= 0 && m2.GetBucketData() != nullptr && m2.GetBucketCount() >= 0);
		
		TStringIDMap m6(Move(m2));
		TRNT_TEST_CHECK_MAP(TStringIDMap, m6, 6, { 99, "hello" }, { 1, "abc" }, { 123, "aaa" }, { 2, "def" }, { 100, "world" }, { 1234, "aaabbb" });

		TRNT_TEST_EXPECT_TRUE(m2.GetElementCount() == 0 && m2.GetBucketData() == nullptr && m2.GetBucketCount() == 0); // m2 is now empty!!!
		TRNT_TEST_EXPECT_TRUE(m2 != m6);

		/////////////////////////////////////////////////////////////////////////////
		TStringIDMap m7(3);
		TRNT_TEST_CHECK_MAP(TStringIDMap, m7, 0);

		TRNT_TEST_EXPECT_TRUE(m7.GetBucketCount() == 3 && m7.GetBucketData() != nullptr);

		TStringIDMap m8(0);
		TRNT_TEST_EXPECT_TRUE(m8.GetBucketCount() == 8 && m8.GetBucketData() != nullptr);

		TStringIDMap m9(-999);
		TRNT_TEST_EXPECT_TRUE(m9.GetBucketCount() == 8 && m9.GetBucketData() != nullptr);
	}

	// Element access
	{
		TStringIDMap m1{ {1, "abc"}, {2, "def"}, {123, "aaa"}, {99, "hello"}, {1234, "aaabbb"}, {100, "world"} };

		TRNT_TEST_EXPECT_TRUE(m1.GetElementCount() == 6);
		TRNT_TEST_EXPECT_TRUE(m1.GetBucketCount() == 8);
		
		static_assert(TStringIDMap::GetMaxLoadFactor() == 0.75f);

		TRNT_TEST_EXPECT_TRUE(!m1.IsEmpty());
		TRNT_TEST_EXPECT_TRUE(m1.GetBucketData() != nullptr);

		/// operator[]

		m1[1] = "hello";
		TRNT_TEST_CHECK_MAP(TStringIDMap, m1, 6, { 1, "hello" }, { 2, "def" }, { 123, "aaa" }, { 99, "hello" }, { 1234, "aaabbb" }, { 100, "world" });
		
		m1[998877] = "world";
		TRNT_TEST_CHECK_MAP(TStringIDMap, m1, 7, { 1, "hello" }, { 2, "def" }, { 123, "aaa" }, { 99, "hello" }, { 1234, "aaabbb" }, { 100, "world" }, { 998877, "world" });

		TRNT_TEST_EXPECT_TRUE(m1[2] == "def");
		TRNT_TEST_EXPECT_TRUE(m1[123] == "aaa");

		/// At(const KeyType &) const ///
		TRNT_TEST_EXPECT_TRUE(m1.At(99) == "hello");

		// assertion failed: m1.At(-100);

		m1.Clear();
		TRNT_TEST_CHECK_MAP(TStringIDMap, m1, 0);
	}
}

#undef TRNT_TEST_CHECK_MAP