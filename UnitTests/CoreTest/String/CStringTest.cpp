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

TRNT_IMPL_TEST_CASE(Strings, TCStringForChar)
{
	using CString = TCString<TChar>;

	static_assert(TAreTheSameType<CString::CharType, TChar>::Value);

	const TChar* Str = "aaaabbbbccccdddd11112222";

	TRNT_TEST_EXPECT_TRUE(GetLength<TChar>(Str, CString::Strchr(Str, 'a')) == 0);
	TRNT_TEST_EXPECT_TRUE(GetLength<TChar>(Str, CString::Strchr(Str, '$')) == -1);

	TRNT_TEST_EXPECT_TRUE(GetLength<TChar>(Str, CString::Strrchr(Str, 'a')) == 3);
	TRNT_TEST_EXPECT_TRUE(GetLength<TChar>(Str, CString::Strrchr(Str, '%')) == -1);

	TRNT_TEST_EXPECT_TRUE(CString::Strcmp("hello world", "hello world") == 0);
	TRNT_TEST_EXPECT_TRUE(CString::Strcmp(Str, "hello world") != 0);
	TRNT_TEST_EXPECT_TRUE(CString::Strcmp("aaa", "aab") < 0);
	TRNT_TEST_EXPECT_TRUE(CString::Strcmp("aad", "aab") > 0);
	TRNT_TEST_EXPECT_TRUE(CString::Strcmp("aaaa", "") > 0);

	TRNT_TEST_EXPECT_TRUE(CString::Strlen(Str) == 24);
	TRNT_TEST_EXPECT_TRUE(CString::Strlen("") == 0);

	TRNT_TEST_EXPECT_TRUE(CString::Stricmp("AAAbbbCCcDDD", "aaabbbcccddd") == 0);
	TRNT_TEST_EXPECT_TRUE(CString::Stricmp("AAAbbbCCcDDD", "aaabbbcccdde") < 0);
	TRNT_TEST_EXPECT_TRUE(CString::Stricmp("AAAbbbCCcDDu", "aaabbbcccdde") > 0);
	TRNT_TEST_EXPECT_TRUE(CString::Stricmp("AAAbbbCCcDDu", "") > 0);

	TRNT_TEST_EXPECT_TRUE(CString::Strncmp("aaaabbbbccccdddd", Str, 16) == 0);
	TRNT_TEST_EXPECT_TRUE(CString::Strncmp("aaabbbcccddd", "aaabbbccd", 9) < 0);
	TRNT_TEST_EXPECT_TRUE(CString::Strncmp("aaabbbcceddd", "aaabbbccd", 9) > 0);
	TRNT_TEST_EXPECT_TRUE(CString::Strncmp("aaabbbcceddd", "", 9) > 0);

	TRNT_TEST_EXPECT_TRUE(CString::Strnicmp("aaabbbcccddd", "AAAbbbCCcDDd", 12) == 0);
	TRNT_TEST_EXPECT_TRUE(CString::Strnicmp("hello", "", 5) > 0);
	TRNT_TEST_EXPECT_TRUE(CString::Strnicmp("aaaBBbCCe", "aaAbbBccD", 9) > 0);
	TRNT_TEST_EXPECT_TRUE(CString::Strnicmp("aaaBBbCCe", "aaAbbBccD", 0) == 0);
	TRNT_TEST_EXPECT_TRUE(CString::Strnicmp("aaaBBbCCe", "", 0) == 0);

	const char* x = "aaa bbb ccc ddd";
	const char* y = "aaa ccc bbb ccc";

	TRNT_TEST_EXPECT_TRUE(CString::StrcmpRanges(x, x + 3, y, y + 3) == 0);
	TRNT_TEST_EXPECT_TRUE(CString::StrcmpRanges(x, x + 10, y, y + 3) > 0);
	TRNT_TEST_EXPECT_TRUE(CString::StrcmpRanges(x + 4, x + 7, y + 4, y + 7) < 0);
	TRNT_TEST_EXPECT_TRUE(CString::StrcmpRanges(x, x, y, y) == 0);

	x = "aAa BbB cCC DDd";
	y = "AAa BBB cCc Ddd";

	TRNT_TEST_EXPECT_TRUE(CString::StricmpRanges(x, x, y, y) == 0);
	TRNT_TEST_EXPECT_TRUE(CString::StricmpRanges(x, x + 3, y, y + 3) == 0);
	TRNT_TEST_EXPECT_TRUE(CString::StricmpRanges(x, x + 3, y + 4, y + 7) < 0);
	TRNT_TEST_EXPECT_TRUE(CString::StricmpRanges(x, x + 3, y, y) > 0);

	x = "hello world hello world";
	y = "llo";

	TRNT_TEST_EXPECT_TRUE(GetLength<TChar>(x, CString::Strstr(x, y)) == 2);
	TRNT_TEST_EXPECT_TRUE(GetLength<TChar>(x, CString::Strstr(x, "w")) == 6);
	TRNT_TEST_EXPECT_TRUE(GetLength<TChar>(x, CString::Strstr(x, "")) == 0);
	TRNT_TEST_EXPECT_TRUE(GetLength<TChar>(x, CString::Strstr(x, "foo")) == -1);

	TRNT_TEST_EXPECT_TRUE(GetLength<TChar>(x, CString::Strrstr(x, y)) == 14);
	TRNT_TEST_EXPECT_TRUE(GetLength<TChar>(x, CString::Strrstr(x, "l")) == 21);
	TRNT_TEST_EXPECT_TRUE(GetLength<TChar>(x, CString::Strrstr(x, "")) == 23);
	TRNT_TEST_EXPECT_TRUE(GetLength<TChar>(x, CString::Strrstr(x, "bar")) == -1);

	x = "HeLLo WORlD Hello WORLD";
	y = "eLLo";

	TRNT_TEST_EXPECT_TRUE(GetLength<TChar>(x, CString::Stristr(x, y)) == 1);
	TRNT_TEST_EXPECT_TRUE(GetLength<TChar>(x, CString::Stristr(x, "w")) == 6);
	TRNT_TEST_EXPECT_TRUE(GetLength<TChar>(x, CString::Stristr(x, "")) == 0);
	TRNT_TEST_EXPECT_TRUE(GetLength<TChar>(x, CString::Stristr(x, "HELLOWORLD")) == -1);

	TRNT_TEST_EXPECT_TRUE(GetLength<TChar>(x, CString::Strristr(x, y)) == 13);
	TRNT_TEST_EXPECT_TRUE(GetLength<TChar>(x, CString::Strristr(x, "orld")) == 19);
	TRNT_TEST_EXPECT_TRUE(GetLength<TChar>(x, CString::Strristr(x, "")) == 23);
	TRNT_TEST_EXPECT_TRUE(GetLength<TChar>(x, CString::Strristr(x, "_WORLD_")) == -1);

	char Arr[6]{ 'h', 'e', 'l', 'l', 'o', '\0' };

	TRNT_TEST_EXPECT_TRUE(CString::Strcmp(CString::Strlwr(Arr), "hello") == 0);
	TRNT_TEST_EXPECT_TRUE(CString::Strcmp(CString::Strupr(Arr), "HELLO") == 0);
	TRNT_TEST_EXPECT_TRUE(CString::Strcmp(CString::Strlwr(Arr), "hello") == 0);

	Str = "hello world, friend of mine!";

	const TChar* Sep = " ,!";
	const TChar* Tmp = Str;
	TRNT_TEST_EXPECT_TRUE(GetLength<TChar>(Str, Tmp = CString::Strpbrk(Tmp, Sep)) == 5);
	TRNT_TEST_EXPECT_TRUE(GetLength<TChar>(Str, Tmp = CString::Strpbrk(++Tmp, Sep)) == 11);
	TRNT_TEST_EXPECT_TRUE(GetLength<TChar>(Str, Tmp = CString::Strpbrk(++Tmp, Sep)) == 12);
	TRNT_TEST_EXPECT_TRUE(GetLength<TChar>(Str, Tmp = CString::Strpbrk(++Tmp, Sep)) == 19);
	TRNT_TEST_EXPECT_TRUE(GetLength<TChar>(Str, Tmp = CString::Strpbrk(++Tmp, Sep)) == 22);
	TRNT_TEST_EXPECT_TRUE(GetLength<TChar>(Str, Tmp = CString::Strpbrk(++Tmp, Sep)) == 27);
	TRNT_TEST_EXPECT_TRUE(GetLength<TChar>(Str, Tmp = CString::Strpbrk(++Tmp, Sep)) == -1);
}