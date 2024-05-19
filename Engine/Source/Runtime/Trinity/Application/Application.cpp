#include "Application.h"

#include "Trinity/Platform/FileSystem.h"
#include "Trinity/Core/String/PathUtils.h"

#include "Trinity/Engine/EngineGlobals.h"

#include "Trinity/Core/Logging/Log.h"

TApplication* TApplication::ApplicationInstance = nullptr;

TApplication::TApplication(const TString& ApplicationName) : ApplicationName(ApplicationName), QuitRequested(false)
{
	TRNT_ASSERT(ApplicationInstance == nullptr);

	ApplicationInstance = this;
}

TRNT_NODISCARD TBool TApplication::OpenProject(const TString& ProjectPath)
{
	// will be implemented later!
	return true;
}

TRNT_NODISCARD TBool TApplication::CreateNewProject(const TString& ProjectDir, const TString& ProjectName)
{
	// will be implemented later!
	return true;
}

TApplication::~TApplication() = default;
