// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class WaveGame_Project : ModuleRules
{
	public WaveGame_Project(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		PublicIncludePaths.AddRange(new string[] { "WaveGame_Project" });
	}
}
