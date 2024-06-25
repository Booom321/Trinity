#include "TrinityPCH.h"

#include "Engine.h"
#include "EngineGlobals.h"
#include "Trinity/Input/InputManager.h"

TRNT_DECLARE_LOG_INFO(Engine, TLogLevel::EDebug);

#if defined(TRNT_USE_GLFW)
#include <GLFW/glfw3.h>

TRNT_DECLARE_LOG_INFO(GlfwCallback, TLogLevel::EDebug);

void GLFWErrorCallback(TInt32 Error, const TChar* Description)
{
	TLog::Error<TRNT_GET_LOG_INFO(GlfwCallback)>("GLFW error {}: {}", Error, Description);
}
#endif

TBool TEngine::Initialize(TInt32 Argc, TChar** Argv)
{
	// will be implemented later!
	return true;
}

void TEngine::Destroy()
{
	// will be implemented later!
	return;
}