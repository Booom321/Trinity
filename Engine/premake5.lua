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
        "Glfw"
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
        "%{IncludeDirs.EngineRuntime}",
        "%{IncludeDirs.glfw}",
        "%{IncludeDirs.fmt}",
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
            "TRNT_USE_NULL_RHI",
            "TRNT_USE_VULKAN_RHI",
            "TRNT_USE_DIRECTX11_RHI",
            "TRNT_USE_DIRECTX12_RHI",
            --------------------------- 
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
        