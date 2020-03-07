// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SlAiEnemyTool.generated.h"

UCLASS()
class SLAICOURSE_API ASlAiEnemyTool : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASlAiEnemyTool();


	//�Ƿ��������
	void ChangeOverlayDetect(bool IsOpen);

	static TSubclassOf<AActor> SpawnEnemyWeapon();

	static TSubclassOf<AActor> SpawnEnemySheild();


protected:

	UFUNCTION()
		virtual void OnOverlayBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		virtual void OnOverlayEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


protected:

	//�����
	USceneComponent* RootScene;

	//��̬ģ��
	UPROPERTY(EditAnywhere, Category = Mesh)
		UStaticMeshComponent* BaseMesh;


	//������ײ��
	UPROPERTY(EditAnywhere, Category = Mesh)
		class UBoxComponent* AffectCollision;


	
	
};