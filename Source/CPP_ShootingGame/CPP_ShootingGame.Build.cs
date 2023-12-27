// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CPP_ShootingGame : ModuleRules
{
	public CPP_ShootingGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG" });
	}
}
