// Fill out your copyright notice in the Description page of Project Settings.

#include "SlAiEnemyTaskAttackFollow.h"
#include "SlAiEnemyController.h"
#include "SlAiEnemyCharacter.h"
#include "NavigationSystem/Public/NavigationSystem.h"
//#include "NavigationSystem/Public/NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"




EBTNodeResult::Type USlAiEnemyTaskAttackFollow::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//�����ʼ�����˲������ɹ�,ֱ�ӷ���ʧ��
	if (!InitEnemyElement(OwnerComp)) return EBTNodeResult::Failed;
	//��Χ��5
	const float ChaseRadius = 0.f;
	//��ȡ��ҵ�����֮��ĵ�λ����
	FVector SPToSE = SEController->GetPlayerLocation() - SECharacter->GetActorLocation();

	//��ȡ����
	float EPDistance = SPToSE.Size();

	//����������100.f,��ȡ�������������Ͼ������100.f���Ǹ�����Ϊԭʼ����Ѱ�ҵ�����
	if (EPDistance > 100.f) {
		//��һ��
		SPToSE.Normalize();
		//̽����������λ�ü�ȥ�����֮������һ���
		const FVector ChaseOrigin = SEController->GetPlayerLocation() - 100.f * SPToSE;
		//���������λ��
		FVector DesLoc(0.f);
		//ʹ�õ���ϵͳ��ȡ�����
		UNavigationSystem::K2_GetRandomReachablePointInRadius(SEController, ChaseOrigin, DesLoc, ChaseRadius);
		//�޸�Ŀ�ĵ�
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(Destination.SelectedKeyName, DesLoc);
	}
	else
	{
		//�������С��100.f,��ô���õ��˵�ǰ��λ��ΪĿ��λ��
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(Destination.SelectedKeyName, SECharacter->GetActorLocation());
	}
	//���سɹ�
	return EBTNodeResult::Succeeded;
}