// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class SLAICOURSE_API SSlAiEnemyHPWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSlAiEnemyHPWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	//�ṩ�����˽�ɫ������
	void ChangeHP(float HP);

private:

	TSharedPtr<class SProgressBar> HPBar;

	//�����ɫ
	FLinearColor ResultColor;

};
