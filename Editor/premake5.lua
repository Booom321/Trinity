project "Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"
    editandcontinue "Off"

    targetdir ("%{wks.location}/BinOutput/" .. WorkspaceSettings.OutputDir .. "/%{prj.name}")
	objdir ("%{wks.location}/BinIntermediate/" .. WorkspaceSettings.OutputDir .. "/%{prj.name}")

    files
    {
        "**.h",
        "**.cpp",
        "*.h",
        "*.cpp"
    }

    includedirs
    {
        "%{IncludeDirs.fmt}",
        "%{IncludeDirs.stb}",
        "%{IncludeDirs.glfw}",
        "%{IncludeDirs.xxHash}",
        "%{IncludeDirs.glslang}",
        "%{IncludeDirs.SPIRVCross}",
        "%{IncludeDirs.EngineRuntime}",
    }
    

    if (EngineFeatures.SupportVulkan == true) then
        includedirs
        {
            "%{IncludeDirs.Vulkan}"
        }

        defines
        {
            "TRNT_SUPPORT_VULKAN_RHI"
        }
        
        libdirs
        {
            "%{LibDirs.Vulkan}"
        }

        links 
        {
            "vulkan-1"
        }
    end

    links 
    {
        "Engine",
        "Glfw",
        "ImGui",
        "Glslang",
        "SPIRVCross",
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
            "TRNT_SUPPORT_NULL_RHI",
            "TRNT_SUPPORT_DIRECTX11_RHI",
            "TRNT_SUPPORT_DIRECTX12_RHI",
            "TRNT_SUPPORT_GLFW",
            "WIN32_LEAN_AND_MEAN",
            "_CRT_SECURE_NO_WARNINGS"
        }

        links
        {
            "Shlwapi",
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