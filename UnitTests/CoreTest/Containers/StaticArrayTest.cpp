#include "StaticArrayTest.h"

#include <Trinity/Core/String/String.h>
#include <Trinity/Core/Containers/StaticArray.h>

template<TSize_T Count>
using StringArray = TStaticArray<TString, Count>;

TRNT_IMPL_TEST_CASE(Containers, TStaticArray)
{
	{
		StringArray<3> Arr;

		Arr[0] = "aaa";
		Arr[1] = "bbb";
		Arr[2] = "ccc";

		StringArray<3> Arr1(Arr);
		TRNT_TEST_EXPECT_EQ(Arr, Arr1);
		TRNT_TEST_EXPECT_EQ(Arr.GetElementCount(), Arr1.GetElementCount());

		StringArray<3> Arr2(Move(Arr1));
		TRNT_TEST_EXPECT_EQ(Arr, Arr2);
		TRNT_TEST_EXPECT_NEQ(Arr1, Arr2);
	}

	{
		StringArray<4> Arr;

		StringArray<4> Tmp;
		Tmp[0] = "foo";
		Tmp[1] = "foo";
		Tmp[2] = "bar";
		Tmp[3] = "bar";

		Arr = Tmp;
		TRNT_TEST_EXPECT_EQ(Arr, Tmp);
		TRNT_TEST_EXPECT_EQ(Arr.GetElementCount(), Tmp.GetElementCount());

		Arr = Move(Tmp);
		TRNT_TEST_EXPECT_TRUE(Arr[0] == "foo");
		TRNT_TEST_EXPECT_TRUE(Arr[1] == "foo");
		TRNT_TEST_EXPECT_TRUE(Arr[2] == "bar");
		TRNT_TEST_EXPECT_TRUE(Arr[3] == "bar");

		TRNT_TEST_EXPECT_TRUE(Tmp[0] == "");
		TRNT_TEST_EXPECT_TRUE(Tmp[1] == "");
		TRNT_TEST_EXPECT_TRUE(Tmp[2] == "");
		TRNT_TEST_EXPECT_TRUE(Tmp[3] == "");
	}

	{
		StringArray<4> Arr;
		Arr[0] = "foo";
		Arr[1] = "foo";
		Arr[2] = "bar";
		Arr[3] = "bar";

		StringArray<4> Arr1;
		Arr1[0] = "foo";
		Arr1[1] = "foo";
		Arr1[2] = "bar";
		Arr1[3] = "bar";

		TRNT_TEST_EXPECT_TRUE(Arr.GetElementCount() == 4);
		TRNT_TEST_EXPECT_TRUE(Arr.IsEmpty() == false);

		TRNT_TEST_EXPECT_TRUE(Arr == Arr1);
		Arr1[2] = "aaa";

		TRNT_TEST_EXPECT_TRUE(Arr != Arr1);
	}

	{
		StringArray<4> Arr;
		Arr[0] = "foo";
		Arr[1] = "foo";
		Arr[2] = "bar";
		Arr[3] = "bar";

		TString S;

		for (StringArray<4>::IteratorType It = Arr.begin(); It != Arr.end(); ++It)
		{
			S += *It;
		}

		TRNT_TEST_EXPECT_TRUE(S == "foofoobarbar");
	}
}