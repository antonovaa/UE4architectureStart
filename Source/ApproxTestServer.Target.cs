// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

[SupportedPlatforms(UnrealPlatformClass.Server)]
public class ApproxTestServerTarget : TargetRules
{
	public ApproxTestServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;
		bUsesSteam = false;
		ExtraModuleNames.Add("ApproxTest"); //+
	}
}
