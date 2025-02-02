#pragma once

#include "Framework/TestMacros.h"

#define TRNT_TEST_CHAR
#include "Framework/StringTestMacros.h"

#if defined(TRNT_TEST_CHAR_TYPE)
TRNT_DECL_TEST_CASE(Strings, TCString_Char);
#elif defined(TRNT_TEST_WCHAR_TYPE)
TRNT_DECL_TEST_CASE(Strings, TCString_WChar);
#endif