#pragma once

#include "TestRunner.h"
#include "TestSuite.h"
#include "TestCase.h"
#include "TestMacros.h"

#if defined(_MSC_VER)

#include <stdlib.h>
#include <crtdbg.h>

#	if defined(TRNT_DEBUG)
#		define TRNT_TEST_REPORT_MEMORY_LEAKS _CrtDumpMemoryLeaks()
#	else
#		define TRNT_TEST_REPORT_MEMORY_LEAKS
#	endif

#endif

#define TRNT_TEST_ENTRY_POINT() \
	int main(int Argc, char** Argv)\
	{\
		{\
			TTestRunner::GetInstance()->RunAllTests();\
			TTestRunner::DeleteInstance();\
		}\
		TRNT_TEST_REPORT_MEMORY_LEAKS;\
		printf("\n");\
		system("pause");\
		return 0;\
	}