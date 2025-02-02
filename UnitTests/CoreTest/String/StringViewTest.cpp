#include "StringViewTest.h"
#include "Trinity/Core/String/StringView.h"

using StringView = TStringViewBase<TRNT_CHAR_TYPE>;

static constexpr StringView sv1(TRNT_TEXT("hello world"));
static constexpr StringView sv2(TRNT_TEXT("HELLO WORLD"));

static constexpr StringView sv2_copy(sv2);
static constexpr StringView sv3{};

static constexpr StringView sv4 = sv2;

static_assert(sv1.GetElementCount() == 11);
static_assert(sv1[0] == TRNT_TEXT('h'));
static_assert(sv1.CharAt(0) == TRNT_TEXT('h'));
static_assert(sv1.First() == TRNT_TEXT('h'));
static_assert(sv1.Last() == TRNT_TEXT('d'));
static_assert(!sv1.IsEmpty());
static_assert(sv3.IsEmpty());

static_assert(sv1.Substring(0, 5) == StringView{ TRNT_TEXT("hello") });
static_assert(sv1.Substring() == sv1);

static_assert(sv1.Compare(sv2) != 0);
static_assert(sv1.Compare(sv2, TStringSearchCase::EIgnoreCase) == 0);

static_assert(sv1.StartsWith(TRNT_TEXT("hello")));
static_assert(sv1.StartsWith(TRNT_TEXT("HeLlO"), TStringSearchCase::EIgnoreCase));
static_assert(sv1.StartsWith(TRNT_TEXT('h')));
static_assert(sv1.StartsWith(TRNT_TEXT("llo")) == false);

static_assert(sv1.EndsWith(TRNT_TEXT("world")));
static_assert(sv1.EndsWith(TRNT_TEXT("RLD"), TStringSearchCase::EIgnoreCase));
static_assert(sv1.EndsWith(TRNT_TEXT('d')));
static_assert(sv1.EndsWith(TRNT_TEXT("abc")) == false);

static_assert(sv1 != sv2);
static_assert(sv3 != sv2);
static_assert(sv2_copy == sv2);

static_assert(sv1.Find(TRNT_TEXT("hello")) == 0);
static_assert(sv1.Find(TRNT_TEXT("hello"), 4) == -1);

static constexpr StringView sv5(TRNT_TEXT("abc abc abc"));

static_assert(sv5.Find(TRNT_TEXT("ABC"), 2, TStringSearchCase::EIgnoreCase) == 4);
static_assert(sv5.FindLast(TRNT_TEXT("abc")) == 8);
static_assert(sv5.FindLast(TRNT_TEXT("abc"), 2) == 0);

static_assert(sv5.FindChar(TRNT_TEXT('c')) == 2);
static_assert(sv5.FindChar(TRNT_TEXT('c'), 5) == 6);

static_assert(sv5.FindLastChar(TRNT_TEXT('c')) == 10);
static_assert(sv5.FindLastChar(TRNT_TEXT('c'), 8) == 6);
static_assert(sv5.FindLastChar(TRNT_TEXT('c'), 1) == -1);

#if defined(TRNT_TEST_CHAR_TYPE)
TRNT_IMPL_TEST_CASE(Strings, TStringView_Char)
#elif defined(TRNT_TEST_WCHAR_TYPE)
TRNT_IMPL_TEST_CASE(Strings, TStringView_WChar)
#endif
{
}