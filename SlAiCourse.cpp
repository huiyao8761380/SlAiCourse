// Fill out your copyright notice in the Description page of Project Settings.

#include "SlAiCourse.h"
#include "Modules/ModuleManager.h"
#include "SlAiStyle.h"





void FSlAiCourceModule::StartupModule()
{
	//��ʼ����ʽ
	FSlateStyleRegistry::UnRegisterSlateStyle(SlAiStyle::GetStyleSetName());
	SlAiStyle::Initialze();
}

void FSlAiCourceModule::ShutdownModule()
{
	SlAiStyle::ShutDown();
}

IMPLEMENT_PRIMARY_GAME_MODULE(FSlAiCourceModule, SlAiCourse, "SlAiCourse");
