#include "Engine.h"

#include "EngineGlobals.h"

#include "Trinity/Core/Config.h"
#include "Trinity/Core/PlatformDetection.h"
#include "Trinity/Core/Logging/Log.h"

#include "Trinity/Platform/FileSystem.h"

#include "Trinity/Input/InputManager.h"

#include <GLFW/glfw3.h>

TRNT_DECLARE_LOG_INFO(Engine, TLogLevel::EDebug);

#if defined(TRNT_USE_GLFW)

TRNT_DECLARE_LOG_INFO(GlfwCallback, TLogLevel::EDebug);

void GLFWErrorCallback(TInt32 Error, const TChar* Description)
{
	TLog::Error<TRNT_GET_LOG_INFO(GlfwCallback)>("GLFW error {}: {}", Error, Description);
}
#endif

TBool TEngine::Initialize(TInt32 Argc, TChar** Argv)
{
	/*TEngineGlobals::CommandLine.ArgumentsCount = Argc;
	TEngineGlobals::CommandLine.Arguments = Argv;

	if (!TCommandLineParser::Parse(TEngineGlobals::CommandLine))
	{
		TLog::Error<TRNT_GET_LOG_INFO(Engine)>("Failed to parse command line!");
		return false;
	}*/

#if defined(TRNT_USE_GLFW)
	if (!glfwInit())
	{
		const TChar* ErrorDescription = nullptr;
		int ErrorCode = glfwGetError(&ErrorDescription);

		TLog::Error<TRNT_GET_LOG_INFO(Engine)>("Failed to initialize GLFW | Error {}: {}", ErrorCode, ErrorDescription);

		return false;
	}
	glfwSetErrorCallback(GLFWErrorCallback);
	TLog::Success<TRNT_GET_LOG_INFO(Engine)>("Initialize GLFW successfully!");
#endif

	TInputManager::Initialize();

	TEngineGlobals::EngineDir = TFileSystem::GetExecutablePath();

	TFileSystem::NormalizePath(TEngineGlobals::EngineDir.GetData());

	TLog::Info<TRNT_GET_LOG_INFO(Engine)>("Executable path: {}", TEngineGlobals::EngineDir.GetData());

	TInt64 Index = TEngineGlobals::EngineDir.Find("Trinity/");

	if (Index != TString::Npos)
	{
		Index += TCString<TChar>::Strlen("Trinity/");
		TEngineGlobals::EngineDir.RemoveAt(Index, TEngineGlobals::EngineDir.Length() - Index);
		TLog::Info<TRNT_GET_LOG_INFO(Engine)>("Engine directory: {}", TEngineGlobals::EngineDir.GetData());
	}
	else
	{
		TLog::Error<TRNT_GET_LOG_INFO(Engine)>("Invalid engine directory: {}", TEngineGlobals::EngineDir.GetData());

		return false;
	}

	TEngineGlobals::EngineShadersDir = TEngineGlobals::EngineDir + "Engine/Shaders/";
	TLog::Info<TRNT_GET_LOG_INFO(Engine)>("Shaders directory: {}", TEngineGlobals::EngineShadersDir.GetData());

	TEngineGlobals::EngineCompiledSPIRVShadersDir = TEngineGlobals::EngineShadersDir + "CompiledSPV/";
	TLog::Info<TRNT_GET_LOG_INFO(Engine)>("Compiled SPIR-V shaders directory: {}", TEngineGlobals::EngineCompiledSPIRVShadersDir.GetData());

	return true;
}

void TEngine::Destroy()
{
#if defined(TRNT_USE_GLFW)
	glfwSetErrorCallback(nullptr);
	glfwTerminate();

	TLog::Info<TRNT_GET_LOG_INFO(Engine)>("GLFW terminated!");
#endif

	TInputManager::DeleteInstance();
}