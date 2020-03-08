// Fill out your copyright notice in the Description page of Project Settings.

#include "SSlAiMenuHUDWidget.h"
#include "SlateOptMacros.h"
#include "SButton.h"
#include "SImage.h"
#include "SlAiStyle.h"
#include "SlAiMenuWidgetStyle.h"
#include "Engine/Engine.h"
#include "SDPIScaler.h"
#include "SSlAiMenuWidget.h"

//创建一个Widget的HUD蓝图，本代码类似它 联动SlAiMenuWidgetStyle 在UE4里指定图片

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSlAiMenuHUDWidget::Construct(const FArguments& InArgs)
{
	//获取编辑器的MenuStyle
	MenuStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiMenuStyle>("BPSlAiMenuStyle");
	//绑定缩放规则方法
	UIScaler.Bind(this, &SSlAiMenuHUDWidget::GetUIScaler);

	ChildSlot
		[
			SNew(SDPIScaler)//DPI窗口大小曲线
			.DPIScale(UIScaler)
		[
			SNew(SOverlay)

			+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SImage)
			.Image(&MenuStyle->MenuHUDBackgroundBrush)//创建MenuStyleHUD在/Game/UI/Style路径（SlAiStyle）在UE4里指定图片..
		]

	+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SAssignNew(MenuWidget, SSlAiMenuWidget)
		]



		]
		];

}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

float SSlAiMenuHUDWidget::GetUIScaler() const
{
	return GetViewportSize().Y / 1080.f;
}

FVector2D SSlAiMenuHUDWidget::GetViewportSize() const
{
	FVector2D Result(1920, 1080);
	if (GEngine && GEngine->GameViewport)
		GEngine->GameViewport->GetViewportSize(Result);
	return Result;
}


