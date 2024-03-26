#pragma once

#include "TestCase.h"

#include <deque>

class TRNT_API TTestSuite
{
public:
	TTestSuite(const TChar* TestSuiteName);

	~TTestSuite() = default;

public:
	void AddTestCase(TTestCase::TestFunctionPointer TestFunc, const TChar* TestCaseName);

	TRNT_FORCE_INLINE const std::deque<TTestCase>& GetTestCaseQueue() const
	{
		return TestCaseQueue;
	}

	TRNT_FORCE_INLINE std::deque<TTestCase>& GetTestCaseQueue()
	{
		return TestCaseQueue;
	}

	TRNT_FORCE_INLINE const TChar* GetSuiteName() const
	{
		return TestSuiteName;
	}

private:
	std::deque<TTestCase> TestCaseQueue;
	const TChar* TestSuiteName;
};