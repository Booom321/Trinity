#include "CStringTest.h"

#include <Trinity/Core/String/CString.h>

template<typename T>
TInt64 GetLength(const T* Begin, const T* End)
{
	if (!Begin || !End)
	{
		return -1;
	}

	return static_cast<TInt64>(End - Begin);
}

#if defined(TRNT_TEST_CHAR_TYPE)
TRNT_IMPL_TEST_CASE(Strings, TCString_Char)
#elif defined(TRNT_TEST_WCHAR_TYPE)
TRNT_IMPL_TEST_CASE(Strings, TCString_WChar)
#endif
{
	using CString = TCString<TRNT_CHAR_TYPE>;

	static_assert(TAreTheSameType<CString::CharType, TRNT_CHAR_TYPE>::Value);

	const TRNT_CHAR_TYPE* Str = TRNT_TEXT("aaaabbbbccccdddd11112222");

	TRNT_TEST_EXPECT_TRUE(GetLength<TRNT_CHAR_TYPE>(Str, CString::Strchr(Str, TRNT_TEXT('a'))) == 0);
	TRNT_TEST_EXPECT_TRUE(GetLength<TRNT_CHAR_TYPE>(Str, CString::Strchr(Str, TRNT_TEXT('$'))) == -1);

	TRNT_TEST_EXPECT_TRUE(GetLength<TRNT_CHAR_TYPE>(Str, CString::Strrchr(Str, TRNT_TEXT('a'))) == 3);
	TRNT_TEST_EXPECT_TRUE(GetLength<TRNT_CHAR_TYPE>(Str, CString::Strrchr(Str, TRNT_TEXT('%'))) == -1);

	TRNT_TEST_EXPECT_TRUE(CString::Strcmp(TRNT_TEXT("hello world"), TRNT_TEXT("hello world")) == 0);
	TRNT_TEST_EXPECT_TRUE(CString::Strcmp(Str, TRNT_TEXT("hello world")) != 0);
	TRNT_TEST_EXPECT_TRUE(CString::Strcmp(TRNT_TEXT("aaa"), TRNT_TEXT("aab")) < 0);
	TRNT_TEST_EXPECT_TRUE(CString::Strcmp(TRNT_TEXT("aad"), TRNT_TEXT("aab")) > 0);
	TRNT_TEST_EXPECT_TRUE(CString::Strcmp(TRNT_TEXT("aaaa"), TRNT_TEXT("")) > 0);

	TRNT_TEST_EXPECT_TRUE(CString::Strlen(Str) == 24);
	TRNT_TEST_EXPECT_TRUE(CString::Strlen(TRNT_TEXT("")) == 0);

	TRNT_TEST_EXPECT_TRUE(CString::Stricmp(TRNT_TEXT("AAAbbbCCcDDD"), TRNT_TEXT("aaabbbcccddd")) == 0);
	TRNT_TEST_EXPECT_TRUE(CString::Stricmp(TRNT_TEXT("AAAbbbCCcDDD"), TRNT_TEXT("aaabbbcccdde")) < 0);
	TRNT_TEST_EXPECT_TRUE(CString::Stricmp(TRNT_TEXT("AAAbbbCCcDDu"), TRNT_TEXT("aaabbbcccdde")) > 0);
	TRNT_TEST_EXPECT_TRUE(CString::Stricmp(TRNT_TEXT("AAAbbbCCcDDu"), TRNT_TEXT("")) > 0);

	TRNT_TEST_EXPECT_TRUE(CString::Strncmp(TRNT_TEXT("aaaabbbbccccdddd"), Str, 16) == 0);
	TRNT_TEST_EXPECT_TRUE(CString::Strncmp(TRNT_TEXT("aaabbbcccddd"), TRNT_TEXT("aaabbbccd"), 9) < 0);
	TRNT_TEST_EXPECT_TRUE(CString::Strncmp(TRNT_TEXT("aaabbbcceddd"), TRNT_TEXT("aaabbbccd"), 9) > 0);
	TRNT_TEST_EXPECT_TRUE(CString::Strncmp(TRNT_TEXT("aaabbbcceddd"), TRNT_TEXT(""), 9) > 0);

	TRNT_TEST_EXPECT_TRUE(CString::Strnicmp(TRNT_TEXT("aaabbbcccddd"), TRNT_TEXT("AAAbbbCCcDDd"), 12) == 0);
	TRNT_TEST_EXPECT_TRUE(CString::Strnicmp(TRNT_TEXT("hello"), TRNT_TEXT(""), 5) > 0);
	TRNT_TEST_EXPECT_TRUE(CString::Strnicmp(TRNT_TEXT("aaaBBbCCe"), TRNT_TEXT("aaAbbBccD"), 9) > 0);
	TRNT_TEST_EXPECT_TRUE(CString::Strnicmp(TRNT_TEXT("aaaBBbCCe"), TRNT_TEXT("aaAbbBccD"), 0) == 0);
	TRNT_TEST_EXPECT_TRUE(CString::Strnicmp(TRNT_TEXT("aaaBBbCCe"), TRNT_TEXT(""), 0) == 0);

	const TRNT_CHAR_TYPE* x = TRNT_TEXT("aaa bbb ccc ddd");
	const TRNT_CHAR_TYPE* y = TRNT_TEXT("aaa ccc bbb ccc");

	TRNT_TEST_EXPECT_TRUE(CString::StrcmpRanges(x, x + 3, y, y + 3) == 0);
	TRNT_TEST_EXPECT_TRUE(CString::StrcmpRanges(x, x + 10, y, y + 3) > 0);
	TRNT_TEST_EXPECT_TRUE(CString::StrcmpRanges(x + 4, x + 7, y + 4, y + 7) < 0);
	TRNT_TEST_EXPECT_TRUE(CString::StrcmpRanges(x, x, y, y) == 0);

	x = TRNT_TEXT("aAa BbB cCC DDd");
	y = TRNT_TEXT("AAa BBB cCc Ddd");

	TRNT_TEST_EXPECT_TRUE(CString::StricmpRanges(x, x, y, y) == 0);
	TRNT_TEST_EXPECT_TRUE(CString::StricmpRanges(x, x + 3, y, y + 3) == 0);
	TRNT_TEST_EXPECT_TRUE(CString::StricmpRanges(x, x + 3, y + 4, y + 7) < 0);
	TRNT_TEST_EXPECT_TRUE(CString::StricmpRanges(x, x + 3, y, y) > 0);

	x = TRNT_TEXT("hello world hello world");
	y = TRNT_TEXT("llo");

	TRNT_TEST_EXPECT_TRUE(GetLength<TRNT_CHAR_TYPE>(x, CString::Strstr(x, y)) == 2);
	TRNT_TEST_EXPECT_TRUE(GetLength<TRNT_CHAR_TYPE>(x, CString::Strstr(x, TRNT_TEXT("w"))) == 6);
	TRNT_TEST_EXPECT_TRUE(GetLength<TRNT_CHAR_TYPE>(x, CString::Strstr(x, TRNT_TEXT(""))) == 0);
	TRNT_TEST_EXPECT_TRUE(GetLength<TRNT_CHAR_TYPE>(x, CString::Strstr(x, TRNT_TEXT("foo"))) == -1);

	TRNT_TEST_EXPECT_TRUE(GetLength<TRNT_CHAR_TYPE>(x, CString::Strrstr(x, y)) == 14);
	TRNT_TEST_EXPECT_TRUE(GetLength<TRNT_CHAR_TYPE>(x, CString::Strrstr(x, TRNT_TEXT("l"))) == 21);
	TRNT_TEST_EXPECT_TRUE(GetLength<TRNT_CHAR_TYPE>(x, CString::Strrstr(x, TRNT_TEXT(""))) == 23);
	TRNT_TEST_EXPECT_TRUE(GetLength<TRNT_CHAR_TYPE>(x, CString::Strrstr(x, TRNT_TEXT("bar"))) == -1);

	x = TRNT_TEXT("HeLLo WORlD Hello WORLD");
	y = TRNT_TEXT("eLLo");

	TRNT_TEST_EXPECT_TRUE(GetLength<TRNT_CHAR_TYPE>(x, CString::Stristr(x, y)) == 1);
	TRNT_TEST_EXPECT_TRUE(GetLength<TRNT_CHAR_TYPE>(x, CString::Stristr(x, TRNT_TEXT("w"))) == 6);
	TRNT_TEST_EXPECT_TRUE(GetLength<TRNT_CHAR_TYPE>(x, CString::Stristr(x, TRNT_TEXT(""))) == 0);
	TRNT_TEST_EXPECT_TRUE(GetLength<TRNT_CHAR_TYPE>(x, CString::Stristr(x, TRNT_TEXT("HELLOWORLD"))) == -1);

	TRNT_TEST_EXPECT_TRUE(GetLength<TRNT_CHAR_TYPE>(x, CString::Strristr(x, y)) == 13);
	TRNT_TEST_EXPECT_TRUE(GetLength<TRNT_CHAR_TYPE>(x, CString::Strristr(x, TRNT_TEXT("orld"))) == 19);
	TRNT_TEST_EXPECT_TRUE(GetLength<TRNT_CHAR_TYPE>(x, CString::Strristr(x, TRNT_TEXT(""))) == 23);
	TRNT_TEST_EXPECT_TRUE(GetLength<TRNT_CHAR_TYPE>(x, CString::Strristr(x, TRNT_TEXT("_WORLD_"))) == -1);

	TRNT_CHAR_TYPE Arr[6]{ TRNT_TEXT('h'), TRNT_TEXT('e'), TRNT_TEXT('l'), TRNT_TEXT('l'), TRNT_TEXT('o'), '\0' };

	TRNT_TEST_EXPECT_TRUE(CString::Strcmp(CString::Strlwr(Arr), TRNT_TEXT("hello")) == 0);
	TRNT_TEST_EXPECT_TRUE(CString::Strcmp(CString::Strupr(Arr), TRNT_TEXT("HELLO")) == 0);
	TRNT_TEST_EXPECT_TRUE(CString::Strcmp(CString::Strlwr(Arr), TRNT_TEXT("hello")) == 0);

	Str = TRNT_TEXT("hello world, friend of mine!");

	const TRNT_CHAR_TYPE* Sep = TRNT_TEXT(" ,!");
	const TRNT_CHAR_TYPE* Tmp = Str;
	TRNT_TEST_EXPECT_TRUE(GetLength<TRNT_CHAR_TYPE>(Str, Tmp = CString::Strpbrk(Tmp, Sep)) == 5);
	TRNT_TEST_EXPECT_TRUE(GetLength<TRNT_CHAR_TYPE>(Str, Tmp = CString::Strpbrk(++Tmp, Sep)) == 11);
	TRNT_TEST_EXPECT_TRUE(GetLength<TRNT_CHAR_TYPE>(Str, Tmp = CString::Strpbrk(++Tmp, Sep)) == 12);
	TRNT_TEST_EXPECT_TRUE(GetLength<TRNT_CHAR_TYPE>(Str, Tmp = CString::Strpbrk(++Tmp, Sep)) == 19);
	TRNT_TEST_EXPECT_TRUE(GetLength<TRNT_CHAR_TYPE>(Str, Tmp = CString::Strpbrk(++Tmp, Sep)) == 22);
	TRNT_TEST_EXPECT_TRUE(GetLength<TRNT_CHAR_TYPE>(Str, Tmp = CString::Strpbrk(++Tmp, Sep)) == 27);
	TRNT_TEST_EXPECT_TRUE(GetLength<TRNT_CHAR_TYPE>(Str, Tmp = CString::Strpbrk(++Tmp, Sep)) == -1);
}