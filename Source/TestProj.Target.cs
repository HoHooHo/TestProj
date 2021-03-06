// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class TestProjTarget : TargetRules
{
	public TestProjTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
        //ExtraModuleNames.Add("TestProj");
        ExtraModuleNames.AddRange(new string[] { "TestProj", "TestGC" });
	}
}
