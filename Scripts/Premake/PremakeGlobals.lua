function DirectoryExists(DirectoryPath)
    local Ok, Error, Code = os.rename(DirectoryPath, DirectoryPath)
    if not Ok then
       if Code == 13 then
          return true
       end
    end
    return Ok, Error
 end

WorkspaceSettings = {}
WorkspaceSettings.Name = "Trinity"
WorkspaceSettings.OutputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
--------------------------------------------------------------------------------------------------------------------
VULKAN_SDK = os.getenv("VULKAN_SDK")

EngineFeatures = {}
EngineFeatures.SupportVulkan = false 

if (VULKAN_SDK == nil or VULKAN_SDK == '') then
    print("Environment variable not found: VULKAN_SDK")
else
    print("VULKAN_SDK environment varible is: ".. VULKAN_SDK)
    if (DirectoryExists(VULKAN_SDK)) then
        EngineFeatures.SupportVulkan = true
    else
        print(VULKAN_SDK.." doesn't exist.")
    end
end

ThirdPartyLocation = "%{wks.location}/Engine/ThirdParty"
EngineRuntimeLocation = "%{wks.location}/Engine/Source/Runtime"
--------------------------------------------------------------------------------------------------------------------
IncludeDirs = {}
IncludeDirs["EngineRuntime"] = EngineRuntimeLocation

IncludeDirs["fmt"] = "%{ThirdPartyLocation}/fmt/include"
IncludeDirs["glfw"] = "%{ThirdPartyLocation}/glfw/include"
IncludeDirs["stb"] = "%{ThirdPartyLocation}/stb"
IncludeDirs["xxHash"] = "%{ThirdPartyLocation}/xxHash"
IncludeDirs["imgui"] = "%{ThirdPartyLocation}/imgui"
IncludeDirs["glslang"] = "%{ThirdPartyLocation}/glslang"
IncludeDirs["SPIRVCross"] = "%{ThirdPartyLocation}/SPIRV-Cross"
if (EngineFeatures.SupportVulkan) then
    IncludeDirs["Vulkan"] = "%{VULKAN_SDK}/Include/"
end

LibDirs = {}
LibDirs.Vulkan = VULKAN_SDK.."/Lib"
--------------------------------------------------------------------------------------------------------------------