#pragma once

#include "TestCase.h"
#include "TestMacros.h"
#include "TestRunner.h"
#include "TestSuite.h"

#if defined(_MSC_VER)
	#include <crtdbg.h>
	#include <stdlib.h>

	#if defined(TRNT_DEBUG)
		#define TRNT_TEST_REPORT_MEMORY_LEAKS _CrtDumpMemoryLeaks()
	#else
		#define TRNT_TEST_REPORT_MEMORY_LEAKS
	#endif
#else
	#define TRNT_TEST_REPORT_MEMORY_LEAKS
#endif

#define TRNT_TEST_ENTRY_POINT()                        \
	int main(int Argc, char** Argv)                    \
	{                                                  \
		{                                              \
			TTestRunner::GetInstance()->RunAllTests(); \
			TTestRunner::DeleteInstance();             \
		}                                              \
		TRNT_TEST_REPORT_MEMORY_LEAKS;                 \
		printf("\n");                                  \
		system("pause");                               \
		return 0;                                      \
	}