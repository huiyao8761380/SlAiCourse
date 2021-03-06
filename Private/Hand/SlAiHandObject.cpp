﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "SlAiHandObject.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "ConstructorHelpers.h"

//手持物品类
#include "SlAiHandNone.h"
#include "SlAiHandWood.h"
#include "SlAiHandStone.h"
#include "SlAiHandApple.h"
#include "SlAiHandMeat.h"
#include "SlAiHandAxe.h"
#include "SlAiHandHammer.h"
#include "SlAiHandSword.h"

#include "SlAiEnemyCharacter.h"

#include "SlAiHelper.h"
#include "SlAiDataHandle.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
ASlAiHandObject::ASlAiHandObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//实例化根组件
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	//创建静态模型组件
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent);
	BaseMesh->SetCollisionProfileName(FName("NoCollision"));

	//实现碰撞组件
	AffectCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("AffectCollision"));
	AffectCollision->SetupAttachment(RootComponent);
	AffectCollision->SetCollisionProfileName(FName("ToolProfile"));

	//初始时关闭Overlay检测
	//AffectCollision->bGenerateOverlapEvents = false;
	AffectCollision->SetGenerateOverlapEvents(true);

	//绑定检测方法到碰撞体
	FScriptDelegate OverlayBegin;
	OverlayBegin.BindUFunction(this, "OnOverlayBegin");
	AffectCollision->OnComponentBeginOverlap.Add(OverlayBegin);

	FScriptDelegate OverlayEnd;
	OverlayEnd.BindUFunction(this, "OnOverlayEnd");
	AffectCollision->OnComponentEndOverlap.Add(OverlayEnd);

	//默认拳头音效
	static ConstructorHelpers::FObjectFinder<USoundWave> StaticSound(TEXT("SoundWave'/Game/Res/Sound/GameSound/Punch.Punch'"));
	OverlaySound = StaticSound.Object;

}

// Called when the game starts or when spawned
void ASlAiHandObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASlAiHandObject::OnOverlayBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (Cast<ASlAiEnemyCharacter>(OtherActor))
	{
		//获取物品属性
		TSharedPtr<ObjectAttribute> ObjectAttr = *SlAiDataHandle::Get()->ObjectAttrMap.Find(ObjectIndex);
		//获取对动物的伤害值
		Cast<ASlAiEnemyCharacter>(OtherActor)->AcceptDamage(ObjectAttr->AnimalAttack);
	}
	//如果音效存在,播放音效,默认音效为拳打
	if (OverlaySound) UGameplayStatics::PlaySoundAtLocation(GetWorld(), OverlaySound, OtherActor->GetActorLocation());
}

void ASlAiHandObject::OnOverlayEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

// Called every frame
void ASlAiHandObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


TSubclassOf<AActor> ASlAiHandObject::SpawnHandObject(int ObjectID)
{
	switch (ObjectID)
	{
	case 0:
		return ASlAiHandNone::StaticClass();
	case 1:
		return ASlAiHandWood::StaticClass();
	case 2:
		return ASlAiHandStone::StaticClass();
	case 3:
		return ASlAiHandApple::StaticClass();
	case 4:
		return ASlAiHandMeat::StaticClass();
	case 5:
		return ASlAiHandAxe::StaticClass();
	case 6:
		return ASlAiHandHammer::StaticClass();
	case 7:
		return ASlAiHandSword::StaticClass();
	}

	return ASlAiHandNone::StaticClass();
}

void ASlAiHandObject::ChangeOverlayDetect(bool IsOpen)
{
	//AffectCollision->bGenerateOverlapEvents = IsOpen;
	AffectCollision->SetGenerateOverlapEvents(true);
}

