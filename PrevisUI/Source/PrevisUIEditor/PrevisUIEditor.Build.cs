using UnrealBuildTool;

public class PrevisUIEditor : ModuleRules
{
	public PrevisUIEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Slate", "SlateCore", "PrevisUI", "EditorStyle", "MainFrame", "UnrealEd", "CinematicCamera", "ProceduralMeshComponent", "MediaAssets", "BlackmagicMedia", "MediaIOCore" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Slate", "SlateCore", "PrevisUI", "EditorStyle", "MainFrame", "UnrealEd", "CinematicCamera", "ProceduralMeshComponent", "MediaAssets", "BlackmagicMedia", "MediaIOCore" });
        

		// VR modules
		PrivateDependencyModuleNames.AddRange(new string[] { "HeadMountedDisplay", "SteamVR" });
	}
}
