// Fill out your copyright notice in the Description page of Project Settings.

#include "SlAiEnemyTaskLocaESC.h"
#include "SlAiEnemyController.h"
#include "SlAiEnemyCharacter.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"



EBTNodeResult::Type USlAiEnemyTaskLocaESC::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//�����ʼ�����˲������ɹ�,ֱ�ӷ���ʧ��
	if (!InitEnemyElement(OwnerComp)) return EBTNodeResult::Failed;

	//��Χ��2000����
	const float ChaseRadius = 2000.f;
	//��ȡ���˵����֮�������
	FVector SPToSE = SECharacter->GetActorLocation() - SEController->GetPlayerLocation();
	//̽��������λ��
	const FVector ChaseOrigin = SECharacter->GetActorLocation();
	//���������λ��
	FVector DesLoc(0.f);
	//ʹ�õ���ϵͳ��ȡ�����
	UNavigationSystem::K2_GetRandomReachablePointInRadius(SEController, ChaseOrigin, DesLoc, ChaseRadius);
	//�����õ�λ�õ����˵���������ҵ����˵������ļнǴ���90��,���¶��巽��
	while(FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(SPToSE, DesLoc - SECharacter->GetActorLocation()))) > 90.f)
	{
		//ʹ�õ���ϵͳ���»�ȡ�����
		UNavigationSystem::K2_GetRandomReachablePointInRadius(SEController, ChaseOrigin, DesLoc, ChaseRadius);
	}
	//�޸��ٶ�Ϊ300
	SECharacter->SetMaxSpeed(300.f);
	//�޸�Ŀ�ĵ�
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(Destination.SelectedKeyName, DesLoc);
	//���سɹ�
	return EBTNodeResult::Succeeded;
}
