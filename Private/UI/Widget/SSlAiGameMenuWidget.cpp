// Fill out your copyright notice in the Description page of Project Settings.

#include "SSlAiGameMenuWidget.h"
#include "SlateOptMacros.h"
#include "SBox.h"
#include "SSlAiGameOptionWidget.h"
#include "SBoxPanel.h"
#include "SlAiStyle.h"
#include "SlAiGameWidgetStyle.h"
#include "STextBlock.h"


#include "SlAiDataHandle.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "SlAiPlayerController.h"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSlAiGameMenuWidget::Construct(const FArguments& InArgs)
{

	//��ȡGameStyle
	GameStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiGameStyle>("BPSlAiGameStyle");

	ChildSlot
		[
			SAssignNew(RootBox, SBox)
			.WidthOverride(600.f)
		.HeightOverride(400.f)
		.Padding(FMargin(50.f))
		[
			SAssignNew(VertBox, SVerticalBox)
		]


		];

	InitializeWidget();

}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSlAiGameMenuWidget::InitializeWidget()
{
	//��ʼ����������
	MenuItemList.Add(
		SNew(SButton)
		.OnClicked(this, &SSlAiGameMenuWidget::OptionEvent)
		.HAlign(HAlign_Center).VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.Text(NSLOCTEXT("SlAiGame", "GameOption", "GameOption"))
		.Font(GameStyle->Font_30)
		]
	);

	MenuItemList.Add(
		SAssignNew(SaveGameButton, SButton)
		.OnClicked(this, &SSlAiGameMenuWidget::SaveGameEvent)
		.HAlign(HAlign_Center).VAlign(VAlign_Center)
		[
			//������һ������
			SAssignNew(SaveGameText, STextBlock)
			.Text(NSLOCTEXT("SlAiGame", "SaveGame", "SaveGame"))
		.Font(GameStyle->Font_30)
		]
	);

	MenuItemList.Add(
		SNew(SButton)
		.OnClicked(this, &SSlAiGameMenuWidget::QuitGameEvent)
		.HAlign(HAlign_Center).VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.Text(NSLOCTEXT("SlAiGame", "QuitGame", "QuitGame"))
		.Font(GameStyle->Font_30)
		]
	);

	//��ʼ����Ϸ��������
	OptionItemList.Add(
		SNew(SSlAiGameOptionWidget)
		.ChangeCulture(this, &SSlAiGameMenuWidget::ChangeCulture)
		.ChangeVolume(this, &SSlAiGameMenuWidget::ChangeVolume)
	);
	OptionItemList.Add(
		SNew(SButton)
		.OnClicked(this, &SSlAiGameMenuWidget::GoBackEvent)
		.HAlign(HAlign_Center).VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.Text(NSLOCTEXT("SlAiGame", "GoBack", "GoBack"))
		.Font(GameStyle->Font_30)
		]
	);

	//��ʼ��һ���˳���Ϸ��ť
	SAssignNew(QuitGameButton, SButton)
		.OnClicked(this, &SSlAiGameMenuWidget::QuitGameEvent)
		.HAlign(HAlign_Center).VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.Text(NSLOCTEXT("SlAiGame", "QuitGame", "QuitGame"))
		.Font(GameStyle->Font_30)
		];

	//���˵���ť���
	for (TArray<TSharedPtr<SCompoundWidget>>::TIterator It(MenuItemList); It; ++It)
	{
		VertBox->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.Padding(10.f)
			.FillHeight(1.f)
			[
				(*It)->AsShared()
			];
	}

}

FReply SSlAiGameMenuWidget::OptionEvent()
{
	//���
	VertBox->ClearChildren();
	//�����ÿؼ����
	VertBox->AddSlot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(10.f)
		.FillHeight(3.2f)
		[
			OptionItemList[0]->AsShared()
		];
	VertBox->AddSlot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(10.f)
		.FillHeight(1.f)
		[
			OptionItemList[1]->AsShared()
		];
	//���ø߶�
	RootBox->SetHeightOverride(520.f);
	return FReply::Handled();
}

FReply SSlAiGameMenuWidget::SaveGameEvent()
{
	//����浵
	SaveGameDele.ExecuteIfBound();
	//����SaveGame��ť������
	SaveGameButton->SetVisibility(EVisibility::HitTestInvisible);
	//�޸Ĵ浵����
	SaveGameText->SetText(NSLOCTEXT("SlAiGame", "SaveCompleted", "SaveCompleted"));
	//����
	return FReply::Handled();
}

FReply SSlAiGameMenuWidget::QuitGameEvent()
{
	Cast<ASlAiPlayerController>(UGameplayStatics::GetPlayerController(GWorld, 0))->ConsoleCommand("quit");
	return FReply::Handled();
}

FReply SSlAiGameMenuWidget::GoBackEvent()
{
	//���
	VertBox->ClearChildren();
	//���˵���ť���
	for (TArray<TSharedPtr<SCompoundWidget>>::TIterator It(MenuItemList); It; ++It)
	{
		VertBox->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.Padding(10.f)
			.FillHeight(1.f)
			[
				(*It)->AsShared()
			];
	}
	//���ø߶�
	RootBox->SetHeightOverride(400.f);
	return FReply::Handled();
}

void SSlAiGameMenuWidget::ChangeCulture(ECultureTeam Cultrue)
{
	SlAiDataHandle::Get()->ChangeLocalizationCulture(Cultrue);
}

void SSlAiGameMenuWidget::ChangeVolume(const float MusicVolume, const float SoundVolume)
{
	SlAiDataHandle::Get()->ResetGameVolume(MusicVolume, SoundVolume);
}

void SSlAiGameMenuWidget::GameLose()
{
	//���
	VertBox->ClearChildren();

	VertBox->AddSlot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(10.f)
		.FillHeight(1.f)
		[
			QuitGameButton->AsShared()
		];

	//���ø߶�
	RootBox->SetHeightOverride(200.f);
}

void SSlAiGameMenuWidget::ResetMenu()
{
	//���
	VertBox->ClearChildren();
	//���˵���ť���
	for (TArray<TSharedPtr<SCompoundWidget>>::TIterator It(MenuItemList); It; ++It)
	{
		VertBox->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.Padding(10.f)
			.FillHeight(1.f)
			[
				(*It)->AsShared()
			];
	}
	//���ø߶�
	RootBox->SetHeightOverride(400.f);
	//���ô浵��ť
	SaveGameButton->SetVisibility(EVisibility::Visible);
	//�޸Ĵ浵����
	SaveGameText->SetText(NSLOCTEXT("SlAiGame", "SaveGame", "SaveGame"));
}
