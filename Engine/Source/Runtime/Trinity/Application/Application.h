#pragma once

#include "Trinity/Core/Types/DataTypes.h"

#include "Trinity/Core/String/String.h"

class TProjectProperties
{
public:
	TString ProjectRoot;
	TString ProjectName;
	TString ProjectAssetsDir;
};

class TRNT_API TApplication
{
	TRNT_DISALLOW_COPY_AND_ASSIGN(TApplication);

public:
	template<typename ApplicationClass, typename ... ArgsType>
	friend TInt32 ApplicationEntryPoint(int Argc, char** Argv, const TChar* AppName, ArgsType&&... Args);

	explicit TApplication(const TString& ApplicationName);

	virtual ~TApplication();

public:
	virtual void OnInitialize() = 0;

	virtual void OnUpdate(const TDouble DeltaTime) = 0;

	virtual void OnQuit() = 0;

public:
	TRNT_FORCE_INLINE const TString& GetApplicationName() const { return ApplicationName; }

	TRNT_FORCE_INLINE void SetApplicationName(const TString& ApplicationName) { this->ApplicationName = ApplicationName; }

	TRNT_NODISCARD TBool OpenProject(const TString& ProjectPath);

	TRNT_NODISCARD TBool CreateNewProject(const TString& ProjectDir, const TString& ProjectName);

	TRNT_FORCE_INLINE const TProjectProperties& GetProjectProperties() const { return ProjectProps; }

	TRNT_FORCE_INLINE void RequestQuit() { this->QuitRequested = true; }

	TRNT_INLINE TBool IsQuitRequested() const { return QuitRequested; }

public:
	static TRNT_FORCE_INLINE TApplication* GetApplicationInstance() { return ApplicationInstance; }

protected:
	TString ApplicationName;

	TProjectProperties ProjectProps;

	TBool QuitRequested;

private:
	static TApplication* ApplicationInstance;
};