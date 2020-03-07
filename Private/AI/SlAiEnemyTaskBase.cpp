// Fill out your copyright notice in the Description page of Project Settings.

#include "SlAiEnemyTaskBase.h"
#include "SlAiEnemyController.h"



bool USlAiEnemyTaskBase::InitEnemyElement(UBehaviorTreeComponent& OwnerComp)
{
	//����Ѿ���ʼ����,ֱ��return,�����ظ�����
	if (SEController && SECharacter) return true;
	//���и�ֵ
	SEController = Cast<ASlAiEnemyController>(OwnerComp.GetAIOwner());
	if (SEController) {
		SECharacter = Cast<ASlAiEnemyCharacter>(SEController->GetPawn());
		if (SECharacter) return true;
	}
	return false;
}
