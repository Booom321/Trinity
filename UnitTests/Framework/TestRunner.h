#pragma once

#include "TestSuite.h"

#include <Trinity/Core/Containers/DynamicArray.h>
#include <Trinity/Core/Defines.h>
#include <Trinity/Core/String/String.h>
#include <Trinity/Core/Types/DataTypes.h>
#include <Trinity/Core/Utilities/NonCopyable.h>

#pragma warning(push)
#pragma warning(disable : 26813)
#pragma warning(disable : 26495)

class TTestCaseResult
{
public:
	const TChar* TestCaseName = nullptr;
	TTestResult TestResult = TTestResult::EUnknown;
	TFloat Elapsed = 0.0f;
};

class TRNT_API TTestRunner : public TNonCopyable
{
	TTestRunner() = default;

	~TTestRunner() = default;

public:
	static TRNT_FORCE_INLINE TTestRunner* GetInstance()
	{
		return (TestRunnerInstance) ? TestRunnerInstance : (TestRunnerInstance = new TTestRunner());
	}

	static TRNT_FORCE_INLINE void DeleteInstance()
	{
		if (TestRunnerInstance)
		{
			delete TestRunnerInstance;
			TestRunnerInstance = nullptr;
		}
	}

	TRNT_FORCE_INLINE void SetCurrentTestResult(const TTestResult TestResult)
	{
		CurrentTestResult = TestResult;
	}

	void AddTestCase(TTestCase::TestFunctionPointer TestFunc, const TChar* TestCaseName, const TChar* TestSuiteName);

	void RunAllTests();

	void RunTestSuite(TTestSuite* TestSuite);

	TRNT_FORCE_INLINE void SetCurrentTestInfoIfFailed(const TChar* File, TUInt32 Line)
	{
		if (CurrentTestResult == TTestResult::EFailed)
		{
			CurrentTestFile = File;
			CurrentLine = Line;
		}
	}

private:
	void SaveTestCaseResultsToFile(const TDynamicArray<TTestCaseResult>& TestCaseResults);

	static TTestRunner* TestRunnerInstance;

	std::deque<TTestSuite> TestSuiteQueue;

	TTestCase* CurrentTestCase = nullptr;
	TTestResult CurrentTestResult = TTestResult::EUnknown;

	TUInt32 TestPassedCount = 0;
	TUInt32 TestFailedCount = 0;

	TFloat TotalTestTime = 0.0f;

	const TChar* CurrentTestFile;
	TUInt32 CurrentLine;

	TString OutputDirectory;
};

#pragma warning(pop)