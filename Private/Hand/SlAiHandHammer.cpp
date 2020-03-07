// Fill out your copyright notice in the Description page of Project Settings.

#include "SlAiHandHammer.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"
#include "Components/BoxComponent.h"



ASlAiHandHammer::ASlAiHandHammer()
{
	//��ģ�����������ģ��
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticBaseMesh(TEXT("StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Bld_FencePost_01.SM_Bld_FencePost_01'"));
	//��ģ�͵�Mesh���
	BaseMesh->SetStaticMesh(StaticBaseMesh.Object);

	//������չģ�����
	ExtendMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ExtendMesh"));
	ExtendMesh->SetupAttachment(RootComponent);
	ExtendMesh->SetCollisionProfileName(FName("NoCollision"));

	//��ģ�͵���չģ�����
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticExtendMesh(TEXT("StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Prop_StoneBlock_01.SM_Prop_StoneBlock_01'"));
	//��ģ�͵�Mesh���
	ExtendMesh->SetStaticMesh(StaticExtendMesh.Object);


	BaseMesh->SetRelativeLocation(FVector(35.f, 0.f, 3.f));
	BaseMesh->SetRelativeRotation(FRotator(0.f, -90.f, -90.f));
	BaseMesh->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f));

	ExtendMesh->SetRelativeLocation(FVector(33.f, 1.f, 3.f));
	ExtendMesh->SetRelativeRotation(FRotator(0.f, -90.f, -90.f));
	ExtendMesh->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f));


	//������ײ������
	AffectCollision->SetRelativeLocation(FVector(26.f, 1.f, 3.f));
	AffectCollision->SetRelativeScale3D(FVector(0.22f, 0.44f, 0.31f));



}

void ASlAiHandHammer::BeginPlay()
{
	Super::BeginPlay();

	//������Ʒ���
	ObjectIndex = 6;

	//���¼��ش��ӵ���Ч
	OverlaySound = LoadObject<USoundWave>(NULL, TEXT("SoundWave'/Game/Res/Sound/GameSound/Hammer.Hammer'"));
}