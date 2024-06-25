project "Glfw"
	kind "StaticLib"
	language "C"
	removeplatforms "emscripten"
	staticruntime "Off"

	targetdir ("%{wks.location}/BinOutput/" .. WorkspaceSettings.OutputDir .. "/%{prj.name}")
	objdir ("%{wks.location}/BinIntermediate/" .. WorkspaceSettings.OutputDir .. "/%{prj.name}")

	files
	{
		"include/GLFW/*.h",

		"src/*.h",
		"src/*.c",
		"src/*.m",
		"src/*.m",
		"src/*.in",
	}

	includedirs
	{
		"include/",
		"%{IncludeDirs.Vulkan}"
	}

	filter "system:linux"
		pic "On"
		systemversion "latest"
		defines
		{
			"_GLFW_X11"
		}

	filter "system:windows"
		systemversion "latest"
		defines 
		{ 
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "system:macosx"
		defines 
		{
			"_GLFW_COCOA",
			"_GLFW_USE_RETINA"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"