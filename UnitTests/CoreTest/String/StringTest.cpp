#include "StringTest.h"

#include <Trinity/Core/String/String.h>

static TRNT_FORCE_INLINE TBool Check(const TStringBase<TRNT_CHAR_TYPE>& Str, const TRNT_CHAR_TYPE* ExpectedStr, TStringBase<TRNT_CHAR_TYPE>::SizeType ExpectedStrLen)
{
	return Str == ExpectedStr && Str.GetElementCount() == ExpectedStrLen && Str.Capacity() >= ExpectedStrLen;
}

#if defined(TRNT_TEST_CHAR_TYPE)
TRNT_IMPL_TEST_CASE(Strings, TString)
#elif defined(TRNT_TEST_WCHAR_TYPE)
TRNT_IMPL_TEST_CASE(Strings, TWString)
#endif
{
	using String = TStringBase<TRNT_CHAR_TYPE>;
	// Constructor
	{
		String Empty;
		TRNT_TEST_EXPECT_TRUE(Check(Empty, TRNT_TEXT(""), 0));

		String S1(TRNT_TEXT("HELLO WORLD"));
		TRNT_TEST_EXPECT_TRUE(Check(S1, TRNT_TEXT("HELLO WORLD"), 11));

		String S2(S1);
		TRNT_TEST_EXPECT_TRUE(Check(S2, TRNT_TEXT("HELLO WORLD"), 11));

		String S3(5, TRNT_TEXT("HELLO WORLD"));
		TRNT_TEST_EXPECT_TRUE(Check(S3, TRNT_TEXT("HELLO"), 5));
		// Assertion failed: String S3(-5, TRNT_TEXT("HELLO WORLD"));

		String S4(String(TRNT_TEXT("FOO BAR")));
		TRNT_TEST_EXPECT_TRUE(Check(S4, TRNT_TEXT("FOO BAR"), 7));

		String S5(Move(S4));
		TRNT_TEST_EXPECT_TRUE(Check(S5, TRNT_TEXT("FOO BAR"), 7));
		TRNT_TEST_EXPECT_TRUE(Check(S4, TRNT_TEXT(""), 0));

		String S6(9, TRNT_TEXT('a'));
		TRNT_TEST_EXPECT_TRUE(Check(S6, TRNT_TEXT("aaaaaaaaa"), 9));
		// Assertion failed: String S6(-9, TRNT_TEXT('a'));

		S6 = TRNT_TEXT("aaabbbcccddd");
		String S7(S6, 3, 9);
		// Assertion failed: String S7(Str7, -3, -9);
		TRNT_ASSERT(Check(S7, TRNT_TEXT("bbbcccddd"), 9));
	}

	// Assignment
	{
		String Str;

		Str = TRNT_TEXT("aaabbbccc");
		TRNT_TEST_EXPECT_TRUE(Check(Str, TRNT_TEXT("aaabbbccc"), 9));

		Str = TRNT_TEXT("aaa");
		TRNT_TEST_EXPECT_TRUE(Check(Str, TRNT_TEXT("aaa"), 3));

		Str = TRNT_TEXT("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
		TRNT_TEST_EXPECT_TRUE(Check(Str, TRNT_TEXT("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"), 52));

		String Tmp{ TRNT_TEXT("foo foo bar bar") };
		Str = Tmp;
		TRNT_TEST_EXPECT_TRUE(Check(Str, TRNT_TEXT("foo foo bar bar"), 15));

		Tmp = TRNT_TEXT("1234567890");
		Str = Move(Tmp);

		TRNT_TEST_EXPECT_TRUE(Check(Str, TRNT_TEXT("1234567890"), 10));
		TRNT_TEST_EXPECT_TRUE(Check(Tmp, TRNT_TEXT(""), 0));
	}

	// Comparision
	{
		String A(TRNT_TEXT("aaabbbcccddd"));
		String B;

		TRNT_TEST_EXPECT_TRUE(A.IsEquals(TRNT_TEXT("aaabbbcccddd")));
		TRNT_TEST_EXPECT_TRUE(A.IsEquals(TRNT_TEXT("aaABBbCCCDdd"), TStringSearchCase::EIgnoreCase));

		B = TRNT_TEXT("AAAbbbCCCddd");
		TRNT_TEST_EXPECT_TRUE(!A.IsEquals(B));
		TRNT_TEST_EXPECT_TRUE(A.IsEquals(B, TStringSearchCase::EIgnoreCase));

		//////////////////////////////////////////////////////////////////////
		TRNT_TEST_EXPECT_TRUE(A == TRNT_TEXT("aaabbbcccddd"));
		TRNT_TEST_EXPECT_TRUE(A != B);
		TRNT_TEST_EXPECT_TRUE(TRNT_TEXT("AAAbbbCCCddd") == B);
		TRNT_TEST_EXPECT_TRUE(TRNT_TEXT("aaabbb") < A);
		TRNT_TEST_EXPECT_TRUE(TRNT_TEXT("zzzzzzzzzzzzzzzzzzzz") >= A);
		TRNT_TEST_EXPECT_TRUE(TRNT_TEXT("z") > A);
		TRNT_TEST_EXPECT_TRUE(TRNT_TEXT("aaaa") <= A);
		TRNT_TEST_EXPECT_TRUE(A >= TRNT_TEXT("aaabbbcccddd"));
		TRNT_TEST_EXPECT_TRUE(A < TRNT_TEXT("aaabbbccceeeeeee"));
		TRNT_TEST_EXPECT_TRUE(B <= TRNT_TEXT("AAAbbbCCCdddEEE"));
		TRNT_TEST_EXPECT_TRUE(B < TRNT_TEXT("AAAbbbCCCdddEEE"));
		TRNT_TEST_EXPECT_TRUE(A != TRNT_TEXT("aaa"));
		//////////////////////////////////////////////////////////////////////
		// A = TRNT_TEXT("aaabbbcccddd")
		// B = TRNT_TEXT("AAAbbbCCCddd")
		TRNT_TEST_EXPECT_TRUE(A.Compare(TRNT_TEXT("aaabbbccc")) > 0);
		TRNT_TEST_EXPECT_TRUE(A.Compare(TRNT_TEXT("AAABBBCCC"), TStringSearchCase::EIgnoreCase) > 0);
		TRNT_TEST_EXPECT_TRUE(A.Compare(B, TStringSearchCase::EIgnoreCase) == 0);
		A = TRNT_TEXT("bbbaaacccddd");
		B = TRNT_TEXT("pppaaaccc");
		TRNT_TEST_EXPECT_TRUE(A.Compare(3, 6, B, 3, 6) == 0);
		TRNT_TEST_EXPECT_TRUE(A.Compare(0, 9, B, 0, 9) < 0);

		B = TRNT_TEXT("pppAAaCCC");
		TRNT_TEST_EXPECT_TRUE(A.Compare(3, 6, B, 3, 6, TStringSearchCase::EIgnoreCase) == 0);
	}

	// Concatenate
	{
		String Str;
		String Tmp1(TRNT_TEXT("1111"));
		String Tmp2(TRNT_TEXT("2222"));
		String Tmp3(TRNT_TEXT("3333"));

		Str = Tmp1 + Tmp2;
		TRNT_TEST_EXPECT_TRUE(Check(Str, TRNT_TEXT("11112222"), 8));

		Str = Tmp1 + TRNT_TEXT("22223333");
		TRNT_TEST_EXPECT_TRUE(Check(Str, TRNT_TEXT("111122223333"), 12));

		Str = Tmp3 + String(TRNT_TEXT("4444"));
		TRNT_TEST_EXPECT_TRUE(Check(Str, TRNT_TEXT("33334444"), 8));

		Str = TRNT_TEXT("1111") + Tmp2;
		TRNT_TEST_EXPECT_TRUE(Check(Str, TRNT_TEXT("11112222"), 8));

		Str = TRNT_TEXT("aaaa") + String(TRNT_TEXT("bbbb"));
		TRNT_TEST_EXPECT_TRUE(Check(Str, TRNT_TEXT("aaaabbbb"), 8));

		Str = String(TRNT_TEXT("Hello ")) + String(TRNT_TEXT("world"));
		TRNT_TEST_EXPECT_TRUE(Check(Str, TRNT_TEXT("Hello world"), 11));

		Str = String(TRNT_TEXT("AAA")) + TRNT_TEXT("BBB");
		TRNT_TEST_EXPECT_TRUE(Check(Str, TRNT_TEXT("AAABBB"), 6));

		Str = String(TRNT_TEXT("AAA")) + Tmp1;
		TRNT_TEST_EXPECT_TRUE(Check(Str, TRNT_TEXT("AAA1111"), 7));
	}

	{
		String S = TRNT_TEXT("aaabbbcccddd");
		String Pre = TRNT_TEXT("aaa");
		String Suf = TRNT_TEXT("ddd");
		TRNT_TEST_EXPECT_TRUE(S.StartsWith(TRNT_TEXT("aaacc"), 3));
		TRNT_TEST_EXPECT_TRUE(S.StartsWith(TRNT_TEXT("AAA"), TStringSearchCase::EIgnoreCase));
		TRNT_TEST_EXPECT_TRUE(S.StartsWith(TRNT_TEXT("AaAdddcc"), 3, TStringSearchCase::EIgnoreCase));
		TRNT_TEST_EXPECT_TRUE(S.StartsWith(Pre));
		TRNT_TEST_EXPECT_TRUE(S.StartsWith(TRNT_TEXT("")));
		TRNT_TEST_EXPECT_FALSE(S.StartsWith(TRNT_TEXT("foo")));
		TRNT_TEST_EXPECT_FALSE(S.StartsWith(TRNT_TEXT("111222")));

		TRNT_TEST_EXPECT_TRUE(S.EndsWith(Suf));
		TRNT_TEST_EXPECT_TRUE(S.EndsWith(TRNT_TEXT("cccddd")));
		TRNT_TEST_EXPECT_TRUE(S.EndsWith(TRNT_TEXT("CCcDDdeee"), 6, TStringSearchCase::EIgnoreCase));
		TRNT_TEST_EXPECT_TRUE(S.EndsWith(TRNT_TEXT("")));
		TRNT_TEST_EXPECT_TRUE(S.EndsWith(TRNT_TEXT("cCddD"), TStringSearchCase::EIgnoreCase));
		TRNT_TEST_EXPECT_FALSE(S.EndsWith(TRNT_TEXT("1")));
		TRNT_TEST_EXPECT_FALSE(S.EndsWith(TRNT_TEXT("eee")));
	}

	// Find
	{
		String Str(TRNT_TEXT("aaaa bbbb cccc dddd aaaa bbbb"));
		String Substr(TRNT_TEXT("aaaa"));

		TRNT_TEST_EXPECT_TRUE(Str.Contains(Substr));
		TRNT_TEST_EXPECT_TRUE(Str.Contains(TRNT_TEXT("AaAA bBBB"), TStringSearchCase::EIgnoreCase));

		TRNT_TEST_EXPECT_TRUE(Str.FindChar(TRNT_TEXT('b')) == 5);
		TRNT_TEST_EXPECT_TRUE(Str.FindLastChar(TRNT_TEXT('b')) == Str.GetElementCount() - 1);

		// CaseSensitive
		TRNT_TEST_EXPECT_TRUE(Str.Find(TRNT_TEXT("cccc")) == 10);
		TRNT_TEST_EXPECT_TRUE(Str.Find(TRNT_TEXT("")) == 0);
		TRNT_TEST_EXPECT_TRUE(Str.Find(TRNT_TEXT(""), 3) == 3);
		TRNT_TEST_EXPECT_TRUE(Str.Find(TRNT_TEXT("cccc"), -10) == 10);
		TRNT_TEST_EXPECT_TRUE(Str.Find(3, TRNT_TEXT("dcccdd")) == String::Npos);
		TRNT_TEST_EXPECT_TRUE(Str.Find(TRNT_TEXT("aaaa"), 10) == 20);
		TRNT_TEST_EXPECT_TRUE(Str.Find(Substr) == 0);

		TRNT_TEST_EXPECT_TRUE(Str.FindLast(TRNT_TEXT("aaaa")) == 20);
		TRNT_TEST_EXPECT_TRUE(Str.FindLast(TRNT_TEXT("aaaa"), 10) == 0);
		TRNT_TEST_EXPECT_TRUE(Str.FindLast(Substr, 1000) == 20);
		TRNT_TEST_EXPECT_TRUE(Str.FindLast(Substr, 15) == 0);
		TRNT_TEST_EXPECT_TRUE(Str.FindLast(TRNT_TEXT(""), 10) == 10);
		TRNT_TEST_EXPECT_TRUE(Str.FindLast(TRNT_TEXT("")) == Str.GetElementCount());
		TRNT_TEST_EXPECT_TRUE(Str.FindLast(3, TRNT_TEXT("bbbbccc"), 15) == 6);

		// IgnoreCase
		TStringSearchCase SearchCase = TStringSearchCase::EIgnoreCase;
		TRNT_TEST_EXPECT_TRUE(Str.Find(TRNT_TEXT("CCCc"), 0, SearchCase) == 10);
		TRNT_TEST_EXPECT_TRUE(Str.Find(TRNT_TEXT(""), 0, SearchCase) == 0);
		TRNT_TEST_EXPECT_TRUE(Str.Find(TRNT_TEXT(""), 3, SearchCase) == 3);
		TRNT_TEST_EXPECT_TRUE(Str.Find(TRNT_TEXT("cCCc"), -10, SearchCase) == 10);
		TRNT_TEST_EXPECT_TRUE(Str.Find(3, TRNT_TEXT("DcCcdd"), 0, SearchCase) == String::Npos);
		TRNT_TEST_EXPECT_TRUE(Str.Find(TRNT_TEXT("AAaA"), 10, SearchCase) == 20);

		TRNT_TEST_EXPECT_TRUE(Str.FindLast(TRNT_TEXT("AaaA"), -1, SearchCase) == 20);
		TRNT_TEST_EXPECT_TRUE(Str.FindLast(TRNT_TEXT("Aaaa"), 10, SearchCase) == 0);
		TRNT_TEST_EXPECT_TRUE(Str.FindLast(Substr, 1000, SearchCase) == 20);
		TRNT_TEST_EXPECT_TRUE(Str.FindLast(Substr, 15, SearchCase) == 0);
		TRNT_TEST_EXPECT_TRUE(Str.FindLast(TRNT_TEXT(""), 10, SearchCase) == 10);
		TRNT_TEST_EXPECT_TRUE(Str.FindLast(TRNT_TEXT(""), -1, SearchCase) == Str.GetElementCount());
		TRNT_TEST_EXPECT_TRUE(Str.FindLast(3, TRNT_TEXT("BBbbCCc"), 15, SearchCase) == 6);

		String Str1{ TRNT_TEXT("AB AB AB") };
		TRNT_TEST_EXPECT_TRUE(Str1.FindLastChar(TRNT_TEXT('B'), 4) == 4);
		TRNT_TEST_EXPECT_TRUE(Str1.FindLastChar(TRNT_TEXT('A')) == 6);

		String Str2{ TRNT_TEXT("ABCD") };
		TRNT_TEST_EXPECT_TRUE(Str2.FindLastChar(TRNT_TEXT('a'), -1, TStringSearchCase::EIgnoreCase) == 0);
	}

	{
		String Str(TRNT_TEXT("AAaabBBbcCCc"));

		Str.ToUpperInternal();
		TRNT_TEST_EXPECT_TRUE(Check(Str, TRNT_TEXT("AAAABBBBCCCC"), 12));

		Str.ToLowerInternal();
		TRNT_TEST_EXPECT_TRUE(Check(Str, TRNT_TEXT("aaaabbbbcccc"), 12));

		String Rev = Str.Reverse();
		TRNT_TEST_EXPECT_TRUE(Check(Rev, TRNT_TEXT("ccccbbbbaaaa"), 12));

		Rev.ReverseInternal();
		TRNT_TEST_EXPECT_TRUE(Check(Rev, TRNT_TEXT("aaaabbbbcccc"), 12));

		Str.Resize(20, 'd');
		TRNT_TEST_EXPECT_TRUE(Check(Str, TRNT_TEXT("aaaabbbbccccdddddddd"), 20));

		Str.Resize(8);
		TRNT_TEST_EXPECT_TRUE(Check(Str, TRNT_TEXT("aaaabbbb"), 8));

		Str.Clear();
		TRNT_TEST_EXPECT_TRUE(Check(Str, TRNT_TEXT(""), 0));

		Str = TRNT_TEXT("aaaaaaa");
		String UpString = Str.ToUpper();
		TRNT_TEST_EXPECT_TRUE(Check(UpString, TRNT_TEXT("AAAAAAA"), 7));

		String LowerString = Str.ToLower();
		TRNT_TEST_EXPECT_TRUE(Check(LowerString, TRNT_TEXT("aaaaaaa"), 7));
	}

	{
		String Str(TRNT_TEXT("aaaabbbbcccc"));

		TRNT_TEST_EXPECT_TRUE(Str.GetElementCount() == 12);
		TRNT_TEST_EXPECT_TRUE(Str.Capacity() == 12);

		TRNT_TEST_EXPECT_TRUE(Str.RemainingCapacity() == 0);

		for (String::IteratorType it = Str.begin(); it != Str.end(); ++it)
		{
			*it = TCharUtils<String::ElementType>::ToUpperCase(*it);
		}
		TRNT_TEST_EXPECT_TRUE(Str == TRNT_TEXT("AAAABBBBCCCC"));

		TRNT_TEST_EXPECT_TRUE(Str.CharAt(3) == TRNT_TEXT('A'));
		TRNT_TEST_EXPECT_TRUE(Str[6] == TRNT_TEXT('B'));

		TRNT_TEST_EXPECT_TRUE(Str.First() == TRNT_TEXT('A'));
		TRNT_TEST_EXPECT_TRUE(Str.Last() == TRNT_TEXT('C'));

		TRNT_TEST_EXPECT_TRUE(TCString<String::ElementType>::Strcmp(Str.GetData(), TRNT_TEXT("AAAABBBBCCCC")) == 0);

		TRNT_TEST_EXPECT_TRUE(Str.IsEmpty() == false);

		Str.Reserve(20);
		TRNT_TEST_EXPECT_TRUE(Str.RemainingCapacity() == 8);
		TRNT_TEST_EXPECT_TRUE(Str.Capacity() == 20);

		Str.Reserve(1);
		TRNT_TEST_EXPECT_TRUE(Str.RemainingCapacity() == 8);
		TRNT_TEST_EXPECT_TRUE(Str.Capacity() == 20);

		String res = Str.Substring(4);
		// Assertion failed: String res = Str.Substring(-4);
		TRNT_TEST_EXPECT_TRUE(res == TRNT_TEXT("BBBBCCCC"));

		res = Str.Substring(4, 4);
		// Assertion failed: res = Str.Substring(-4, -4);
		TRNT_TEST_EXPECT_TRUE(res == TRNT_TEXT("BBBB"));
	}

	{
		String S;
		String Tmp;
		S.Append(TRNT_TEXT("aaa"));
		S.Append(3, TRNT_TEXT('b'));
		// failed: S.Append(-3, TRNT_TEXT('b'));
		S.Append(TRNT_TEXT("cccbbb"), 3);
		// failed: S.Append(TRNT_TEXT("cccbbb"), -3);
		S.Append(Tmp);
		Tmp = TRNT_TEXT("aaabbbcccddd");
		S.Append(Tmp, 9, 3);
		// failed: S.Append(Tmp, -9, -3);

		TRNT_TEST_EXPECT_TRUE(Check(S, TRNT_TEXT("aaabbbcccddd"), 12));

		S = TRNT_TEXT("");
		S += TRNT_TEXT("111");
		S += TRNT_TEXT('2');
		Tmp = TRNT_TEXT("22333");
		S += Tmp;
		S += TRNT_TEXT("");
		TRNT_TEST_EXPECT_TRUE(Check(S, TRNT_TEXT("111222333"), 9));
	}

	{
		// 33333344400011133334444222
		String S;

		S.Insert(0, TRNT_TEXT("000"));
		S.Insert(3, TRNT_TEXT("111222"), 3);
		S.Insert(6, 3, TRNT_TEXT('2'));

		String Tmp{ TRNT_TEXT("333444") };

		S.Insert(0, Tmp);
		S.Insert(0, Tmp, 0, 3);

		TRNT_TEST_EXPECT_TRUE(Check(S, TRNT_TEXT("333333444000111222"), 18));

		S.Insert(S.GetElementCount() - 3, TRNT_TEXT("33334444"));

		TRNT_TEST_EXPECT_TRUE(Check(S, TRNT_TEXT("33333344400011133334444222"), 26));
	}

	{
		String S(TRNT_TEXT("hello hello world"));
		S.RemoveAt(0, 6);
		// failed: S.RemoveAt(0, -6);

		TRNT_TEST_EXPECT_TRUE(Check(S, TRNT_TEXT("hello world"), 11));
	}

	// split
	{
		String S(TRNT_TEXT("Welcome to Trinity Engine"));

		TDynamicArray<String> Res;
		TRNT_TEST_EXPECT_TRUE(S.SplitByString(Res, TRNT_TEXT(" ")));

		TRNT_TEST_EXPECT_TRUE(Res[0].IsEquals(TRNT_TEXT("Welcome")));
		TRNT_TEST_EXPECT_TRUE(Res[1].IsEquals(TRNT_TEXT("to")));
		TRNT_TEST_EXPECT_TRUE(Res[2].IsEquals(TRNT_TEXT("Trinity")));
		TRNT_TEST_EXPECT_TRUE(Res[3].IsEquals(TRNT_TEXT("Engine")));

		///////

		S = TRNT_TEXT("hello?world?friend <of,mine,!");
		Res.Clear();
		TRNT_TEST_EXPECT_TRUE(S.SplitBySeparators(Res, TRNT_TEXT(",<?")));

		TRNT_TEST_EXPECT_TRUE(Res[0].IsEquals(TRNT_TEXT("hello")));
		TRNT_TEST_EXPECT_TRUE(Res[1].IsEquals(TRNT_TEXT("world")));
		TRNT_TEST_EXPECT_TRUE(Res[2].IsEquals(TRNT_TEXT("friend ")));
		TRNT_TEST_EXPECT_TRUE(Res[3].IsEquals(TRNT_TEXT("of")));
		TRNT_TEST_EXPECT_TRUE(Res[4].IsEquals(TRNT_TEXT("mine")));
		TRNT_TEST_EXPECT_TRUE(Res[5].IsEquals(TRNT_TEXT("!")));
	}

	{
		String S(TRNT_TEXT(" one  two three four"));

		TDynamicArray<String> Res;
		TRNT_TEST_EXPECT_TRUE(S.SplitByString(Res, TRNT_TEXT(" ")));
		TRNT_TEST_EXPECT_TRUE(Res[0].IsEquals(TRNT_TEXT("one")));
		TRNT_TEST_EXPECT_TRUE(Res[1].IsEquals(TRNT_TEXT("two")));
		TRNT_TEST_EXPECT_TRUE(Res[2].IsEquals(TRNT_TEXT("three")));
		TRNT_TEST_EXPECT_TRUE(Res[3].IsEquals(TRNT_TEXT("four")));
	}
	{
		String s(TRNT_TEXT("  abcdef       "));

		s.TrimStartInternal();
		TRNT_TEST_EXPECT_TRUE(Check(s, TRNT_TEXT("abcdef       "), 13));

		s.TrimEndInternal();
		TRNT_TEST_EXPECT_TRUE(Check(s, TRNT_TEXT("abcdef"), 6));

		String s1(TRNT_TEXT("  abcdef       "));
		s1.TrimStartAndEndInternal();
		TRNT_TEST_EXPECT_TRUE(Check(s1, TRNT_TEXT("abcdef"), 6));
	}

	{
		String Path = TRNT_TEXT("");
		Path /= TRNT_TEXT("dir1");
		Path /= TRNT_TEXT("/dir2");
		Path /= TRNT_TEXT("dir3/");
		Path /= TRNT_TEXT("/dir4/");

		TRNT_TEST_EXPECT_TRUE(Check(Path, TRNT_TEXT("dir1/dir2/dir3/dir4/"), 20));

		Path /= TRNT_TEXT("");
		TRNT_TEST_EXPECT_TRUE(Check(Path, TRNT_TEXT("dir1/dir2/dir3/dir4/"), 20));

		String Dir1 = TRNT_TEXT("C:/dir1");
		String Dir2 = TRNT_TEXT("dir2/");

		Path = Dir1 / Dir2;

		TRNT_TEST_EXPECT_TRUE(Check(Path, TRNT_TEXT("C:/dir1/dir2/"), 13));

		Path = Dir1 / TRNT_TEXT("other_dir_1/other_dir_2");
		TRNT_TEST_EXPECT_TRUE(Check(Path, TRNT_TEXT("C:/dir1/other_dir_1/other_dir_2"), 31));
	}
}