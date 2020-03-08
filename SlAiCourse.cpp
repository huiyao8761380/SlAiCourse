// Fill out your copyright notice in the Description page of Project Settings.

#include "SlAiCourse.h"
#include "Modules/ModuleManager.h"
#include "SlAiStyle.h"





void FSlAiCourceModule::StartupModule()
{
	//初始化样式
	FSlateStyleRegistry::UnRegisterSlateStyle(SlAiStyle::GetStyleSetName());//SlAiStyle文件里也有取消注册函数 它通过 获取样式名字 来调用 
	SlAiStyle::Initialze();
}

void FSlAiCourceModule::ShutdownModule()
{
	SlAiStyle::ShutDown();
}

IMPLEMENT_PRIMARY_GAME_MODULE(FSlAiCourceModule, SlAiCourse, "SlAiCourse");
