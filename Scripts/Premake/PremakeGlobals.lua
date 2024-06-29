WorkspaceSettings = {}
WorkspaceSettings.Name = "Trinity"
WorkspaceSettings.OutputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
--------------------------------------------------------------------------------------------------------------------
VULKAN_SDK = os.getenv("VULKAN_SDK")

if (VULKAN_SDK == nil or VULKAN_SDK == '') then
    print("Environment variable not found: VULKAN_SDK")
else
    print("VULKAN_SDK environment varible is: ".. VULKAN_SDK)
end

ThirdPartyLocation = "%{wks.location}/Engine/ThirdParty"
EngineRuntimeLocation = "%{wks.location}/Engine/Source/Runtime"
--------------------------------------------------------------------------------------------------------------------
IncludeDirs = {}

IncludeDirs["fmt"] = "%{ThirdPartyLocation}/fmt/include"
IncludeDirs["glfw"] = "%{ThirdPartyLocation}/glfw/include"
IncludeDirs["stb"] = "%{ThirdPartyLocation}/stb"
IncludeDirs["xxHash"] = "%{ThirdPartyLocation}/xxHash"
IncludeDirs["imgui"] = "%{ThirdPartyLocation}/imgui"
IncludeDirs["Vulkan"] = "%{VULKAN_SDK}/Include"
IncludeDirs["EngineRuntime"] = EngineRuntimeLocation
--------------------------------------------------------------------------------------------------------------------