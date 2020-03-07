// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/SceneCapture2D.h"
#include "SlAiSceneCapture2D.generated.h"


class UTextureRenderTarget2D;

/**
 * 
 */
UCLASS()
class SLAICOURSE_API ASlAiSceneCapture2D : public ASceneCapture2D
{
	GENERATED_BODY()

public:

	ASlAiSceneCapture2D();

	//��ȡMiniMapText
	UTextureRenderTarget2D* GetMiniMapTex();

	//���±任
	void UpdateTransform(FVector NormLocation, FRotator NormRotator);

	//������Ұ��Χ
	void UpdateMiniMapWidth(int Delta);

	//��ȡС��ͼ�ߴ�
	float GetMapSize();

private:

	UTextureRenderTarget2D* MiniMapTex;
	
	
};
