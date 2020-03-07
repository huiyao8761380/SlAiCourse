// Fill out your copyright notice in the Description page of Project Settings.

#include "SSlAiPointerWidget.h"
#include "SlateOptMacros.h"
#include "SlAiStyle.h"
#include "SlAiGameWidgetStyle.h"
#include "SBox.h"
#include "SImage.h"
#include "Materials/MaterialInstanceDynamic.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSlAiPointerWidget::Construct(const FArguments& InArgs)
{

	//��ȡGameStyle
	GameStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiGameStyle>("BPSlAiGameStyle");
	//��ʼ����СΪ20
	CurrentSize = 20.f;
	IsAimed = false;

	ChildSlot
		[
			SAssignNew(RootBox, SBox)
			.WidthOverride(TAttribute<FOptionalSize>(this, &SSlAiPointerWidget::GetBoxWidget))
		.HeightOverride(TAttribute<FOptionalSize>(this, &SSlAiPointerWidget::GetBoxHeight))
		[
			SNew(SImage)
			.Image(&GameStyle->PointerBrush)
		]

		];


	//���ز���ʵ��
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> StaticPointerMaterialInstance(TEXT("MaterialInstanceConstant'/Game/Material/PointerMatInst.PointerMatInst'"));
	//ת��Ϊ��̬����ʵ��
	PointerMaterial = (UMaterialInstanceDynamic*)StaticPointerMaterialInstance.Object;


}

void SSlAiPointerWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	//ʵʱ�޸Ŀռ��С
	CurrentSize = IsAimed ? FMath::FInterpTo(CurrentSize, 130.f, InDeltaTime, 10.f) : FMath::FInterpTo(CurrentSize, 20.f, InDeltaTime, 10.f);
}

void SSlAiPointerWidget::UpdatePointer(bool IsAim, float Range)
{
	IsAimed = IsAim;
	PointerMaterial->SetScalarParameterValue(FName("Range"), Range);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION


FOptionalSize SSlAiPointerWidget::GetBoxWidget() const
{
	return FOptionalSize(CurrentSize);
}

FOptionalSize SSlAiPointerWidget::GetBoxHeight() const
{
	return FOptionalSize(CurrentSize);
}
