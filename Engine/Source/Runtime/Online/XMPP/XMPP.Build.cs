// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class XMPP : ModuleRules
{
	public XMPP(TargetInfo Target)
	{
		Definitions.Add("XMPP_PACKAGE=1");

        PrivateIncludePaths.AddRange(
			new string[] 
			{
				"Runtime/Online/XMPP/Private"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[] 
			{ 
				"Core",
				"Json"
			}
		);

		if (!UnrealBuildTool.UnrealBuildTool.RunningRocket())
		{
			if (Target.Platform == UnrealTargetPlatform.Win64 ||
				Target.Platform == UnrealTargetPlatform.Win32 ||
				Target.Platform == UnrealTargetPlatform.Mac)
			{
				AddThirdPartyPrivateStaticDependencies(Target, "WebRTC");	
			}
		}
	}
}
