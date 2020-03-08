// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//新建一个None class类的 SlAiSingleton c++文件
/**
*
*/
template<class T>
class SLAICOURSE_API SlAiSingleton
{

public:

	static TSharedPtr<T> Get();

private:

	static void Initialize();

	static TSharedRef<T> Create();//共享的引用

private:

	static TSharedPtr<T> TInstance;//单例的指针

};






template<class T>
TSharedPtr<T> SlAiSingleton<T>::TInstance = NULL;//设置为null避免报错

template<class T>
TSharedRef<T> SlAiSingleton<T>::Create()
{
	TSharedRef<T> TRef = MakeShareable(new T());
	return TRef;
}

template<class T>
void SlAiSingleton<T>::Initialize()
{
	if (!TInstance.IsValid()) {
		TInstance = Create();
	}
}

template<class T>
TSharedPtr<T> SlAiSingleton<T>::Get()
{
	Initialize();
	return TInstance;
}






