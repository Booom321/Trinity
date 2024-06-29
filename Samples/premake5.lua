project "Samples"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("%{wks.location}/BinOutput/" .. WorkspaceSettings.OutputDir .. "/%{prj.name}")
	objdir ("%{wks.location}/BinIntermediate/" .. WorkspaceSettings.OutputDir .. "/%{prj.name}")

    files 
    {
        "*.h",
        "*.cpp"
    }

    includedirs
    {
        "%{IncludeDirs.EngineRuntime}",
        "%{IncludeDirs.fmt}",
        "%{IncludeDirs.glfw}",
        "%{IncludeDirs.xxHash}",
        "%{IncludeDirs.stb}",
        "%{IncludeDirs.Vulkan}"
    }

    links
    {
        "Engine",
        "Glfw"
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
            "TRNT_USE_VULKAN",
            "TRNT_SUPPORT_GLFW",
            "WIN32_LEAN_AND_MEAN",
			"_CRT_SECURE_NO_WARNINGS"
        }

        libdirs
        {
            VULKAN_SDK .. "/Lib"
        }

        links
        {
            "Shlwapi",
            "vulkan-1"
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