// Fill out your copyright notice in the Description page of Project Settings.

#include "SlAiGameHUD.h"
#include "Engine/Engine.h"
#include "SlateBasics.h"
#include "Kismet/GameplayStatics.h"

#include "SSlAiGameHUDWidget.h"
#include "SSlAiShortcutWidget.h"
#include "SSlAiRayInfoWidget.h"
#include "SSlAiPointerWidget.h"
#include "SSlAiPlayerStateWidget.h"
#include "SSlAiPackageWidget.h"
#include "SSlAiMiniMapWidget.h"
#include "SSlAiGameMenuWidget.h"

#include "SlAiPlayerController.h"
#include "SlAiPlayerState.h"
#include "SlAiGameMode.h"


ASlAiGameHUD::ASlAiGameHUD()
{
	if (GEngine && GEngine->GameViewport) {
		SAssignNew(GameHUDWidget, SSlAiGameHUDWidget);
		GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(GameHUDWidget.ToSharedRef()));
	}
}

void ASlAiGameHUD::BeginPlay()
{
	Super::BeginPlay();

	GM = Cast<ASlAiGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GM) return;
	//��ȷ��Ҫ���õ�������Ѿ�ʵ��
	GM->InitGamePlayModule();
	//��ע����������
	GameHUDWidget->ShortcutWidget->RegisterShortcutContainer.BindUObject(GM->SPState, &ASlAiPlayerState::RegisterShortcutContainer);
	//��ע��������Ϣ�ı��¼�
	GameHUDWidget->RayInfoWidget->RegisterRayInfoEvent.BindUObject(GM->SPState, &ASlAiPlayerState::RegisterRayInfoEvent);
	//���޸�׼��ί��
	GM->SPController->UpdatePointer.BindRaw(GameHUDWidget->PointerWidget.Get(), &SSlAiPointerWidget::UpdatePointer);
	//�󶨸������״̬��ί��
	GM->SPState->UpdateStateWidget.BindRaw(GameHUDWidget->PlayerStateWidget.Get(), &SSlAiPlayerStateWidget::UpdateStateWidget);
    //����ʾUIί��
	GM->SPController->ShowGameUI.BindRaw(GameHUDWidget.Get(), &SSlAiGameHUDWidget::ShowGameUI);
	//��ʼ���������������������
	GM->InitPackageManager.BindRaw(GameHUDWidget->PackageWidget.Get(), &SSlAiPackageWidget::InitPackageManager);
	//��ע��С��ͼ��ͼί��
	GM->RegisterMiniMap.BindRaw(GameHUDWidget->MiniMapWidget.Get(), &SSlAiMiniMapWidget::RegisterMiniMap);
	//�󶨸���С��ͼ����ί��
	GM->UpdateMapData.BindRaw(GameHUDWidget->MiniMapWidget.Get(), &SSlAiMiniMapWidget::UpdateMapData);
	//������Ϸ�¼���
	GameHUDWidget->GameMenuWidget->SaveGameDele.BindUObject(GM, &ASlAiGameMode::SaveGame);
}


