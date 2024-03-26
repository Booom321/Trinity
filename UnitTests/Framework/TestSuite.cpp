#include "TestSuite.h"

#include "TestRunner.h"

#include <Trinity/Core/Utilities/OutputStream.h>

TTestSuite::TTestSuite(const TChar* TestSuiteName)
	: TestSuiteName(TestSuiteName)
{}

void TTestSuite::AddTestCase(TTestCase::TestFunctionPointer TestFunc, const TChar* TestCaseName)
{
	TSize_T Index = 0;
	TSize_T Count = TestCaseQueue.size();
	for (; Index < Count; ++Index)
	{
		if (TestCaseQueue[Index].TestCaseName == TestCaseName)
		{
			return;
		}
	}

	TTestCase TestCase(TestFunc, TestCaseName);
	TestCaseQueue.push_back(TestCase);
}