using UnrealBuildTool;

public class unreal_positional_editor : ModuleRules
{
	public unreal_positional_editor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "unreal_positional" });

		PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd", "EditorStyle", "Slate", "SlateCore" });

		PublicIncludePaths.AddRange(new string[] { @"unreal_positional_editor\Public", @"unreal_positional\positional\src" });
		PrivateIncludePaths.Add(@"unreal_positional_editor\Private");
	}
}
