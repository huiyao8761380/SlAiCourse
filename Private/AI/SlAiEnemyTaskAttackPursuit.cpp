// Fill out your copyright notice in the Description page of Project Settings.

#include "SlAiEnemyTaskAttackPursuit.h"
#include "SlAiEnemyController.h"
#include "SlAiEnemyCharacter.h"
#include "NavigationSystem/Public/NavigationSystem.h"
//#include "NavigationSystem/Public/NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TimerManager.h"



EBTNodeResult::Type USlAiEnemyTaskAttackPursuit::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//�����ʼ�����˲������ɹ�,ֱ�ӷ���ʧ��
	if (!InitEnemyElement(OwnerComp)) return EBTNodeResult::Failed;
	//����׷������
	float AttackDuration = SECharacter->PlayAttackAction(EEnemyAttackType::EA_Pursuit);
	//�����ٶ�Ϊ600,��С�����
	SECharacter->SetMaxSpeed(600.f);
	//���ò���
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(IsActionFinish.SelectedKeyName, false);
	//����¼�ί��
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &USlAiEnemyTaskAttackPursuit::OnAnimationTimerDone);
	//ע�ᵽ�¼�������
	SEController->GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, AttackDuration, false);
	return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type USlAiEnemyTaskAttackPursuit::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//�����ʼ�����˲������ɹ������¼����û�м���,ֱ�ӷ���
	if (!InitEnemyElement(OwnerComp) || !TimerHandle.IsValid()) return EBTNodeResult::Aborted;
	//ж��ʱ��ί��
	SEController->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	//����
	return EBTNodeResult::Aborted;
}

void USlAiEnemyTaskAttackPursuit::OnAnimationTimerDone()
{
	//���ö������
	if (SEController) SEController->ResetProcess(true);
	//�޸��ٶȻ�300
	if (SECharacter) SECharacter->SetMaxSpeed(300.f);
}
