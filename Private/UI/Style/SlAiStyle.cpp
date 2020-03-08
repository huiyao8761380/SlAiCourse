// Fill out your copyright notice in the Description page of Project Settings.

#include "SlAiStyle.h"
#include "SlateGameResources.h"

TSharedPtr<FSlateStyleSet> SlAiStyle::SlAiStyleInstance = NULL;

void SlAiStyle::Initialze()
{
	if (!SlAiStyleInstance.IsValid()) {
		SlAiStyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*SlAiStyleInstance);//注册
	}
}

FName SlAiStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("SlAiStyle"));//ftext本地化 FName系统 Fstring字符串调用
	return StyleSetName;

}

void SlAiStyle::ShutDown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*SlAiStyleInstance);//取消注册
	ensure(SlAiStyleInstance.IsUnique());//确保是唯一的
	SlAiStyleInstance.Reset();
}

const ISlateStyle& SlAiStyle::Get()
{
	return *SlAiStyleInstance;//获取 但可能是空 
}

TSharedRef<class FSlateStyleSet> SlAiStyle::Create()//创建共享指针
{//继承SlAiMenuWidgetStyle样式 在路径找样式蓝图
	TSharedRef<FSlateStyleSet> StyleRef = FSlateGameResources::New(SlAiStyle::GetStyleSetName(), "/Game/UI/Style", "/Game/UI/Style");
	StyleRef->Set("MenuItemFont", FSlateFontInfo("Roboto-Regular.ttf", 50));//设置字体样式
	return StyleRef;
}
