project "SPIRVCross"
    kind "StaticLib"
    language "C++"
	cppdialect "C++17"
    staticruntime "Off"

    targetdir ("%{wks.location}/BinOutput/" .. WorkspaceSettings.OutputDir .. "/%{prj.name}")
	objdir ("%{wks.location}/BinIntermediate/" .. WorkspaceSettings.OutputDir .. "/%{prj.name}")

    includedirs 
    {
        "./",
    }

    files 
    {
        "*.h",
        "*.hpp",
        "*.cpp",
    }

    filter "system:windows"
    systemversion "latest"

    filter "system:linux"
        pic "On"
        systemversion "latest"

    filter "system:macosx"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"