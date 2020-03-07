// Fill out your copyright notice in the Description page of Project Settings.

#include "SlAiMenuHUD.h"
#include "SSlAiMenuHUDWidget.h"
#include "SlateBasics.h"


ASlAiMenuHUD::ASlAiMenuHUD()
{
	if (GEngine && GEngine->GameViewport) {
		//3创建widget 的两个方式：SAssignNew（共享指针/指针，类）	 SNew（类）
		SAssignNew(MenuHUDWidget, SSlAiMenuHUDWidget);
		//1SlAiMunuHUD 创建主界面去add viewpoint 类似蓝图
		GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(MenuHUDWidget.ToSharedRef()));
		//2addviewport调用SNew的方法创建SWeakWidget类名，把MenuHUDWidget赋给SNew
	}
}
