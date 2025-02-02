#include "TrinityPCH.h"

#include "Application.h"

TApplication* TApplication::ApplicationInstance = nullptr;

TApplication::TApplication(const TString& ApplicationName)
	: ApplicationName(ApplicationName), QuitRequested(false)
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
