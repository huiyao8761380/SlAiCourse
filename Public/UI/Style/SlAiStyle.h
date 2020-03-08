// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
//创建SlAiStyle
/**
 * 
 */
class SLAICOURSE_API SlAiStyle
{
public:
	
	static void Initialze();

	static FName GetStyleSetName();

	static void ShutDown();

	static const ISlateStyle& Get();


private:

	static TSharedRef<class FSlateStyleSet> Create(); //注册样式

	static TSharedPtr<FSlateStyleSet> SlAiStyleInstance;


};
