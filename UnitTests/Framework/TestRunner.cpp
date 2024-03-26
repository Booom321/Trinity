#include "TestRunner.h"

#include <Trinity/Core/Utilities/OutputStream.h>
#include <Trinity/Platform/Timer.h>

#include <chrono>

TTestRunner* TTestRunner::TestRunnerInstance = nullptr;

void TTestRunner::AddTestCase(TTestCase::TestFunctionPointer TestFunc, const TChar* TestCaseName, const TChar* TestSuiteName)
{
	TInt64 FoundIndex = -1;
	TInt64 Count = (TInt64)TestSuiteQueue.size();
	for (TInt64 Index = 0; Index < Count; ++Index)
	{
		if (TestSuiteQueue[Index].GetSuiteName() == TestSuiteName)
		{
			FoundIndex = Index;
			break;
		}
	}

	if (FoundIndex == -1)
	{
		TestSuiteQueue.push_back(TTestSuite(TestSuiteName));
		TestSuiteQueue.back().AddTestCase(TestFunc, TestCaseName);
	}
	else
	{
		TestSuiteQueue[FoundIndex].AddTestCase(TestFunc, TestCaseName);
	}
}

void TTestRunner::RunAllTests()
{
	Printf("===================================== Test starts at {:%Y-%m-%d %H:%M:%S} =====================================\n", std::chrono::system_clock::now());

	TTestSuite* CurrentTestSuite = nullptr;
	while (!TestSuiteQueue.empty())
	{
		CurrentTestSuite = &TestSuiteQueue.front();
		RunTestSuite(CurrentTestSuite);
		TestSuiteQueue.pop_front();
	}

	Printf("======================================================================================================================\n");

	Printf("Results: \033[92m{} passed\033[39m, \033[91m{} failed\033[39m\n", TestPassedCount, TestFailedCount);
	Printf("Total time: {:.5f}ms\n", TotalTestTime);
}
#pragma warning(push)
#pragma warning(disable: 26813)

void TTestRunner::RunTestSuite(TTestSuite* TestSuite)
{
	std::deque<TTestCase>& TestCaseQueue = TestSuite->GetTestCaseQueue();
	TDynamicArray<TTestCaseResult> TestCaseResults(TestCaseQueue.size());

	TTimer Timer;
	TFloat Elapsed;

	const TChar* TestSuiteName = TestSuite->GetSuiteName();

	Printf("\033[97m\x1b[1mTest suite \"{}\":\x1b[0m\033[39m\n", TestSuiteName);

	TTestCaseResult CurrentTestCaseResult{};

	while (!TestCaseQueue.empty())
	{
		CurrentTestResult = TTestResult::EPassed;
		CurrentTestCase = &TestCaseQueue.front();

		Timer.Reset();
		CurrentTestCase->TestFunction();
		Elapsed = Timer.GetElapsedMillisecond<TFloat>();

		CurrentTestCaseResult.TestCaseName = CurrentTestCase->TestCaseName;
		CurrentTestCaseResult.TestResult = CurrentTestResult;
		CurrentTestCaseResult.Elapsed = Elapsed;

		TestCaseResults.EmplaceBack(CurrentTestCaseResult);

		TotalTestTime += Elapsed;

		if (CurrentTestResult == TTestResult::EPassed)
		{
			Printf("\t\033[92mPASSED\033[39m\t\033[2m{} ({:.5f}ms)\033[0m\n", CurrentTestCase->TestCaseName, Elapsed);
			++TestPassedCount;
		}
		else
		{
			Printf("\t\033[91mFAILED\033[39m\t\033[2m{} ({:.5f}ms)\n\t\t-> {}:{}\033[0m\n", CurrentTestCase->TestCaseName, Elapsed, CurrentTestFile, CurrentLine);
			++TestFailedCount;
		}

		TestCaseQueue.pop_front();
	}
}

void TTestRunner::SaveTestCaseResultsToFile(const TDynamicArray<TTestCaseResult>& TestCaseResults)
{
	// will be implemented later :v
}

#pragma warning(pop)