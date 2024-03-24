// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FirstGame : ModuleRules
{
	public FirstGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			// InitialModules
			"Core", "CoreUObject", "Engine", "InputCore",
		
			// Enhanced Input
			"EnhancedInput",

			// AI
			"NavigationSystem",
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		
	}
}
