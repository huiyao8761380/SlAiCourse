// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlAiTypes.h"
#include "SSlAiContainerBaseWidget.h"
/**
 * 
 */
class SLAICOURSE_API SlAiPackageManager
{

public:

	SlAiPackageManager();

	static void Initialize();

	static TSharedPtr<SlAiPackageManager> Get();

	//�������
	void InsertContainer(TSharedPtr<class SSlAiContainerBaseWidget> Container, EContainerType::Type InsertType);

	//������ͣ��������ɫ
	void UpdateHovered(FVector2D MousePos, FGeometry PackGeo);

	//����¼�,���������λ�ú�PackageWidget��Geometry
	void LeftOption(FVector2D MousePos, FGeometry PackGeo);

	//�Ҽ��¼�,���������λ�ú�PackageWidget��Geometry
	void RightOption(FVector2D MousePos, FGeometry PackGeo);

	//�Ƿ��в�����Ʒ�Ŀռ�,�ṩ���ⲿ����
	bool SearchFreeSpace(int ObjectID);

	//�����Ʒ
	void AddObject(int ObjectID);

	//�Զ���,����������ID,�����Ƿ�ɹ��Ե�
	bool EatUpEvent(int ShortcutID);

	//���ش浵
	void LoadRecord(TArray<int32>* InputIndex, TArray<int32>* InputNum, TArray<int32>* NormalIndex, TArray<int32>* NormalNum, TArray<int32>* ShortcutIndex, TArray<int32>* ShortcutNum);

	void SaveData(TArray<int32>& InputIndex, TArray<int32>& InputNum, TArray<int32>& NormalIndex, TArray<int32>& NormalNum, TArray<int32>& ShortcutIndex, TArray<int32>& ShortcutNum);

public:

	//�����ƷID
	int ObjectIndex;
	//�����Ʒ����
	int ObjectNum;

	//������Ʒί��,�󶨵ķ�����PlayerState��PlayerThrowObject
	FThrowObject PlayerThrowObject;
	//�޸Ŀ������Ϣί��
	FPackShortChange ChangeHandObject;

private:

	//����ʵ������
	static TSharedRef<SlAiPackageManager> Create();

	//��ȡ���ָ�������
	TSharedPtr<SSlAiContainerBaseWidget> LocateContainer(FVector2D MousePos, FGeometry PackGeo);

	//������Ʒ�¼�
	void ThrowObject(int ObjectID, int Num);
	//�ϳ���ȡ�¼�
	void CompoundOutput(int ObjectID, int Num);
	//�ϳ������¼�
	void CompoundInput();
	//������任�¼�
	void PackShortChange(int ShortcutID, int ObjectID, int ObjectNum);
	//��ȡ�Ƿ���Ե���
	bool MultiplyAble(int ObjectID);
	//�Ƿ��в�����Ʒ�Ŀռ�,ÿ��ֻ�����һ��,���ؿ��Բ�����Ǹ�����
	bool SearchFreeSpace(int ObjectID, TSharedPtr<SSlAiContainerBaseWidget>& FreeContainer);

private:

	//����ָ��
	static TSharedPtr<SlAiPackageManager> PackageInstance;

	//�����б�
	TArray<TSharedPtr<SSlAiContainerBaseWidget>> InputContainerList;
	TArray<TSharedPtr<SSlAiContainerBaseWidget>> NormalContainerList;
	TArray<TSharedPtr<SSlAiContainerBaseWidget>> ShortcutContainerList;
	//�������ֻ��һ��
	TSharedPtr<SSlAiContainerBaseWidget> OutputContainer;

	//��һ����ͣ������
	TSharedPtr<SSlAiContainerBaseWidget> LastHoveredCon;

};
