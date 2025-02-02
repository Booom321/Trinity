#include "CharUtilsTest.h"

#include <Trinity/Core/String/CharUtils.h>

#if defined(TRNT_TEST_CHAR_TYPE)
TRNT_IMPL_TEST_CASE(Strings, TCharUtils_Char)
#elif defined(TRNT_TEST_WCHAR_TYPE)
TRNT_IMPL_TEST_CASE(Strings, TCharUtils_WChar)
#endif
{
	using CharUtils = TCharUtils<TRNT_CHAR_TYPE>;
	static_assert(TAreTheSameType<CharUtils::CharType, TRNT_CHAR_TYPE>::Value);

	TRNT_TEST_EXPECT_TRUE(CharUtils::ToUpperCase(TRNT_TEXT('a')) == TRNT_TEXT('A'));
	TRNT_TEST_EXPECT_TRUE(CharUtils::ToUpperCase(TRNT_TEXT('A')) == TRNT_TEXT('A'));

	TRNT_TEST_EXPECT_TRUE(CharUtils::ToLowerCase(TRNT_TEXT('a')) == TRNT_TEXT('a'));
	TRNT_TEST_EXPECT_TRUE(CharUtils::ToLowerCase(TRNT_TEXT('A')) == TRNT_TEXT('a'));

	TRNT_TEST_EXPECT_TRUE(CharUtils::IsWhitespace(TRNT_TEXT(' ')));
	TRNT_TEST_EXPECT_FALSE(CharUtils::IsWhitespace(TRNT_TEXT('$')));

	TRNT_TEST_EXPECT_TRUE(CharUtils::IsAlnum(TRNT_TEXT('a')));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsAlnum(TRNT_TEXT('0')));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsAlnum(TRNT_TEXT('1')));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsAlnum(TRNT_TEXT('2')));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsAlnum(TRNT_TEXT('3')));
	TRNT_TEST_EXPECT_FALSE(CharUtils::IsAlnum(TRNT_TEXT('$')));
	TRNT_TEST_EXPECT_FALSE(CharUtils::IsAlnum(TRNT_TEXT('%')));
	TRNT_TEST_EXPECT_FALSE(CharUtils::IsAlnum(TRNT_TEXT('#')));

	TRNT_TEST_EXPECT_TRUE(CharUtils::IsAlpha(TRNT_TEXT('a')));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsAlpha(TRNT_TEXT('A')));
	TRNT_TEST_EXPECT_FALSE(CharUtils::IsAlpha(TRNT_TEXT('#')));
	TRNT_TEST_EXPECT_FALSE(CharUtils::IsAlpha(TRNT_TEXT('1')));

	TRNT_TEST_EXPECT_TRUE(CharUtils::IsDigit(TRNT_TEXT('1')));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsDigit(TRNT_TEXT('2')));
	TRNT_TEST_EXPECT_TRUE(!CharUtils::IsDigit(TRNT_TEXT('E')));
	TRNT_TEST_EXPECT_TRUE(!CharUtils::IsDigit(TRNT_TEXT('$')));

	TRNT_TEST_EXPECT_TRUE(CharUtils::IsUnderscore(TRNT_TEXT('_')));
	TRNT_TEST_EXPECT_TRUE(!CharUtils::IsUnderscore(TRNT_TEXT('-')));
	TRNT_TEST_EXPECT_TRUE(!CharUtils::IsUnderscore(TRNT_TEXT('a')));

	TRNT_TEST_EXPECT_TRUE(CharUtils::IsLower(TRNT_TEXT('a')));
	TRNT_TEST_EXPECT_TRUE(!CharUtils::IsLower(TRNT_TEXT('A')));

	TRNT_TEST_EXPECT_TRUE(CharUtils::IsUpper(TRNT_TEXT('A')));
	TRNT_TEST_EXPECT_TRUE(!CharUtils::IsUpper(TRNT_TEXT('a')));

	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit(TRNT_TEXT('0')));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit(TRNT_TEXT('1')));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit(TRNT_TEXT('2')));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit(TRNT_TEXT('3')));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit(TRNT_TEXT('4')));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit(TRNT_TEXT('5')));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit(TRNT_TEXT('6')));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit(TRNT_TEXT('7')));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit(TRNT_TEXT('8')));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit(TRNT_TEXT('9')));

	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit(TRNT_TEXT('a')));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit(TRNT_TEXT('b')));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit(TRNT_TEXT('c')));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit(TRNT_TEXT('d')));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit(TRNT_TEXT('e')));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit(TRNT_TEXT('f')));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit(TRNT_TEXT('A')));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit(TRNT_TEXT('B')));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit(TRNT_TEXT('C')));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit(TRNT_TEXT('D')));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit(TRNT_TEXT('E')));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsHexDigit(TRNT_TEXT('F')));

	TRNT_TEST_EXPECT_TRUE(!CharUtils::IsHexDigit(TRNT_TEXT('R')));
	TRNT_TEST_EXPECT_TRUE(!CharUtils::IsHexDigit(TRNT_TEXT('$')));

	TRNT_TEST_EXPECT_TRUE(CharUtils::IsBlank(TRNT_TEXT(' ')));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsBlank(TRNT_TEXT('\t')));
	TRNT_TEST_EXPECT_TRUE(!CharUtils::IsBlank(TRNT_TEXT('\n')));
	TRNT_TEST_EXPECT_TRUE(!CharUtils::IsBlank(TRNT_TEXT('a')));

	TRNT_TEST_EXPECT_TRUE(CharUtils::IsSpace(TRNT_TEXT(' ')));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsSpace(TRNT_TEXT('\t')));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsSpace(TRNT_TEXT('\n')));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsSpace(TRNT_TEXT('\v')));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsSpace(TRNT_TEXT('\f')));
	TRNT_TEST_EXPECT_TRUE(CharUtils::IsSpace(TRNT_TEXT('\r')));
}