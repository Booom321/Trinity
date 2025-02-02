#pragma once

#include <Trinity/Core/Defines.h>
#include <Trinity/Core/Types/DataTypes.h>

enum class TTestResult : TInt8
{
	EUnknown = -1,
	EFailed = 0,
	EPassed = 1,
};

#pragma warning(push)
#pragma warning(disable : 26495)

class TTestCase
{
public:
	using TestFunctionPointer = void (*)();

	TestFunctionPointer TestFunction;
	const TChar* TestCaseName;

	TRNT_FORCE_INLINE TTestCase(TestFunctionPointer TestFunction, const TChar* TestCaseName)
		: TestFunction(TestFunction), TestCaseName(TestCaseName)
	{}
};

#pragma warning(pop)