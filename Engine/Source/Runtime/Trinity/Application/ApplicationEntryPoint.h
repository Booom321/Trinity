#pragma once

#include "Application.h"

#include "Trinity/Engine/Engine.h"
#include "Trinity/Platform/Timer.h"

template<typename ApplicationClass, typename ... ArgsType>
static TInt32 ApplicationEntryPoint(int Argc, char** Argv, const TChar* AppName, ArgsType&&... Args)
{
	if (!TEngine::Initialize(Argc, Argv))
	{
		return -1;
	}

	ApplicationClass* App = new ApplicationClass(Forward<ArgsType>(Args)...);
	App->SetApplicationName(AppName);
	App->OnInitialize();

	TTimer Timer{};
	while (!App->QuitRequested)
	{
		Timer.Reset();
		App->OnUpdate(Timer.GetElapsedMillisecond<TDouble>());
	}

	App->OnQuit();

	delete App;
	App = nullptr;

	TEngine::Destroy();
	
	return 0;
}

#ifdef _MSC_VER

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#if defined(TRNT_DEBUG)
#	define TRNT_REPORT_MEMORY_LEAKS _CrtDumpMemoryLeaks()
#else
#	define TRNT_REPORT_MEMORY_LEAKS
#endif

#if !defined(TRNT_ENTRY_POINT)
#	define TRNT_ENTRY_POINT(ApplicationClass, AppName, ...) \
	int main(int Argc, char** Argv)\
	{\
		TInt32 ReturnCode;\
		{\
			ReturnCode = ApplicationEntryPoint<ApplicationClass>(Argc, Argv, AppName, __VA_ARGS__);\
		}\
		TRNT_REPORT_MEMORY_LEAKS;\
		return ReturnCode;\
	}
#endif

#else

#if !defined(TRNT_ENTRY_POINT)
#	define TRNT_ENTRY_POINT(ApplicationClass, AppName, ...) \
	int main(int Argc, char** Argv)\
	{\
		return ApplicationEntryPoint<ApplicationClass>(Argc, Argv, AppName, __VA_ARGS__);\
	}
#endif

#endif