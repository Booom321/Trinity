#pragma once

#include "Trinity/Core/Assert/AssertionMacros.h"

#include "TestRunner.h"

#if !defined(TRNT_DECL_TEST_CASE)
#	define TRNT_DECL_TEST_CASE(TestSuiteName, TestCaseName) \
	struct TTestCase_##TestSuiteName##_##TestCaseName\
	{\
		TTestCase_##TestSuiteName##_##TestCaseName()\
		{\
			TTestRunner::GetInstance()->AddTestCase(RunTest, TRNT_STRINGIFY(TestCaseName), TRNT_STRINGIFY(TestSuiteName));\
		}\
		static void RunTest();\
	};\
	static TTestCase_##TestSuiteName##_##TestCaseName TestCase_##TestSuiteName##_##TestCaseName
#endif

#if !defined(TRNT_IMPL_TEST_CASE)
#	define TRNT_IMPL_TEST_CASE(TestSuiteName, TestCaseName) void TTestCase_##TestSuiteName##_##TestCaseName::RunTest()
#endif

#define TRNT_TEST_SECTION(TestSectionName, ...)

#define TRNT_TEST_EXPECT_TRUE(Condition) \
	if (!(Condition))\
	{\
		TTestRunner::GetInstance()->SetCurrentTestResult(TTestResult::EFailed);\
		TTestRunner::GetInstance()->SetCurrentTestInfoIfFailed(__FILE__, __LINE__);\
		return;\
	}

#define TRNT_TEST_EXPECT_FALSE(Condition) \
	if (Condition)\
	{\
		TTestRunner::GetInstance()->SetCurrentTestResult(TTestResult::EFailed);\
		TTestRunner::GetInstance()->SetCurrentTestInfoIfFailed(__FILE__, __LINE__);\
		return;\
	}

#define TRNT_TEST_EXPECT_EQ(Value1, Value2) TRNT_TEST_EXPECT_TRUE(Value1 == Value2)

#define TRNT_TEST_EXPECT_NEQ(Value1, Value2) TRNT_TEST_EXPECT_TRUE(Value1 != Value2)

#define TRNT_TEST_ASSERT(Condition)	TRNT_ASSERT(Condition)