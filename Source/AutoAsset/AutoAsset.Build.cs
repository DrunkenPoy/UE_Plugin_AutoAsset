using UnrealBuildTool;

public class AutoAsset : ModuleRules
{
    public AutoAsset(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateDependencyModuleNames.AddRange(new[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "UnrealEd",
            "EditorSubsystem",
            "AssetTools",
            "Blutility",
            "UMGEditor"
        });
    }
}