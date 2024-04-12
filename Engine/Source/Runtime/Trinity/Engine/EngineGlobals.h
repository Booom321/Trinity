#pragma once

#include "Trinity/Core/Defines.h"
#include "Trinity/Core/String/String.h"

#include "Trinity/Core/Types/Version.h"

class TEngineGlobals
{
public:
	static TString EngineDir;

	static TString EngineShadersDir;

	static TString EngineCompiledSPIRVShadersDir;

public:
	static TVersion EngineVersion;
};