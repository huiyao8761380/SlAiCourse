// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class SlAiCourse : ModuleRules
{
	public SlAiCourse(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "Slate",
            "SlateCore",
            "Json",//加json模块来保存菜单设置
            "JsonUtilities",
            "UMG",
            "AIModule",
            "GameplayTasks",
        });

		PrivateDependencyModuleNames.AddRange(new string[] {
       
        });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
