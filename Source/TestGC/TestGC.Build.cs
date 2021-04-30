// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

// This module must be loaded "PreLoadingScreen" in the .uproject file, otherwise it will not hook in time!

public class TestGC : ModuleRules
{
	public TestGC(ReadOnlyTargetRules Target)
		: base(Target)
	{
		
		//The path for the header files
		PublicIncludePaths.AddRange(new string[] { "TestGC/Public" });

		//The path for the source files
		PrivateIncludePaths.AddRange(new string[] { "TestGC/Private" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"Engine"
			}
		);
	}
}
