#include "CharUtilsTest.h"

#include <Trinity/Core/String/CharUtils.h>

TRNT_IMPL_TEST_CASE(Strings, TCharUtilsForChar)
{
	using CharUtils = TCharUtils<TChar>;

	static_assert(TAreTheSameType<CharUtils::CharType, TChar>::Value);

	TRNT_TEST_EXPECT_TRUE(CharUtils::ToUpperCase('a') == 'A');
	TRNT_TEST_EXPECT_TRUE(CharUtils::ToUpperCase('A') == 'A');

	TRNT_TEST_EXPECT_TRUE(CharUtils::ToLowerCase('a') == 'a');
	TRNT_TEST_EXPECT_TRUE(CharUtils::ToLowerCase('A') == 'a');

	TRNT_TEST_EXPECT_TRUE(CharUtils::IsWhitespace(' '));
	TRNT_TEST_EXPECT_TRUE(!CharUtils::IsWhitespace('$'));

	TRNT_TEST_EXPECT_TRUE(CharUtils::IsAlnum('a'));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsAlnum('0'));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsAlnum('1'));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsAlnum('2'));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsAlnum('3'));
	TRNT_TEST_EXPECT_TRUE(!CharUtils::IsAlnum('$'));
	TRNT_TEST_EXPECT_TRUE(!CharUtils::IsAlnum('%'));
	TRNT_TEST_EXPECT_TRUE(!CharUtils::IsAlnum('#'));

	TRNT_TEST_EXPECT_TRUE(CharUtils::IsAlpha('a'));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsAlpha('A'));
	TRNT_TEST_EXPECT_TRUE(!CharUtils::IsAlpha('#'));
	TRNT_TEST_EXPECT_TRUE(!CharUtils::IsAlpha('1'));

	TRNT_TEST_EXPECT_TRUE(CharUtils::IsDigit('1'));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsDigit('2'));
	TRNT_TEST_EXPECT_TRUE(!CharUtils::IsDigit('E'));
	TRNT_TEST_EXPECT_TRUE(!CharUtils::IsDigit('$'));

	TRNT_TEST_EXPECT_TRUE(CharUtils::IsUnderscore('_'));
	TRNT_TEST_EXPECT_TRUE(!CharUtils::IsUnderscore('-'));
	TRNT_TEST_EXPECT_TRUE(!CharUtils::IsUnderscore('a'));

	TRNT_TEST_EXPECT_TRUE(CharUtils::IsLower('a'));
	TRNT_TEST_EXPECT_TRUE(!CharUtils::IsLower('A'));

	TRNT_TEST_EXPECT_TRUE(CharUtils::IsUpper('A'));
	TRNT_TEST_EXPECT_TRUE(!CharUtils::IsUpper('a'));

	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit('0'));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit('1'));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit('2'));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit('3'));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit('4'));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit('5'));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit('6'));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit('7'));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit('8'));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit('9'));

	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit('a'));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit('b'));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit('c'));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit('d'));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit('e'));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit('f'));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit('A'));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit('B'));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit('C'));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit('D'));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit('E'));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit('F'));

	TRNT_TEST_EXPECT_TRUE(!CharUtils::IsHexDigit('R'));
	TRNT_TEST_EXPECT_TRUE(!CharUtils::IsHexDigit('$'));

	TRNT_TEST_EXPECT_TRUE(CharUtils::IsBlank(' '));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsBlank('\t'));
	TRNT_TEST_EXPECT_TRUE(!CharUtils::IsBlank('\n'));
	TRNT_TEST_EXPECT_TRUE(!CharUtils::IsBlank('a'));

	TRNT_TEST_EXPECT_TRUE(CharUtils::IsSpace(' '));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsSpace('\t'));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsSpace('\n'));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsSpace('\v'));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsSpace('\f'));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsSpace('\r'));
}

TRNT_IMPL_TEST_CASE(Strings, TCharUtilsForWChar)
{
	using TWCharUtils = TCharUtils<TWChar>;

	static_assert(TAreTheSameType<TWCharUtils::CharType, TWChar>::Value);

	TRNT_TEST_EXPECT_TRUE(TWCharUtils::ToUpperCase(L'a') == L'A');
	TRNT_TEST_EXPECT_TRUE(TWCharUtils::ToUpperCase(L'A') == L'A');

	TRNT_TEST_EXPECT_TRUE(TWCharUtils::ToLowerCase(L'a') == L'a');
	TRNT_TEST_EXPECT_TRUE(TWCharUtils::ToLowerCase(L'A') == L'a');

	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsWhitespace(L' '));
	TRNT_TEST_EXPECT_TRUE(!TWCharUtils::IsWhitespace(L'$'));

	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsAlnum(L'a'));
	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsAlnum(L'0'));
	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsAlnum(L'1'));
	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsAlnum(L'2'));
	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsAlnum(L'3'));
	TRNT_TEST_EXPECT_TRUE(!TWCharUtils::IsAlnum(L'$'));
	TRNT_TEST_EXPECT_TRUE(!TWCharUtils::IsAlnum(L'%'));
	TRNT_TEST_EXPECT_TRUE(!TWCharUtils::IsAlnum(L'#'));

	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsAlpha(L'a'));
	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsAlpha(L'A'));
	TRNT_TEST_EXPECT_TRUE(!TWCharUtils::IsAlpha(L'#'));
	TRNT_TEST_EXPECT_TRUE(!TWCharUtils::IsAlpha(L'1'));

	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsDigit(L'1'));
	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsDigit(L'2'));
	TRNT_TEST_EXPECT_TRUE(!TWCharUtils::IsDigit(L'E'));
	TRNT_TEST_EXPECT_TRUE(!TWCharUtils::IsDigit(L'$'));

	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsUnderscore(L'_'));
	TRNT_TEST_EXPECT_TRUE(!TWCharUtils::IsUnderscore(L'-'));
	TRNT_TEST_EXPECT_TRUE(!TWCharUtils::IsUnderscore(L'a'));

	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsLower(L'a'));
	TRNT_TEST_EXPECT_TRUE(!TWCharUtils::IsLower(L'A'));

	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsUpper(L'A'));
	TRNT_TEST_EXPECT_TRUE(!TWCharUtils::IsUpper(L'a'));

	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsHexDigit(L'0'));
	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsHexDigit(L'1'));
	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsHexDigit(L'2'));
	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsHexDigit(L'3'));
	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsHexDigit(L'4'));
	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsHexDigit(L'5'));
	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsHexDigit(L'6'));
	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsHexDigit(L'7'));
	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsHexDigit(L'8'));
	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsHexDigit(L'9'));

	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsHexDigit(L'a'));
	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsHexDigit(L'b'));
	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsHexDigit(L'c'));
	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsHexDigit(L'd'));
	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsHexDigit(L'e'));
	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsHexDigit(L'f'));
	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsHexDigit(L'A'));
	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsHexDigit(L'B'));
	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsHexDigit(L'C'));
	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsHexDigit(L'D'));
	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsHexDigit(L'E'));
	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsHexDigit(L'F'));

	TRNT_TEST_EXPECT_TRUE(!TWCharUtils::IsHexDigit(L'R'));
	TRNT_TEST_EXPECT_TRUE(!TWCharUtils::IsHexDigit(L'$'));

	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsBlank(L' '));
	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsBlank(L'\t'));
	TRNT_TEST_EXPECT_TRUE(!TWCharUtils::IsBlank(L'\n'));
	TRNT_TEST_EXPECT_TRUE(!TWCharUtils::IsBlank(L'a'));

	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsSpace(L' '));
	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsSpace(L'\t'));
	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsSpace(L'\n'));
	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsSpace(L'\v'));
	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsSpace(L'\f'));
	TRNT_TEST_EXPECT_TRUE(TWCharUtils::IsSpace(L'\r'));
}