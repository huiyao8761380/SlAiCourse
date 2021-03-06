﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "SlAiFlobObject.h"
#include "ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "SlAiDataHandle.h"
#include "TimerManager.h"
#include "SlAiPlayerCharacter.h"

// Sets default values
ASlAiFlobObject::ASlAiFlobObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = (USceneComponent*)BoxCollision;
	//设置碰撞属性
	BoxCollision->SetCollisionProfileName(FName("FlobProfile"));
	//启动物体模拟
	BoxCollision->SetSimulatePhysics(true);
	//锁定旋转
	BoxCollision->SetConstraintMode(EDOFMode::Default);
	BoxCollision->GetBodyInstance()->bLockXRotation = true;
	BoxCollision->GetBodyInstance()->bLockYRotation = true;
	BoxCollision->GetBodyInstance()->bLockZRotation = true;
	//设置大小
	BoxCollision->SetBoxExtent(FVector(15.f));

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticBaseMesh(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	BaseMesh->SetStaticMesh(StaticBaseMesh.Object);
	BaseMesh->SetCollisionResponseToChannels(ECR_Ignore);
	BaseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//设置变换
	BaseMesh->SetRelativeRotation(FRotator(0.f, 0.f, 90.f));
	BaseMesh->SetRelativeScale3D(FVector(0.3f));

	UMaterialInterface* StaticObjectIconMat = LoadObject<UMaterialInterface>(NULL, TEXT("MaterialInstanceConstant'/Game/Material/FlobIconMatInst.FlobIconMatInst'"));
	//动态创建材质
	ObjectIconMatDynamic = UMaterialInstanceDynamic::Create(StaticObjectIconMat, nullptr);
}

// Called when the game starts or when spawned
void ASlAiFlobObject::BeginPlay()
{
	Super::BeginPlay();

	//检测世界是否存在
	if (!GetWorld()) return;

	//注册检测事件
	FTimerDelegate DetectPlayerDele;
	DetectPlayerDele.BindUObject(this, &ASlAiFlobObject::DetectPlayer);
	//每秒运行一次,循环运行,延迟3秒运行
	GetWorld()->GetTimerManager().SetTimer(DetectTimer, DetectPlayerDele, 1.f, true, 3.f);

	//注册销毁事件
	FTimerDelegate DestroyDele;
	DestroyDele.BindUObject(this, &ASlAiFlobObject::DestroyEvent);
	GetWorld()->GetTimerManager().SetTimer(DestroyTimer, DestroyDele, 30.f, false);

	//初始玩家指针为空
	SPCharacter = NULL;
}



// Called every frame
void ASlAiFlobObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//一直旋转
	BaseMesh->AddLocalRotation(FRotator(DeltaTime * 60.f, 0.f, 0.f));

	//如果检测到玩家
	if (SPCharacter) {
		//靠近玩家
		SetActorLocation(FMath::VInterpTo(GetActorLocation(), SPCharacter->GetActorLocation() + FVector(0.f, 0.f, 40.f), DeltaTime, 5.f));
		//如果距离接近0
		if (FVector::Distance(GetActorLocation(), SPCharacter->GetActorLocation() + FVector(0.f, 0.f, 40.f)) < 10.f)
		{
			//判断玩家背包是否有空间
			if (SPCharacter->IsPackageFree(ObjectIndex)) {
				//添加对应的物品到背包
				SPCharacter->AddPackageObject(ObjectIndex);
				//销毁自己
				DestroyEvent();
			}
			else {
				//如果玩家背包不为空,重置参数
				SPCharacter = NULL;
				//唤醒检测
				GetWorld()->GetTimerManager().UnPauseTimer(DetectTimer);
				//唤醒销毁线程
				GetWorld()->GetTimerManager().UnPauseTimer(DestroyTimer);
				//开启物理模拟
				BoxCollision->SetSimulatePhysics(true);
			}
		}
	}
}

void ASlAiFlobObject::CreateFlobObject(int ObjectID)
{
	//指定ID
	ObjectIndex = ObjectID;

	//渲染贴图
	RenderTexture();

	//做随机方向的力
	FRandomStream Stream;
	Stream.GenerateNewSeed();
	int DirYaw = Stream.RandRange(-180, 180);
	FRotator ForceRot = FRotator(0.f, DirYaw, 0.f);

	//添加力
	BoxCollision->AddForce((FVector(0.f, 0.f, 4.f) + ForceRot.Vector()) * 100000.f);
}


void ASlAiFlobObject::ThrowFlobObject(int ObjectID, float DirYaw)
{
	//指定ID
	ObjectIndex = ObjectID;

	//渲染贴图
	RenderTexture();

	//随机方向添加力
	FRandomStream Stream;

	//产生新的随机种子
	Stream.GenerateNewSeed();

	//添加偏移方向
	DirYaw += Stream.RandRange(-30, 30);

	FRotator ForceRot = FRotator(0.f, DirYaw, 0.f);

	//添加力
	BoxCollision->AddForce((FVector(0.f, 0.f, 2.f) + ForceRot.Vector()) * 120000.f);
}

void ASlAiFlobObject::RenderTexture()
{
	TSharedPtr<ObjectAttribute> ObjectAttr = *SlAiDataHandle::Get()->ObjectAttrMap.Find(ObjectIndex);
	ObjectIconTex = LoadObject<UTexture>(NULL, *ObjectAttr->TexPath);
	ObjectIconMatDynamic->SetTextureParameterValue(FName("ObjectTex"), ObjectIconTex);
	BaseMesh->SetMaterial(0, ObjectIconMatDynamic);
}

void ASlAiFlobObject::DetectPlayer()
{
	//检测世界是否存在
	if (!GetWorld()) return;

	//保存检测结果
	TArray<FOverlapResult> Overlaps;
	FCollisionObjectQueryParams ObjectParams;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	//Params.bTraceAsyncScene = true;

	//进行动态检测,检测范围是200,检测成功的话返回true
	if (GetWorld()->OverlapMultiByObjectType(Overlaps, GetActorLocation(), FQuat::Identity, ObjectParams, FCollisionShape::MakeSphere(200.f), Params))
	{
		for (TArray<FOverlapResult>::TIterator It(Overlaps); It; ++It) {
			//如果检测到了玩家
			if (Cast<ASlAiPlayerCharacter>(It->GetActor())) {
				//赋值
				SPCharacter = Cast<ASlAiPlayerCharacter>(It->GetActor());
				//如果背包有空间,后面再添加函数
				if (SPCharacter->IsPackageFree(ObjectIndex))
				{
					//停止检测
					GetWorld()->GetTimerManager().PauseTimer(DetectTimer);
					//停止销毁定时器
					GetWorld()->GetTimerManager().PauseTimer(DestroyTimer);
					//关闭物理模拟
					BoxCollision->SetSimulatePhysics(false);
				}
				return;
			}
		}
	}
}

void ASlAiFlobObject::DestroyEvent()
{
	if (!GetWorld()) return;
	//注销定时器
	GetWorld()->GetTimerManager().ClearTimer(DetectTimer);
	GetWorld()->GetTimerManager().ClearTimer(DestroyTimer);
	//销毁自己
	GetWorld()->DestroyActor(this);
}
