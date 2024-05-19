include "Scripts/Premake/PremakeGlobals"

WorkspaceSettings.RootDirectory = os.getcwd()

__ROOT_DIRECTORY__ = "\""..WorkspaceSettings.RootDirectory.."\""

print("Root directory: "..WorkspaceSettings.RootDirectory)

workspace( WorkspaceSettings.Name )
    architecture "x86_64"
	configurations { "Debug", "Release" }
	flags { "MultiProcessorCompile" }
	startproject "UnitTests"

-----------------------------------------------------------------------------------------------------------------------------

group "ThirdParty"
	include "Engine/ThirdParty/glfw/premake5"
group ""

-----------------------------------------------------------------------------------------------------------------------------

include "Engine/premake5"
include "Samples/premake5"
include "UnitTests/premake5"