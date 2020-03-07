// Fill out your copyright notice in the Description page of Project Settings.

#include "SlAiGameMode.h"
#include "SlAiPlayerController.h"
#include "SlAiPlayerCharacter.h"
#include "SlAiPlayerState.h"
#include "SlAiGameHUD.h"


#include "SlAiDataHandle.h"
#include "SlAiHelper.h"
#include "SlAiGameInstance.h"
#include "Kismet/GameplayStatics.h"

#include "SlAiPackageManager.h"
#include "SlAiSceneCapture2D.h"
#include "SlAiEnemyCharacter.h"
#include "EngineUtils.h"

#include "Sound/SoundWave.h"
#include "SlAiSaveGame.h"
#include "SlAiResourceRock.h"
#include "SlAiResourceTree.h"
#include "SlAiPickupStone.h"
#include "SlAiPickupWood.h"



ASlAiGameMode::ASlAiGameMode()
{
	//��������tick����
	PrimaryActorTick.bCanEverTick = true;


	//�������
	HUDClass = ASlAiGameHUD::StaticClass();

	PlayerControllerClass = ASlAiPlayerController::StaticClass();
	PlayerStateClass = ASlAiPlayerState::StaticClass();
	DefaultPawnClass = ASlAiPlayerCharacter::StaticClass();


	//��ʼû�г�ʼ������
	IsInitPackage = false;
	//С��ͼ��û����
	IsCreateMiniMap = false;
	//��ʼ���ò���Ҫ���ش浵
	IsNeedLoadRecord = false;

}

void ASlAiGameMode::Tick(float DeltaSeconds)
{
	//��ʼ�������С��ͼ�����
	InitializeMiniMapCamera();

	//���������ش浵,���ڳ�ʼ������������Ϊ���ڵڶ�֡��ִ��
	LoadRecordPackage();

	//��ʼ������
	InitializePackage();
}

void ASlAiGameMode::InitGamePlayModule()
{
	//��������
	SPController = Cast<ASlAiPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	SPCharacter = Cast<ASlAiPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	SPState = Cast<ASlAiPlayerState>(SPController->PlayerState);
}

void ASlAiGameMode::SaveGame()
{
	//����浵����Default,�����б���
	if (SlAiDataHandle::Get()->RecordName.Equals(FString("Default"))) return;

	//����һ���µĴ浵
	USlAiSaveGame* NewRecord = Cast<USlAiSaveGame>(UGameplayStatics::CreateSaveGameObject(USlAiSaveGame::StaticClass()));

	//�Դ浵���и�ֵ
	//�������λ�ú�Ѫ��
	NewRecord->PlayerLocation = SPCharacter->GetActorLocation();
	SPState->SaveState(NewRecord->PlayerHP, NewRecord->PlayerHunger);

	//ѭ�����õ���
	for (TActorIterator<ASlAiEnemyCharacter> EnemyIt(GetWorld()); EnemyIt; ++EnemyIt)
	{
		NewRecord->EnemyLoaction.Add((*EnemyIt)->GetActorLocation());
		NewRecord->EnemyHP.Add((*EnemyIt)->GetHP());
	}

	//ѭ��������ʯ
	for (TActorIterator<ASlAiResourceRock> RockIt(GetWorld()); RockIt; ++RockIt)
	{
		NewRecord->ResourceRock.Add((*RockIt)->GetActorLocation());
	}

	//ѭ��������ľ
	for (TActorIterator<ASlAiResourceTree> TreeIt(GetWorld()); TreeIt; ++TreeIt) {
		NewRecord->ResourceTree.Add((*TreeIt)->GetActorLocation());
	}

	//ѭ������ʰȡ��Ʒʯͷ
	for (TActorIterator<ASlAiPickupStone> StoneIt(GetWorld()); StoneIt; ++StoneIt) {
		NewRecord->PickupStone.Add((*StoneIt)->GetActorLocation());
	}

	//ѭ������ʰȡ��Ʒľͷ
	for (TActorIterator<ASlAiPickupWood> WoodIt(GetWorld()); WoodIt; ++WoodIt) {
		NewRecord->PickupWood.Add((*WoodIt)->GetActorLocation());
	}

	//��ȡ��������
	SlAiPackageManager::Get()->SaveData(NewRecord->InputIndex, NewRecord->InputNum, NewRecord->NormalIndex, NewRecord->NormalNum, NewRecord->ShortcutIndex, NewRecord->ShortcutNum);

	//�鿴�Ƿ��Ѿ��д浵����
	if (UGameplayStatics::DoesSaveGameExist(SlAiDataHandle::Get()->RecordName, 0)) {
		//�еĻ���ɾ��
		UGameplayStatics::DeleteGameInSlot(SlAiDataHandle::Get()->RecordName, 0);
	}
	//����浵
	UGameplayStatics::SaveGameToSlot(NewRecord, SlAiDataHandle::Get()->RecordName, 0);

	//�鿴json�Ƿ��Ѿ�������浵
	bool IsRecordExist = false;
	for (TArray<FString>::TIterator It(SlAiDataHandle::Get()->RecordDataList); It; ++It)
	{
		//ֻҪ��һ����ͬ,������
		if ((*It).Equals(SlAiDataHandle::Get()->RecordName)) {
			IsRecordExist = true;
			break;
		}
	}
	//����浵������,�����ݹ��������Ӵ浵��json
	if (!IsRecordExist) SlAiDataHandle::Get()->AddNewRecord();
}


void ASlAiGameMode::BeginPlay()
{
	//��ʼ����Ϸ����
	SlAiDataHandle::Get()->InitializeGameData();

	if (!SPController) InitGamePlayModule();

	//���ű�������
	USoundWave* BGMusic = LoadObject<USoundWave>(NULL, TEXT("SoundWave'/Game/Res/Sound/GameSound/GameBG.GameBG'"));
	BGMusic->bLooping = true;
	UGameplayStatics::PlaySound2D(GetWorld(), BGMusic, 0.1f);


	LoadRecord();
}

void ASlAiGameMode::InitializePackage()
{
	if (IsInitPackage) return;

	//��PackageWidget��ʼ������������
	InitPackageManager.ExecuteIfBound();
	//�󶨶�����Ʒί��
	SlAiPackageManager::Get()->PlayerThrowObject.BindUObject(SPCharacter, &ASlAiPlayerCharacter::PlayerThrowObject);
	//���޸Ŀ������Ϣί��
	SlAiPackageManager::Get()->ChangeHandObject.BindUObject(SPState, &ASlAiPlayerState::ChangeHandObject);
	IsInitPackage = true;
}

void ASlAiGameMode::InitializeMiniMapCamera()
{
	//���������������ڲ��������Ѿ�����
	if (!IsCreateMiniMap && GetWorld())
	{
		//����С��ͼ�����
		MiniMapCamera = GetWorld()->SpawnActor<ASlAiSceneCapture2D>(ASlAiSceneCapture2D::StaticClass());
		//����ί�и�MiniMapWidget������Ⱦ��MiniMapTex
		RegisterMiniMap.ExecuteIfBound(MiniMapCamera->GetMiniMapTex());
		//���޸�С��ͼ��Ұ��ί��
		SPController->UpdateMiniMapWidth.BindUObject(MiniMapCamera, &ASlAiSceneCapture2D::UpdateMiniMapWidth);
		//�����Ѿ�����С��ͼ
		IsCreateMiniMap = true;
	}

	//���С��ͼ�Ѿ�����
	if (IsCreateMiniMap)
	{
		//ÿ֡����С��ͼ�������λ�ú���ת
		MiniMapCamera->UpdateTransform(SPCharacter->GetActorLocation(), SPCharacter->GetActorRotation());

		TArray<FVector2D> EnemyPosList;
		TArray<bool> EnemyLockList;
		TArray<float> EnemyRotateList;

		//��ȡ�����еĵ���
		for (TActorIterator<ASlAiEnemyCharacter> EnemyIt(GetWorld()); EnemyIt; ++EnemyIt)
		{
			FVector EnemyPos = FVector((*EnemyIt)->GetActorLocation().X - SPCharacter->GetActorLocation().X, (*EnemyIt)->GetActorLocation().Y - SPCharacter->GetActorLocation().Y, 0.f);
			EnemyPos = FQuat(FVector::UpVector, FMath::DegreesToRadians(-SPCharacter->GetActorRotation().Yaw - 90.f)) * EnemyPos;
			EnemyPosList.Add(FVector2D(EnemyPos.X, EnemyPos.Y));

			EnemyLockList.Add((*EnemyIt)->IsLockPlayer());
			EnemyRotateList.Add((*EnemyIt)->GetActorRotation().Yaw - SPCharacter->GetActorRotation().Yaw);
		}


		//ÿ֡����С��ͼ�ķ�������λ��
		UpdateMapData.ExecuteIfBound(SPCharacter->GetActorRotation(), MiniMapCamera->GetMapSize(), &EnemyPosList, &EnemyLockList, &EnemyRotateList);

	}

}

void ASlAiGameMode::LoadRecord()
{
	//���RecordNameΪ��,ֱ��renturn
	if (SlAiDataHandle::Get()->RecordName.IsEmpty() || SlAiDataHandle::Get()->RecordName.Equals(FString("Default"))) return;
	//ѭ�����浵�Ƿ��Ѿ�����
	for (TArray<FString>::TIterator It(SlAiDataHandle::Get()->RecordDataList); It; ++It) {
		//�����һ��һ����ֱ������Ϊtrue,����ֱ������ѭ��
		if ((*It).Equals(SlAiDataHandle::Get()->RecordName)) {
			IsNeedLoadRecord = true;
			break;
		}
	}
	//�����Ҫ����,���д浵�ļ���,����浵����,���м���
	if (IsNeedLoadRecord && UGameplayStatics::DoesSaveGameExist(SlAiDataHandle::Get()->RecordName, 0))
	{
		GameRecord = Cast<USlAiSaveGame>(UGameplayStatics::LoadGameFromSlot(SlAiDataHandle::Get()->RecordName, 0));
	}
	else {
		IsNeedLoadRecord = false;
	}
	//�����Ҫ���ز��Ҵ浵����
	if (IsNeedLoadRecord && GameRecord)
	{
		//�������λ�ú�Ѫ��
		SPCharacter->SetActorLocation(GameRecord->PlayerLocation);
		SPState->LoadState(GameRecord->PlayerHP, GameRecord->PlayerHunger);

		//ѭ�����õ���
		int EnemyCount = 0;
		for (TActorIterator<ASlAiEnemyCharacter> EnemyIt(GetWorld()); EnemyIt; ++EnemyIt) {
			if (EnemyCount < GameRecord->EnemyLoaction.Num())
			{
				(*EnemyIt)->SetActorLocation(GameRecord->EnemyLoaction[EnemyCount]);
				(*EnemyIt)->LoadHP(GameRecord->EnemyHP[EnemyCount]);
			}
			else {
				//�������������һ֡����
				(*EnemyIt)->IsDestroyNextTick = true;
			}
			++EnemyCount;
		}


		//ѭ��������ʯ
		int RockCount = 0;
		for (TActorIterator<ASlAiResourceRock> RockIt(GetWorld()); RockIt; ++RockIt) {
			if (RockCount < GameRecord->ResourceRock.Num()) {
				(*RockIt)->SetActorLocation(GameRecord->ResourceRock[RockCount]);
			}
			else {
				//���������Դ��һ֡����
				(*RockIt)->IsDestroyNextTick = true;
			}
			++RockCount;
		}

		//ѭ��������ľ
		int TreeCount = 0;
		for (TActorIterator<ASlAiResourceTree> TreeIt(GetWorld()); TreeIt; ++TreeIt) {
			if (TreeCount < GameRecord->ResourceTree.Num()) {
				(*TreeIt)->SetActorLocation(GameRecord->ResourceTree[TreeCount]);
			}
			else {
				//���������Դ��һ֡����
				(*TreeIt)->IsDestroyNextTick = true;
			}
			++TreeCount;
		}

		//ѭ������ʰȡ��Ʒʯͷ
		int StoneCount = 0;
		for (TActorIterator<ASlAiPickupStone> StoneIt(GetWorld()); StoneIt; ++StoneIt) {
			if (StoneCount < GameRecord->PickupStone.Num()) {
				(*StoneIt)->SetActorLocation(GameRecord->PickupStone[StoneCount]);
			}
			else {
				//���������Դ��һ֡����
				(*StoneIt)->IsDestroyNextTick = true;
			}
			++StoneCount;
		}

		//ѭ������ʰȡ��Ʒľͷ
		int WoodCount = 0;
		for (TActorIterator<ASlAiPickupWood> WoodIt(GetWorld()); WoodIt; ++WoodIt) {
			if (WoodCount < GameRecord->PickupWood.Num()) {
				(*WoodIt)->SetActorLocation(GameRecord->PickupWood[WoodCount]);
			}
			else {
				//���������Դ��һ֡����
				(*WoodIt)->IsDestroyNextTick = true;
			}
			++WoodCount;
		}

	}

}

void ASlAiGameMode::LoadRecordPackage()
{
	//�������û�г�ʼ�����߲��ü��ش浵,ֱ�ӷ���
	if (!IsInitPackage || !IsNeedLoadRecord) return;

	if (IsNeedLoadRecord && GameRecord)
	{
		SlAiPackageManager::Get()->LoadRecord(&GameRecord->InputIndex, &GameRecord->InputNum, &GameRecord->NormalIndex, &GameRecord->NormalNum, &GameRecord->ShortcutIndex, &GameRecord->ShortcutNum);
	}
	//������ò��ü��ش浵��
	IsNeedLoadRecord = false;
}