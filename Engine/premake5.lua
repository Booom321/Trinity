project "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "Off"
    editandcontinue "Off"

    pchheader("TrinityPCH.h")
    pchsource("Source/PCH/TrinityPCH.cpp")

    targetdir ("%{wks.location}/BinOutput/" .. WorkspaceSettings.OutputDir .. "/%{prj.name}")
	objdir ("%{wks.location}/BinIntermediate/" .. WorkspaceSettings.OutputDir .. "/%{prj.name}")

    links
    {
        "Glfw",
        "ImGui",
        "Glslang",
        "SPIRVCross",
    }

    files 
    {
        "Source/PCH/*.h",
        "Source/PCH/*.cpp",
        EngineRuntimeLocation.."/**.h",
        EngineRuntimeLocation.."/**.cpp"
    }

    includedirs
    {
        "Source/PCH",
        "%{IncludeDirs.glm}",
        "%{IncludeDirs.fmt}",
        "%{IncludeDirs.stb}",
        "%{IncludeDirs.glfw}",
        "%{IncludeDirs.imgui}",
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

    defines
    {
        "FMT_HEADER_ONLY",
        "STB_IMAGE_IMPLEMENTATION",
        "TRNT_ROOT_DIRECTORY="..__ROOT_DIRECTORY__,
        "TRNT_ROOT_DIRECTORY_W=L"..__ROOT_DIRECTORY__
    }
    
    filter 'files:ThirdParty/**.cpp'
        flags  { "NoPCH" }
    filter 'files:ThirdParty/**.c'
        flags  { "NoPCH" }
    filter { "files:ThirdParty/**"}
        warnings "Off"

    filter "system:windows"
        systemversion "latest"
        conformancemode "on"
        defines
        {
            ---- RHI ------------------
            "TRNT_SUPPORT_NULL_RHI",
            "TRNT_SUPPORT_DIRECTX11_RHI",
            "TRNT_SUPPORT_DIRECTX12_RHI",
            --------------------------- 
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
        