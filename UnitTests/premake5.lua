project "UnitTests"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("%{wks.location}/BinOutput/" .. WorkspaceSettings.OutputDir .. "/%{prj.name}")
	objdir ("%{wks.location}/BinIntermediate/" .. WorkspaceSettings.OutputDir .. "/%{prj.name}")

    files 
    {
        "*.h",
        "*.cpp",

        "CoreTest/**.h",
        "CoreTest/**.cpp",

        "Framework/**.h",
        "Framework/**.cpp",
    }

    includedirs
    {
        "%{prj.location}",

        "%{IncludeDirs.EngineRuntime}",
        "%{IncludeDirs.fmt}",
        "%{IncludeDirs.xxHash}"
    }

    links
    {
        "Engine"
    }

    defines
    {
        "FMT_HEADER_ONLY",
        "TRNT_ROOT_DIRECTORY="..__ROOT_DIRECTORY__
    }

    filter "system:windows"
        systemversion "latest"
        conformancemode "on"
        defines
        {
            "WIN32_LEAN_AND_MEAN",
			"_CRT_SECURE_NO_WARNINGS"
        }

        links
        {
            "Shlwapi"
        }

    filter "configurations:Debug"
        defines { "_DEBUG", "DEBUG" }
        symbols "On"
		runtime "Debug"
		optimize "Off"

    filter "configurations:Release"
        defines { "NDEBUG" }
        symbols "Off"
		runtime "Release"
		optimize "On"