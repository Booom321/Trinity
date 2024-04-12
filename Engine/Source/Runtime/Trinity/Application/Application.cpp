#include "Application.h"

#include "Trinity/Platform/FileSystem.h"

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
	const TChar* ProjectPathCString = ProjectPath.GetData();

	if (!TFileSystem::FileExists(ProjectPathCString))
	{
		TLog::Error<TRNT_GET_LOG_INFO(Default)>("Project path does not exist: {}", ProjectPathCString);
		return false;
	}

	ProjectProps.ProjectName = TFileSystem::GetFileNameWithoutExtension(ProjectPathCString);

	const TChar* FoundSlash = TCString<TChar>::Strrpbrk(ProjectPathCString, ProjectPathCString + ProjectPath.Length(), "/\\");
	TInt64 FoundSlashIndex = static_cast<TInt64>((FoundSlash) ? FoundSlash - ProjectPathCString : -1);

	if (FoundSlashIndex != -1)
	{
		ProjectProps.ProjectRoot = ProjectPath.Substring(0, ++FoundSlashIndex);
		ProjectProps.ProjectAssetsDir = ProjectProps.ProjectRoot + "Assets/";
	}
	else
	{
		TLog::Error<TRNT_GET_LOG_INFO(Default)>("Invalid project path: {}", ProjectPathCString);

		return false;
	}

	TLog::Success<TRNT_GET_LOG_INFO(Default)>("Open project successfully: {}", ProjectPathCString);

	return true;
}

TRNT_NODISCARD TBool TApplication::CreateNewProject(const TString& ProjectDir, const TString& ProjectName)
{
	ProjectProps.ProjectName = ProjectName;
	ProjectProps.ProjectRoot = ProjectDir;

	if (ProjectDir.Last() == '/' || ProjectDir.Last() == '\\')
	{
		ProjectProps.ProjectAssetsDir = ProjectDir + "Assets/";
	}
	else
	{
		ProjectProps.ProjectAssetsDir = ProjectDir + "/Assets/";
	}

#undef CreateDirectory

	TFileSystem::CreateDirectory(ProjectDir.GetData());

	if (!TFileSystem::DirectoryExists(ProjectProps.ProjectAssetsDir.GetData()))
	{
		TFileSystem::CreateDirectory(ProjectProps.ProjectAssetsDir.GetData());
	}

	TString ProjectAssetsSubdir = ProjectProps.ProjectAssetsDir + "Models/";
	if (!TFileSystem::DirectoryExists(ProjectAssetsSubdir.GetData()))
	{
		TFileSystem::CreateDirectory(ProjectAssetsSubdir.GetData());
	}

	ProjectAssetsSubdir = ProjectProps.ProjectAssetsDir + "Textures/";
	if (!TFileSystem::DirectoryExists(ProjectAssetsSubdir.GetData()))
	{
		TFileSystem::CreateDirectory(ProjectAssetsSubdir.GetData());
	}

	ProjectAssetsSubdir = ProjectProps.ProjectAssetsDir + "Materials/";
	if (!TFileSystem::DirectoryExists(ProjectAssetsSubdir.GetData()))
	{
		TFileSystem::CreateDirectory(ProjectAssetsSubdir.GetData());
	}

	TLog::Success<TRNT_GET_LOG_INFO(Default)>("Create project successfully!");

	return true;
}

TApplication::~TApplication() = default;
