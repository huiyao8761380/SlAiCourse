// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/SlAiEnemyTaskBase.h"
#include "SlAiEnemyTaskDefence.generated.h"

/**
 * 
 */
UCLASS()
class SLAICOURSE_API USlAiEnemyTaskDefence : public USlAiEnemyTaskBase
{
	GENERATED_BODY()
	
		// ��дִ�к���
		virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	//��д������ֹ����
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	//�����������¼�
	void OnAnimationTimerDone();

protected:

	UPROPERTY(EditAnywhere, Category = "Blackboard")
		struct FBlackboardKeySelector IsDefenceFinish;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
		struct FBlackboardKeySelector PlayerPawn;
	
	//���������������ί��
	FTimerHandle TimerHandle;
	
};