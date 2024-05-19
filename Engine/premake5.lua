project "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("%{wks.location}/BinOutput/" .. WorkspaceSettings.OutputDir .. "/%{prj.name}")
	objdir ("%{wks.location}/BinIntermediate/" .. WorkspaceSettings.OutputDir .. "/%{prj.name}")

    links
    {
        "Glfw"
    }

    files 
    {
        "Source/Runtime/**.h",
        "Source/Runtime/**.cpp"
    }

    includedirs
    {
        "%{wks.location}/Engine/Source/Runtime",
        "%{IncludeDirs.fmt}",
        "%{IncludeDirs.glfw}",
        "%{IncludeDirs.xxHash}",
        "%{IncludeDirs.stb}",
        "%{IncludeDirs.Vulkan}"
    }

    defines
    {
        "FMT_HEADER_ONLY",
        "STB_IMAGE_IMPLEMENTATION",
        "TRNT_ROOT_DIRECTORY="..__ROOT_DIRECTORY__
    }

    filter "system:windows"
        systemversion "latest"
        conformancemode "on"
        defines
        {
            "TRNT_SUPPORTS_GLFW",
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
        