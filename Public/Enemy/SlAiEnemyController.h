// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SlAiEnemyController.generated.h"

/**
 * 
 */
UCLASS()
class SLAICOURSE_API ASlAiEnemyController : public AAIController
{
	GENERATED_BODY()
	
public:

	ASlAiEnemyController();

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

	virtual void Tick(float DeltaTime) override;

	//��ȡ��ҵ�λ��
	FVector GetPlayerLocation() const;

	//����Ƿ��Ѿ�����
	bool IsPlayerDead();

	//���������,��Character��OnSeePlayer����
	void OnSeePlayer();

	//��ʧ��Ҷ�λ
	void LoosePlayer();

	//�ж�����Ƿ���Զ��
	bool IsPlayerAway();

	//��ȡ���ָ��
	UObject* GetPlayerPawn();

	//���߿������������
	void ResetProcess(bool IsFinish);

	//�����˺�,����ʣ������ֵ
	void UpdateDamageRatio(float HPRatioVal);

	//����˺�״̬
	void FinishStateHurt();

	//��ɷ���״̬
	void FinishStateDefence();

	//����
	void EnemyDead();

public:

	//�Ƿ����������
	bool IsLockPlayer;


protected:

	virtual void BeginPlay() override;


private:

	//����״̬����
	void UpdateStatePama();

private:

	//��ҵ�ָ��
	class ASlAiPlayerCharacter* SPCharacter;

	//���˽�ɫָ��
	class ASlAiEnemyCharacter* SECharacter;
	
	class UBlackboardComponent* BlackboardComp;

	class UBehaviorTreeComponent* BehaviorComp;

	//��������ҵľ�������,����8��,ÿ�������һ��
	TArray<float> EPDisList;

	//ʱ��ί�о��
	FTimerHandle EPDisHandle;

	//Ѫֵ
	float HPRatio;

	//�Ƿ�����������״̬
	bool IsAllowHurt;

	//���˼�ʱ��
	float HurtTimeCount;

};
