// Fill out your copyright notice in the Description page of Project Settings.

#include "SlAiEnemyTaskHurt.h"
#include "SlAiEnemyController.h"
#include "SlAiEnemyCharacter.h"
//#include "AI/Navigation/NavigationSystem.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TimerManager.h"



EBTNodeResult::Type USlAiEnemyTaskHurt::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//�����ʼ�����˲������ɹ�,ֱ�ӷ���ʧ��
	if (!InitEnemyElement(OwnerComp)) return EBTNodeResult::Failed;
	//�������˶���
	float ActionDuration = SECharacter->PlayHurtAction();
	//���õȴ�ʱ��
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(WaitTime.SelectedKeyName, ActionDuration);
	//����¼�ί��
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &USlAiEnemyTaskHurt::OnAnimationTimerDone);
	//ע�ᵽ�¼�������
	SEController->GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, ActionDuration, false);
	return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type USlAiEnemyTaskHurt::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//�����ʼ�����˲������ɹ������¼����û�м���,ֱ�ӷ���
	if(!InitEnemyElement(OwnerComp) || !TimerHandle.IsValid()) return EBTNodeResult::Aborted;
	//ж��ʱ��ί��
	SEController->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	//����
	return EBTNodeResult::Aborted;
}

void USlAiEnemyTaskHurt::OnAnimationTimerDone()
{
	//���߿������������
	SEController->FinishStateHurt();
}
