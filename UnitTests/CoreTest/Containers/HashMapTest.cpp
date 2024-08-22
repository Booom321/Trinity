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

		TRNT_TEST_EXPECT_TRUE(m1.Contains(1));
		TRNT_TEST_EXPECT_TRUE(m1.Contains(-9999999) == false);

		TRNT_TEST_EXPECT_TRUE(m1.ContainsByHash(TStringIDMap::GetHashCode(99)));
		TRNT_TEST_EXPECT_TRUE(m1.ContainsByHash(TStringIDMap::GetHashCode(-100)) == false);

		TRNT_TEST_EXPECT_TRUE(m1.Count(1) == 1);
		TRNT_TEST_EXPECT_TRUE(m1.Count(-1) == 0); // key -1 does not exist on m1

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

	{
		TStringIDMap m1{ {1, "abc"}, {2, "def"}, {123, "aaa"}, {99, "hello"}, {1234, "aaabbb"}, {100, "world"} };

		TDynamicArray<typename TStringIDMap::KeyType> outKeys;
		TInt64 keyCount = m1.GetKeys(outKeys);

		TRNT_TEST_EXPECT_TRUE(keyCount == 6);
		TRNT_TEST_EXPECT_TRUE(outKeys.Contains(1));
		TRNT_TEST_EXPECT_TRUE(outKeys.Contains(2));
		TRNT_TEST_EXPECT_TRUE(outKeys.Contains(123));
		TRNT_TEST_EXPECT_TRUE(outKeys.Contains(99));
		TRNT_TEST_EXPECT_TRUE(outKeys.Contains(1234));
		TRNT_TEST_EXPECT_TRUE(outKeys.Contains(100));

		TDynamicArray<typename TStringIDMap::ValueType> outVals;
		TInt64 valCount = m1.GetValues(outVals);
		TRNT_TEST_EXPECT_TRUE(valCount == m1.GetElementCount());
		TRNT_TEST_EXPECT_TRUE(outVals.Contains("abc"));
		TRNT_TEST_EXPECT_TRUE(outVals.Contains("def"));
		TRNT_TEST_EXPECT_TRUE(outVals.Contains("aaa"));
		TRNT_TEST_EXPECT_TRUE(outVals.Contains("hello"));
		TRNT_TEST_EXPECT_TRUE(outVals.Contains("aaabbb"));
		TRNT_TEST_EXPECT_TRUE(outVals.Contains("world"));

		TDynamicArray<typename TStringIDMap::ElementType> outElems;
		TInt64 elemCount = m1.GetElements(outElems);
		TRNT_TEST_EXPECT_TRUE(elemCount== m1.GetElementCount());

		const auto ContainsElement = [&outElems](TInt32 Id, const typename TStringIDMap::ValueType Value) -> TBool
		{
			for (auto& elem : outElems)
			{
				if (elem.Key == Id && elem.Value == Value)
				{
					return true;
				}
			}
			return false;
		};

		ContainsElement(1, "abc");
		ContainsElement(2, "def");
		ContainsElement(123, "aaa");
		ContainsElement(99, "hello");
		ContainsElement(1234, "aaabbb");
		ContainsElement(100, "world");
	}

	{
		TStringIDMap m1{ {1, "abc"}, {2, "def"}, {123, "aaa"}, {99, "hello"}, {1234, "aaabbb"}, {100, "world"} };

		TDynamicArray<typename TStringIDMap::KeyType> outKeys;
		m1.FilterKeys([](typename TStringIDMap::KeyType Key) { return Key <= 100; }, outKeys);
		TRNT_TEST_EXPECT_TRUE(outKeys.Contains(1));
		TRNT_TEST_EXPECT_TRUE(outKeys.Contains(2));
		TRNT_TEST_EXPECT_TRUE(outKeys.Contains(99));
		TRNT_TEST_EXPECT_TRUE(outKeys.Contains(100));

		TDynamicArray<typename TStringIDMap::ValueType> outVals;
		m1.FilterValues([](typename TStringIDMap::ValueType Val) { return Val.GetElementCount() == 3; }, outVals);
		TRNT_TEST_EXPECT_TRUE(outVals.Contains("abc"));
		TRNT_TEST_EXPECT_TRUE(outVals.Contains("def"));
		TRNT_TEST_EXPECT_TRUE(outVals.Contains("aaa"));
	}

	{
		TStringIDMap map{};
		map.Emplace(1, "hello world");
		TRNT_TEST_CHECK_MAP(TStringIDMap, map, 1, { 1, "hello world" });

		map.Emplace(1, "hello");
		TRNT_TEST_CHECK_MAP(TStringIDMap, map, 1, { 1, "hello world" });

		map.EmplaceByHash(TStringIDMap::GetHashCode(100), 100, "hehehe");
		TRNT_TEST_CHECK_MAP(TStringIDMap, map, 2, { 1, "hello world" }, { 100, "hehehe" });
	}
}

#undef TRNT_TEST_CHECK_MAP