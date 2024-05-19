#include "StringTest.h"

#include <Trinity/Core/String/String.h>
#include <Trinity/Core/Logging/Log.h>
#include <Trinity/Core/String/StringConversion.h>
#undef max
static constexpr TUInt64 max = std::numeric_limits<TUInt64>::max();

static TRNT_FORCE_INLINE TBool Check(const TString& Str, const TChar* ExpectedStr, TString::SizeType ExpectedStrLen)
{
	return Str == ExpectedStr && Str.GetElementCount() == ExpectedStrLen && Str.Capacity() >= ExpectedStrLen;
}

TRNT_IMPL_TEST_CASE(Strings, TString)
{
	// Constructor
	{
		TString Empty;
		TRNT_TEST_EXPECT_TRUE(Check(Empty, "", 0));

		TString S1("HELLO WORLD");
		TRNT_TEST_EXPECT_TRUE(Check(S1, "HELLO WORLD", 11));

		TString S2(S1);
		TRNT_TEST_EXPECT_TRUE(Check(S2, "HELLO WORLD", 11));

		TString S3(5, "HELLO WORLD");
		TRNT_TEST_EXPECT_TRUE(Check(S3, "HELLO", 5));
		// Assertion failed: TString S3(-5, "HELLO WORLD");

		TString S4(TString("FOO BAR"));
		TRNT_TEST_EXPECT_TRUE(Check(S4, "FOO BAR", 7));

		TString S5(Move(S4));
		TRNT_TEST_EXPECT_TRUE(Check(S5, "FOO BAR", 7));
		TRNT_TEST_EXPECT_TRUE(Check(S4, "", 0));

		TString S6(9, 'a');
		TRNT_TEST_EXPECT_TRUE(Check(S6, "aaaaaaaaa", 9));
		// Assertion failed: TString S6(-9, 'a');

		S6 = "aaabbbcccddd";
		TString S7(S6, 3, 9);
		// Assertion failed: TString S7(Str7, -3, -9);
		TRNT_ASSERT(Check(S7, "bbbcccddd", 9));
	}

	// Assignment
	{
		TString Str;

		Str = "aaabbbccc";
		TRNT_TEST_EXPECT_TRUE(Check(Str, "aaabbbccc", 9));

		Str = "aaa";
		TRNT_TEST_EXPECT_TRUE(Check(Str, "aaa", 3));

		Str = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
		TRNT_TEST_EXPECT_TRUE(Check(Str, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 52));

		TString Tmp{ "foo foo bar bar" };
		Str = Tmp;
		TRNT_TEST_EXPECT_TRUE(Check(Str, "foo foo bar bar", 15));

		Tmp = "1234567890";
		Str = Move(Tmp);

		TRNT_TEST_EXPECT_TRUE(Check(Str, "1234567890", 10));
		TRNT_TEST_EXPECT_TRUE(Check(Tmp, "", 0));
	}

	// Comparision
	{
		TString A("aaabbbcccddd");
		TString B;

		TRNT_TEST_EXPECT_TRUE(A.IsEquals("aaabbbcccddd"));
		TRNT_TEST_EXPECT_TRUE(A.IsEquals("aaABBbCCCDdd", TStringSearchCase::EIgnoreCase));

		B = "AAAbbbCCCddd";
		TRNT_TEST_EXPECT_TRUE(!A.IsEquals(B));
		TRNT_TEST_EXPECT_TRUE(A.IsEquals(B, TStringSearchCase::EIgnoreCase));

		//////////////////////////////////////////////////////////////////////
		TRNT_TEST_EXPECT_TRUE(A == "aaabbbcccddd");
		TRNT_TEST_EXPECT_TRUE(A != B);
		TRNT_TEST_EXPECT_TRUE("AAAbbbCCCddd" == B);
		TRNT_TEST_EXPECT_TRUE("aaabbb" < A);
		TRNT_TEST_EXPECT_TRUE("zzzzzzzzzzzzzzzzzzzz" >= A);
		TRNT_TEST_EXPECT_TRUE("z" > A);
		TRNT_TEST_EXPECT_TRUE("aaaa" <= A);
		TRNT_TEST_EXPECT_TRUE(A >= "aaabbbcccddd");
		TRNT_TEST_EXPECT_TRUE(A < "aaabbbccceeeeeee");
		TRNT_TEST_EXPECT_TRUE(B <= "AAAbbbCCCdddEEE");
		TRNT_TEST_EXPECT_TRUE(B < "AAAbbbCCCdddEEE");
		TRNT_TEST_EXPECT_TRUE(A != "aaa");
		//////////////////////////////////////////////////////////////////////
		// A = "aaabbbcccddd"
		// B = "AAAbbbCCCddd"
		TRNT_TEST_EXPECT_TRUE(A.Compare("aaabbbccc") > 0);
		TRNT_TEST_EXPECT_TRUE(A.Compare("AAABBBCCC", TStringSearchCase::EIgnoreCase) > 0);
		TRNT_TEST_EXPECT_TRUE(A.Compare(B, TStringSearchCase::EIgnoreCase) == 0);
		A = "bbbaaacccddd";
		B = "pppaaaccc";
		TRNT_TEST_EXPECT_TRUE(A.Compare(3, 6, B, 3, 6) == 0);
		TRNT_TEST_EXPECT_TRUE(A.Compare(0, 9, B, 0, 9) < 0);

		B = "pppAAaCCC";
		TRNT_TEST_EXPECT_TRUE(A.Compare(3, 6, B, 3, 6, TStringSearchCase::EIgnoreCase) == 0);
	}
	
	// Concatenate
	{
		TString Str;
		TString Tmp1("1111");
		TString Tmp2("2222");
		TString Tmp3("3333");

		Str = Tmp1 + Tmp2;
		TRNT_TEST_EXPECT_TRUE(Check(Str, "11112222", 8));

		Str = Tmp1 + "22223333";
		TRNT_TEST_EXPECT_TRUE(Check(Str, "111122223333", 12));

		Str = Tmp3 + TString("4444");
		TRNT_TEST_EXPECT_TRUE(Check(Str, "33334444", 8));

		Str = "1111" + Tmp2;
		TRNT_TEST_EXPECT_TRUE(Check(Str, "11112222", 8));

		Str = "aaaa" + TString("bbbb");
		TRNT_TEST_EXPECT_TRUE(Check(Str, "aaaabbbb", 8));

		Str = TString("Hello ") + TString("world");
		TRNT_TEST_EXPECT_TRUE(Check(Str, "Hello world", 11));

		Str = TString("AAA") + "BBB";
		TRNT_TEST_EXPECT_TRUE(Check(Str, "AAABBB", 6));

		Str = TString("AAA") + Tmp1;
		TRNT_TEST_EXPECT_TRUE(Check(Str, "AAA1111", 7));
	}

	{
		TString S = "aaabbbcccddd";
		TString Pre = "aaa";
		TString Suf = "ddd";
		TRNT_TEST_EXPECT_TRUE(S.StartsWith("aaacc", 3));
		TRNT_TEST_EXPECT_TRUE(S.StartsWith("AAA", TStringSearchCase::EIgnoreCase));
		TRNT_TEST_EXPECT_TRUE(S.StartsWith("AaAdddcc", 3, TStringSearchCase::EIgnoreCase));
		TRNT_TEST_EXPECT_TRUE(S.StartsWith(Pre));
		TRNT_TEST_EXPECT_TRUE(S.StartsWith(""));
		TRNT_TEST_EXPECT_FALSE(S.StartsWith("foo"));
		TRNT_TEST_EXPECT_FALSE(S.StartsWith("111222"));

		TRNT_TEST_EXPECT_TRUE(S.EndsWith(Suf));
		TRNT_TEST_EXPECT_TRUE(S.EndsWith("cccddd"));
		TRNT_TEST_EXPECT_TRUE(S.EndsWith("CCcDDdeee", 6, TStringSearchCase::EIgnoreCase));
		TRNT_TEST_EXPECT_TRUE(S.EndsWith(""));
		TRNT_TEST_EXPECT_TRUE(S.EndsWith("cCddD", TStringSearchCase::EIgnoreCase));
		TRNT_TEST_EXPECT_FALSE(S.EndsWith("1"));
		TRNT_TEST_EXPECT_FALSE(S.EndsWith("eee"));
	}

	// Find
	{
		TString Str("aaaa bbbb cccc dddd aaaa bbbb");
		TString Substr("aaaa");

		TRNT_TEST_EXPECT_TRUE(Str.Contains(Substr));
		TRNT_TEST_EXPECT_TRUE(Str.Contains("AaAA bBBB", TStringSearchCase::EIgnoreCase));

		TRNT_TEST_EXPECT_TRUE(Str.FindChar('b') == 5);
		TRNT_TEST_EXPECT_TRUE(Str.FindLastChar('b') == Str.GetElementCount() - 1);

		// CaseSensitive
		TRNT_TEST_EXPECT_TRUE(Str.Find("cccc") == 10);
		TRNT_TEST_EXPECT_TRUE(Str.Find("") == 0);
		TRNT_TEST_EXPECT_TRUE(Str.Find("", 3) == 3);
		TRNT_TEST_EXPECT_TRUE(Str.Find("cccc", -10) == 10);
		TRNT_TEST_EXPECT_TRUE(Str.Find(3, "dcccdd") == TString::Npos);
		TRNT_TEST_EXPECT_TRUE(Str.Find("aaaa", 10) == 20);
		TRNT_TEST_EXPECT_TRUE(Str.Find(Substr) == 0);

		TRNT_TEST_EXPECT_TRUE(Str.FindLast("aaaa") == 20);
		TRNT_TEST_EXPECT_TRUE(Str.FindLast("aaaa", 10) == 0);
		TRNT_TEST_EXPECT_TRUE(Str.FindLast(Substr, 1000) == 20);
		TRNT_TEST_EXPECT_TRUE(Str.FindLast(Substr, 15) == 0);
		TRNT_TEST_EXPECT_TRUE(Str.FindLast("", 10) == 10);
		TRNT_TEST_EXPECT_TRUE(Str.FindLast("") == Str.GetElementCount());
		TRNT_TEST_EXPECT_TRUE(Str.FindLast(3, "bbbbccc", 15) == 6);

		// IgnoreCase
		TStringSearchCase SearchCase = TStringSearchCase::EIgnoreCase;
		TRNT_TEST_EXPECT_TRUE(Str.Find("CCCc", 0, SearchCase) == 10);
		TRNT_TEST_EXPECT_TRUE(Str.Find("", 0, SearchCase) == 0);
		TRNT_TEST_EXPECT_TRUE(Str.Find("", 3, SearchCase) == 3);
		TRNT_TEST_EXPECT_TRUE(Str.Find("cCCc", -10, SearchCase) == 10);
		TRNT_TEST_EXPECT_TRUE(Str.Find(3, "DcCcdd", 0, SearchCase) == TString::Npos);
		TRNT_TEST_EXPECT_TRUE(Str.Find("AAaA", 10, SearchCase) == 20);

		TRNT_TEST_EXPECT_TRUE(Str.FindLast("AaaA", -1, SearchCase) == 20);
		TRNT_TEST_EXPECT_TRUE(Str.FindLast("Aaaa", 10, SearchCase) == 0);
		TRNT_TEST_EXPECT_TRUE(Str.FindLast(Substr, 1000, SearchCase) == 20);
		TRNT_TEST_EXPECT_TRUE(Str.FindLast(Substr, 15, SearchCase) == 0);
		TRNT_TEST_EXPECT_TRUE(Str.FindLast("", 10, SearchCase) == 10);
		TRNT_TEST_EXPECT_TRUE(Str.FindLast("", -1, SearchCase) == Str.GetElementCount());
		TRNT_TEST_EXPECT_TRUE(Str.FindLast(3, "BBbbCCc", 15, SearchCase) == 6);
	}

	{
		TString Str("AAaabBBbcCCc");

		Str.ToUpperInternal();
		TRNT_TEST_EXPECT_TRUE(Check(Str, "AAAABBBBCCCC", 12));

		Str.ToLowerInternal();
		TRNT_TEST_EXPECT_TRUE(Check(Str, "aaaabbbbcccc", 12));

		TString Rev = Str.Reverse();
		TRNT_TEST_EXPECT_TRUE(Check(Rev, "ccccbbbbaaaa", 12));

		Rev.ReverseInternal();
		TRNT_TEST_EXPECT_TRUE(Check(Rev, "aaaabbbbcccc", 12));

		Str.Resize(20, 'd');
		TRNT_TEST_EXPECT_TRUE(Check(Str, "aaaabbbbccccdddddddd", 20));

		Str.Resize(8);
		TRNT_TEST_EXPECT_TRUE(Check(Str, "aaaabbbb", 8));

		Str.Clear();
		TRNT_TEST_EXPECT_TRUE(Check(Str, "", 0));

		Str = "aaaaaaa";
		TString UpString = Str.ToUpper();
		TRNT_TEST_EXPECT_TRUE(Check(UpString, "AAAAAAA", 7));

		TString LowerString = Str.ToLower();
		TRNT_TEST_EXPECT_TRUE(Check(LowerString, "aaaaaaa", 7));
	}

	{
		TString Str("aaaabbbbcccc");

		TRNT_TEST_EXPECT_TRUE(Str.GetElementCount() == 12);
		TRNT_TEST_EXPECT_TRUE(Str.Capacity() == 12);

		TRNT_TEST_EXPECT_TRUE(Str.RemainingCapacity() == 0);

		for (TString::IteratorType it = Str.begin(); it != Str.end(); ++it)
		{
			*it = TCharUtils<TString::ElementType>::ToUpperCase(*it);
		}
		TRNT_TEST_EXPECT_TRUE(Str == "AAAABBBBCCCC");

		TRNT_TEST_EXPECT_TRUE(Str.CharAt(3) == 'A');
		TRNT_TEST_EXPECT_TRUE(Str[6] == 'B');

		TRNT_TEST_EXPECT_TRUE(Str.First() == 'A');
		TRNT_TEST_EXPECT_TRUE(Str.Last() == 'C');

		TRNT_TEST_EXPECT_TRUE(TCString<TString::ElementType>::Strcmp(Str.GetData(), "AAAABBBBCCCC") == 0);

		TRNT_TEST_EXPECT_TRUE(Str.IsEmpty() == false);

		Str.Reserve(20);
		TRNT_TEST_EXPECT_TRUE(Str.RemainingCapacity() == 8);
		TRNT_TEST_EXPECT_TRUE(Str.Capacity() == 20);

		Str.Reserve(1);
		TRNT_TEST_EXPECT_TRUE(Str.RemainingCapacity() == 8);
		TRNT_TEST_EXPECT_TRUE(Str.Capacity() == 20);

		TString res = Str.Substring(4);
		// Assertion failed: TString res = Str.Substring(-4);
		TRNT_TEST_EXPECT_TRUE(res == "BBBBCCCC");

		res = Str.Substring(4, 4);
		// Assertion failed: res = Str.Substring(-4, -4);
		TRNT_TEST_EXPECT_TRUE(res == "BBBB");
	}

	{
		TString S;
		TString Tmp;
		S.Append("aaa");
		S.Append(3, 'b');
		//failed: S.Append(-3, 'b');
		S.Append("cccbbb", 3);
		//failed: S.Append("cccbbb", -3);
		S.Append(Tmp);
		Tmp = "aaabbbcccddd";
		S.Append(Tmp, 9, 3);
		//failed: S.Append(Tmp, -9, -3);

		TRNT_TEST_EXPECT_TRUE(Check(S, "aaabbbcccddd", 12));

		S = "";
		S += "111";
		S += '2';
		Tmp = "22333";
		S += Tmp;
		S += "";
		TRNT_TEST_EXPECT_TRUE(Check(S, "111222333", 9));
	}

	{
		// 33333344400011133334444222
		TString S;

		S.Insert(0, "000");
		S.Insert(3, "111222", 3);
		S.Insert(6, 3, '2');

		TString Tmp{ "333444" };

		S.Insert(0, Tmp);
		S.Insert(0, Tmp, 0, 3);

		TRNT_TEST_EXPECT_TRUE(Check(S, "333333444000111222", 18));

		S.Insert(S.GetElementCount() - 3, "33334444");

		TRNT_TEST_EXPECT_TRUE(Check(S, "33333344400011133334444222", 26));
	}

	{
		TString S("hello hello world");
		S.RemoveAt(0, 6);
		//failed: S.RemoveAt(0, -6);

		TRNT_TEST_EXPECT_TRUE(Check(S, "hello world", 11));
	}

	// split
	{
		TString S("Welcome to Trinity Engine");

		TDynamicArray<TString> Res;
		TRNT_TEST_EXPECT_TRUE(S.SplitByString(Res));
		
		TRNT_TEST_EXPECT_TRUE(Res[0].IsEquals("Welcome"));
		TRNT_TEST_EXPECT_TRUE(Res[1].IsEquals("to"));
		TRNT_TEST_EXPECT_TRUE(Res[2].IsEquals("Trinity"));
		TRNT_TEST_EXPECT_TRUE(Res[3].IsEquals("Engine"));

		///////

		S = "hello?world?friend <of,mine,!";
		Res.Clear();
		TRNT_TEST_EXPECT_TRUE(S.SplitBySeparators(Res, ",<?"));
		TRNT_TEST_EXPECT_TRUE(Res[0].IsEquals("hello"));
		TRNT_TEST_EXPECT_TRUE(Res[1].IsEquals("world"));
		TRNT_TEST_EXPECT_TRUE(Res[2].IsEquals("friend "));
		TRNT_TEST_EXPECT_TRUE(Res[3].IsEquals("of"));
		TRNT_TEST_EXPECT_TRUE(Res[4].IsEquals("mine"));
		TRNT_TEST_EXPECT_TRUE(Res[5].IsEquals("!"));
	}

	{
		TString s("  abcdef       ");

		s.TrimStartInternal();
		TRNT_TEST_EXPECT_TRUE(Check(s, "abcdef       ", 13));

		s.TrimEndInternal();
		TRNT_TEST_EXPECT_TRUE(Check(s, "abcdef", 6));

		TString s1("  abcdef       ");
		s1.TrimStartAndEndInternal();
		TRNT_TEST_EXPECT_TRUE(Check(s1, "abcdef", 6));
	}
}