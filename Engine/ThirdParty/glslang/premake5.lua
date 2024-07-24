project "Glslang"
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
        -- glslang/
        "glslang/CInterface/*.cpp",
        "glslang/GenericCodeGen/*.cpp",
        "glslang/Include/*.h",
        "glslang/build_info.h",
        "glslang/MachineIndependent/**.h",
        "glslang/MachineIndependent/**.cpp",
        "glslang/Public/*.h",
        "glslang/ResourceLimits/*.cpp",

        -- SPIRV/
        "SPIRV/**.h",
        "SPIRV/**.cpp"
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