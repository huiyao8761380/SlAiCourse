// Fill out your copyright notice in the Description page of Project Settings.

#include "SlAiThirdPlayerAnim.h"
#include "SlAiPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SlAiHelper.h"
#include "Animation/AnimMontage.h"
#include "ConstructorHelpers.h"


USlAiThirdPlayerAnim::USlAiThirdPlayerAnim()
{

	//����Դ��Montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PlayerHitMon(TEXT("AnimMontage'/Game/Res/PolygonAdventure/Mannequin/Player/Animation/UpperBody/PlayerHitMontage.PlayerHitMontage'"));
	PlayerHitMontage = PlayerHitMon.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PlayerEatMon(TEXT("AnimMontage'/Game/Res/PolygonAdventure/Mannequin/Player/Animation/UpperBody/PlayerEatMontage.PlayerEatMontage'"));
	PlayerEatMontage = PlayerEatMon.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PlayerFightMon(TEXT("AnimMontage'/Game/Res/PolygonAdventure/Mannequin/Player/Animation/UpperBody/PlayerFightMontage.PlayerFightMontage'"));
	PlayerFightMontage = PlayerFightMon.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PlayerPunchMon(TEXT("AnimMontage'/Game/Res/PolygonAdventure/Mannequin/Player/Animation/UpperBody/PlayerPunchMontage.PlayerPunchMontage'"));
	PlayerPunchMontage = PlayerPunchMon.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PlayerPickUpMon(TEXT("AnimMontage'/Game/Res/PolygonAdventure/Mannequin/Player/Animation/UpperBody/PlayerPickUpMontage.PlayerPickUpMontage'"));
	PlayerPickUpMontage = PlayerPickUpMon.Object;


	//�����Լ��������˳��ǵ����˳�
	GameView = EGameViewMode::Third;
	//������ֵ
	Direction = 0.f;
	//���Ƿ��ڿ��и�ֵ
	IsInAir = false;

}

void USlAiThirdPlayerAnim::UpdateParameter()
{
	Super::UpdateParameter();
	//���������ֱ�ӷ���,�����ָ������ж�
	if (!SPCharacter) return;
	//��ȡ�Ƿ�����Ծ
	IsInAir = SPCharacter->GetCharacterMovement()->IsFalling();
	//�ٶȳ����Yaw��ת��ȥActor������ʵ��������������Yaw��ת�õ���Եķ���
	float PreDir = SPCharacter->GetVelocity().ToOrientationRotator().Yaw - SPCharacter->GetActorRotation().Yaw;

	//��������벻���׿���ȥ����˼·����һЩ
	if (PreDir > 180.f) PreDir -= 360.f;
	if (PreDir < -180.f) PreDir += 360.f;
	Direction = PreDir;


}

