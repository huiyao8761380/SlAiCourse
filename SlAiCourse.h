// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class FSlAiCourceModule : public FDefaultGameModuleImpl
{
public:

	virtual void StartupModule() override;//看作模组 模组加载时调用
	virtual void ShutdownModule() override;

};