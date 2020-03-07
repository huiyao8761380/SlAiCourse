// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SlAiTypes.h"

#include "SlAiPlayerCharacter.generated.h"

UCLASS()
class SLAICOURSE_API ASlAiPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASlAiPlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//�޸��ӽ�
	void ChangeView(EGameViewMode::Type NewGameView);

	//�޸ĵ�ǰ���ֳ���Ʒ
	void ChangeHandObject(TSubclassOf<AActor> HandObjectClass);

	//�޸��ֳ���Ʒ����ײ����Ƿ���
	void ChangeHandObjectDetect(bool IsOpen);

	//�Ƿ���Ⱦ������Ʒ,��Anim���е���
	void RenderHandObject(bool IsRender);

	//������Ʒ
	void PlayerThrowObject(int ObjectID, int Num);

	//�����Ƿ��пռ�
	bool IsPackageFree(int ObjectID);

	//�����Ʒ������
	void AddPackageObject(int ObjectID);

	//���궫�����õ��¼�,��Anim���е���
	void EatUpEvent();

	//��ȡ�Ƿ��Ѿ�����
	bool IsPlayerDead();

	//�����˺�
	void AcceptDamage(int DamageVal);

	//��ȡ�����λ��
	FVector GetCameraPos();


	//������������,���ز���ʱ��
	float PlayDeadAnim();

public:

	UPROPERTY(VisibleDefaultsOnly, Category = "SlAi")
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleDefaultsOnly, Category = "SlAi")
		class UCameraComponent* ThirdCamera;

	UPROPERTY(VisibleDefaultsOnly, Category = "SlAi")
		UCameraComponent* FirstCamera;

	//��ҿ�����ָ��
	class ASlAiPlayerController* SPController;

	//��ǰ���ӽ�ģ��
	EGameViewMode::Type GameView;

	//�ϰ�����״̬
	EUpperBody::Type UpperType;

	//�Ƿ������л��ӽ�
	bool IsAllowSwitch;

	//�Ƿ���ס����
	bool IsInputLocked;

	//�Ƿ��ڹ���
	bool IsAttack;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:

	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUpAtRate(float Value);
	void Turn(float Value);
	void TurnAtRate(float Value);
	void OnStartJump();
	void OnStopJump();
	void OnStartRun();
	void OnStopRun();


private:

	//��һ�˳ƵĹ���ģ��
	UPROPERTY(VisibleDefaultsOnly, Category = "SlAi")
		USkeletalMeshComponent* MeshFirst;

	//������Ʒ
	UPROPERTY(VisibleDefaultsOnly, Category = "SlAi")
		class UChildActorComponent* HandObject;

	//��ת����
	float BaseLookUpRate;
	float BaseTurnRate;

	//����������Դ
	class UAnimationAsset* AnimDead;


	
};
