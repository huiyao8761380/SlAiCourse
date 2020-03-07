// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SlAiTypes.h"

#include "SlAiEnemyCharacter.generated.h"

class UAnimationAsset;

UCLASS()
class SLAICOURSE_API ASlAiEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASlAiEnemyCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//ʵʱ����Ѫ���ĳ���,��Controller����,�������λ��
	void UpdateHPBarRotation(FVector SPLoaction);

	//�޸��ƶ��ٶ�
	void SetMaxSpeed(float Speed);

	//��ȡIdle�ȴ�ʱ��
	float GetIdleWaitTime();

	//���Ź�������,���ع���ʱ��
	float PlayAttackAction(EEnemyAttackType AttackType);


	//���ܹ���,Ҳ������дAPawn��TakeDamage����,���������鷳
	void AcceptDamage(int DamageVal);

	//�������˶���
	float PlayHurtAction();

	//��������
	void StartDefence();

	//ֹͣ����
	void StopDefence();

	//���ٺ���
	void DestroyEvent();

	//��ȡ��Ʒ��Ϣ
	FText GetInfoText() const;

	//�޸��ֳ���Ʒ����ײ����Ƿ���
	void ChangeWeaponDetect(bool IsOpen);

	//��ȡ�Ƿ��Ѿ����������
	bool IsLockPlayer();

	//����Ѫ��
	void LoadHP(float HPVal);

	//��ȡѪ��,������Ϸʱ����
	float GetHP();

public:

	//��ԴID
	int ResourceIndex;

	//�Ƿ���һ֡�����Լ�,��GameMode������Ϸ�浵ʱ��������
	bool IsDestroyNextTick;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//�����ʵ��亯��
	void CreateFlobObject();

protected:

	//�������
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UChildActorComponent* WeaponSocket;

	//���Ʋ��
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UChildActorComponent* SheildSocket;


	//Ѫ��
	UPROPERTY(EditAnywhere, Category = Mesh)
		class UWidgetComponent* HPBar;


	//���˸�֪
	UPROPERTY(EditAnywhere, Category = Mesh)
		class UPawnSensingComponent* EnemySense;

private:


	//�󶨵����˸�֪�ķ���
	UFUNCTION()
		void OnSeePlayer(APawn* PlayerChar);

	//Ѫ��UI����
	TSharedPtr<class SSlAiEnemyHPWidget> HPBarWidget;


	//����������
	class ASlAiEnemyController* SEController;

	//Ѫ��
	float HP;

	//��������
	class USlAiEnemyAnim* SEAnim;

	//����������Դ
	UAnimationAsset* AnimDead_I;
	UAnimationAsset* AnimDead_II;

	//����ʱ��ί��
	FTimerHandle DeadHandle;

};
