// Fill out your copyright notice in the Description page of Project Settings.

#include "SlAiEnemyController.h"
#include "Kismet/GameplayStatics.h"
#include "SlAiPlayerCharacter.h"
#include "SlAiEnemyCharacter.h"

#include "SlAiEnemyBlackboard.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"




ASlAiEnemyController::ASlAiEnemyController()
{
	PrimaryActorTick.bCanEverTick = true;
}


void ASlAiEnemyController::OnPossess(APawn* InPawn)
{
	//����һ������Ҫ���ø��ຯ��
	Super::OnPossess(InPawn);

	//˳��������ʵ����һ�½�ɫ
	SECharacter = Cast<ASlAiEnemyCharacter>(InPawn);



	//��ȡ��Ϊ����Դ
	UBehaviorTree* StaticBehaviorTreeObject = LoadObject<UBehaviorTree>(NULL, TEXT("BehaviorTree'/Game/Blueprint/Enemy/EnemyBehaviorTree.EnemyBehaviorTree'"));

	UBehaviorTree* BehaviorTreeObject = DuplicateObject<UBehaviorTree>(StaticBehaviorTreeObject, NULL);

	//�����Դ������,ֱ�ӷ���
	if (!BehaviorTreeObject) return;

	BehaviorTreeObject->BlackboardAsset = DuplicateObject<USlAiEnemyBlackboard>((USlAiEnemyBlackboard*)StaticBehaviorTreeObject->BlackboardAsset, NULL);
	


	BlackboardComp = Blackboard;

	bool IsSuccess = true;

	if (BehaviorTreeObject->BlackboardAsset && (Blackboard == nullptr || Blackboard->IsCompatibleWith(BehaviorTreeObject->BlackboardAsset) == false))
	{
		IsSuccess = UseBlackboard(BehaviorTreeObject->BlackboardAsset, BlackboardComp);
	}

	if (IsSuccess)
	{
		BehaviorComp = Cast<UBehaviorTreeComponent>(BrainComponent);
		if (!BehaviorComp)
		{
			BehaviorComp = NewObject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
			BehaviorComp->RegisterComponent();
		}
		BrainComponent = BehaviorComp;
		check(BehaviorComp != NULL);

		BehaviorComp->StartTree(*BehaviorTreeObject, EBTExecutionMode::Looped);

		// ����Ԥ״̬ΪѲ��
		BlackboardComp->SetValueAsEnum("EnemyState", (uint8)EEnemyAIState::ES_Patrol);

		//�޸ĵ��˵ĳ�ʼ�ƶ��ٶ���100
		SECharacter->SetMaxSpeed(100.f);

		/*int32 EnemyStateIndex = BlackboardComp->GetKeyID("EnemyState");
		BlackboardComp->SetValue<UBlackboardKeyType_Enum>(EnemyStateIndex, (UBlackboardKeyType_Enum::FDataType)EEnemyAIState::ES_Patrol);*/
	}

}

void ASlAiEnemyController::OnUnPossess()
{
	Super::OnUnPossess();

	//ֹͣ��Ϊ��
	if (BehaviorComp) BehaviorComp->StopTree();
}

void ASlAiEnemyController::BeginPlay()
{
	//����һ������Ҫ���ø��ຯ��
	Super::BeginPlay();

	//��ʼ��һ�����ָ��,���ָ���һֱ����
	SPCharacter = Cast<ASlAiPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GWorld, 0));
	//�����ɫû�г�ʼ��
	if (!SECharacter) SECharacter = Cast<ASlAiEnemyCharacter>(GetPawn());
	//��ʼ�趨û���������
	IsLockPlayer = false;
	//����ί�а�
	FTimerDelegate EPDisDele = FTimerDelegate::CreateUObject(this, &ASlAiEnemyController::UpdateStatePama);
	GetWorld()->GetTimerManager().SetTimer(EPDisHandle, EPDisDele, 0.3f, true);
	//Ѫ���ٷֱȳ�ʼ��Ϊ1
	HPRatio = 1;
	//����״̬��ʱ��
	IsAllowHurt = false;
	HurtTimeCount = 0.f;
}


void ASlAiEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//������ָ��ͽ�ɫָ�����
	if (SECharacter && SPCharacter) SECharacter->UpdateHPBarRotation(SPCharacter->GetCameraPos());
}

FVector ASlAiEnemyController::GetPlayerLocation() const
{
	//������ָ�����,�������λ��
	if (SPCharacter) return SPCharacter->GetActorLocation();
	return FVector::ZeroVector;
}

bool ASlAiEnemyController::IsPlayerDead()
{
	if (SPCharacter) return SPCharacter->IsPlayerDead();
	return false;
}

void ASlAiEnemyController::OnSeePlayer()
{
	//����Ѿ���������һ�������Ѿ�����,����ִ������ĺ���
	if (IsLockPlayer || IsPlayerDead()) return;

	//�������������
	IsLockPlayer = true;
	//�޸�Ԥ״̬Ϊ׷��
	BlackboardComp->SetValueAsEnum("EnemyState", (uint8)EEnemyAIState::ES_Chase);
	//�޸�����ٶ�Ϊ300
	SECharacter->SetMaxSpeed(300.f);
}

void ASlAiEnemyController::LoosePlayer()
{
	//���ö�ʧ��ҵ�����
	IsLockPlayer = false;
}

bool ASlAiEnemyController::IsPlayerAway()
{
	if (!IsLockPlayer || !SPCharacter || EPDisList.Num() < 6 || IsPlayerDead()) return false;
	int BiggerNum = 0;
	float LastDis = -1.f;
	//ֻҪ��������ǰ��Ĵ�,���ж�Զ����
	for (TArray<float>::TIterator It(EPDisList); It; ++It)
	{
		if (*It > LastDis) BiggerNum += 1;
		LastDis = *It;
	}
	return BiggerNum > 3;
}


UObject* ASlAiEnemyController::GetPlayerPawn()
{
	return SPCharacter;
}

void ASlAiEnemyController::ResetProcess(bool IsFinish)
{
	//�޸����״̬
	BlackboardComp->SetValueAsBool("ProcessFinish", IsFinish);
}

void ASlAiEnemyController::UpdateDamageRatio(float HPRatioVal)
{
	//����Ѫֵ�ٷֱ�
	HPRatio = HPRatioVal;
	//״̬�޸�Ϊ����
	if(IsAllowHurt) BlackboardComp->SetValueAsEnum("EnemyState", (uint8)EEnemyAIState::ES_Hurt);
	//������������״̬
	IsAllowHurt = false;
}

void ASlAiEnemyController::FinishStateHurt()
{
	//���û���������,��������
	if (!IsLockPlayer) IsLockPlayer = true;
	//���Ѫֵ��0.2f����
	if (HPRatio < 0.2f)
	{
		//���������
		FRandomStream Stream;
		//�����µ��������
		Stream.GenerateNewSeed();
		//�����һ���������
		int ActionRatio = Stream.RandRange(0, 10);
		//30�ļ��ʴ�������
		if (ActionRatio < 4) {
			//�Ƚ������״̬
			BlackboardComp->SetValueAsEnum("EnemyState", (uint8)EEnemyAIState::ES_Defence);
		}
		else 
		{
			BlackboardComp->SetValueAsEnum("EnemyState", (uint8)EEnemyAIState::ES_Escape);
		}
	}
	else
	{
		//���������
		FRandomStream Stream;
		//�����µ��������
		Stream.GenerateNewSeed();
		//�����һ���������
		int ActionRatio = Stream.RandRange(0, 10);
		//30�ļ��ʴ�������
		if (ActionRatio < 4) {
			//�������״̬
			BlackboardComp->SetValueAsEnum("EnemyState", (uint8)EEnemyAIState::ES_Defence);
		}
		else
		{
			//���빥��
			BlackboardComp->SetValueAsEnum("EnemyState", (uint8)EEnemyAIState::ES_Attack);
		}
	}
}

void ASlAiEnemyController::FinishStateDefence()
{
	//����״̬���
	ResetProcess(true);
	//ֹͣ��������
	SECharacter->StopDefence();
	//��ȡ����ҵľ���
	float SEToSP = FVector::Distance(SECharacter->GetActorLocation(), GetPlayerLocation());
	//�����һ��ڹ������Ҿ���С��200,��������״̬
	if (SPCharacter->IsAttack && SEToSP < 200.f)
	{
		BlackboardComp->SetValueAsEnum("EnemyState", (uint8)EEnemyAIState::ES_Defence);
	}
	else 
	{
		//���ѪֵС��0.2,����
		if (HPRatio < 0.2f)
		{
			BlackboardComp->SetValueAsEnum("EnemyState", (uint8)EEnemyAIState::ES_Escape);
		}
		else
		{
			//��������״̬
			BlackboardComp->SetValueAsEnum("EnemyState", (uint8)EEnemyAIState::ES_Attack);
		}
	}


}

void ASlAiEnemyController::EnemyDead()
{
	//ֹͣ��Ϊ��
	if (BehaviorComp) BehaviorComp->StopTree(EBTStopMode::Safe);

	//��ʱ����,ע��ʱ�亯��
	if (EPDisHandle.IsValid()) GetWorld()->GetTimerManager().ClearTimer(EPDisHandle);
}

void ASlAiEnemyController::UpdateStatePama()
{
	//��������ҵľ�������
	if (EPDisList.Num() < 6)
	{
		EPDisList.Push(FVector::Distance(SECharacter->GetActorLocation(), GetPlayerLocation()));
	}
	else 
	{
		EPDisList.RemoveAt(0);
		EPDisList.Push(FVector::Distance(SECharacter->GetActorLocation(), GetPlayerLocation()));
	}

	//�������˺�����,����һ�κ�6���ڲ������������״̬
	if (HurtTimeCount < 6.f)
	{
		HurtTimeCount += 0.3f;
	}
	else {
		HurtTimeCount = 0.f;
		IsAllowHurt = true;
	}

}
