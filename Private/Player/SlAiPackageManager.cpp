// Fill out your copyright notice in the Description page of Project Settings.

#include "SlAiPackageManager.h"
#include "SlAiDataHandle.h"

//��ʼʱ�Ե���ָ�븳ֵ
TSharedPtr<SlAiPackageManager> SlAiPackageManager::PackageInstance = NULL;

void SlAiPackageManager::Initialize()
{
	if (!PackageInstance.IsValid())
		PackageInstance = Create();
}

TSharedPtr<SlAiPackageManager> SlAiPackageManager::Get()
{
	Initialize();
	return PackageInstance;
}


TSharedRef<SlAiPackageManager> SlAiPackageManager::Create()
{
	TSharedRef<SlAiPackageManager> PackageRef = MakeShareable(new SlAiPackageManager());
	return PackageRef;
}

SlAiPackageManager::SlAiPackageManager()
{
	//��ʼ����Ʒ������Ϊ0
	ObjectIndex = 0;
	ObjectNum = 0;
}

void SlAiPackageManager::InsertContainer(TSharedPtr<class SSlAiContainerBaseWidget> Container, EContainerType::Type InsertType)
{
	switch (InsertType)
	{
	case EContainerType::Output:
		OutputContainer = Container;
		OutputContainer->CompoundOutput.BindRaw(this, &SlAiPackageManager::CompoundOutput);
		OutputContainer->ThrowObject.BindRaw(this, &SlAiPackageManager::ThrowObject);
		break;
	case EContainerType::Input:
		Container->CompoundInput.BindRaw(this, &SlAiPackageManager::CompoundInput);
		InputContainerList.Add(Container);
		break;
	case EContainerType::Normal:
		NormalContainerList.Add(Container);
		break;
	case EContainerType::Shortcut:
		Container->PackShortChange.BindRaw(this, &SlAiPackageManager::PackShortChange);
		ShortcutContainerList.Add(Container);
		break;
	}
}

void SlAiPackageManager::UpdateHovered(FVector2D MousePos, FGeometry PackGeo)
{
	//�Ȼ�ȡ��ͣ������
	TSharedPtr<SSlAiContainerBaseWidget> CurrHoveredCon = LocateContainer(MousePos, PackGeo);
	//�����������
	if (CurrHoveredCon.IsValid())
	{
		//���õ�ǰ������ͣ��ʾ
		CurrHoveredCon->UpdateHovered(true);
		//�����һ��������,�����뵱ǰ��������ͬ
		if (LastHoveredCon.IsValid() && LastHoveredCon.Get() != CurrHoveredCon.Get()) {
			//������ͣ��ʾ
			LastHoveredCon->UpdateHovered(false);
		}
	}
	else 
	{
		//��ǰ��������������һ��������,ȡ����һ��������ͣ��ʾ
		if (LastHoveredCon.IsValid()) {
			LastHoveredCon->UpdateHovered(false);
		}
	}

	//������һ��ͣ����
	LastHoveredCon = CurrHoveredCon;
}



void SlAiPackageManager::LeftOption(FVector2D MousePos, FGeometry PackGeo)
{
	//�Ȼ�ȡ���������
	TSharedPtr<SSlAiContainerBaseWidget> ClickedContainer = LocateContainer(MousePos, PackGeo);
	//�����������,ִ�������¼�
	if (ClickedContainer.IsValid()) 
	{
		ClickedContainer->LeftOperate(ObjectIndex, ObjectNum, ObjectIndex, ObjectNum);
	}
	//������������ڲ���������Ʒ��Ϊ��
	if (!ClickedContainer.IsValid() && ObjectIndex != 0) 
	{
		//����Ʒ����,����д
		ThrowObject(ObjectIndex, ObjectNum);
		//������Ʒ
		ObjectIndex = ObjectNum = 0;
	}
}

void SlAiPackageManager::RightOption(FVector2D MousePos, FGeometry PackGeo)
{
	//�Ȼ�ȡ���������
	TSharedPtr<SSlAiContainerBaseWidget> ClickedContainer = LocateContainer(MousePos, PackGeo);
	//�����������,ִ�������¼�
	if (ClickedContainer.IsValid()) {
		ClickedContainer->RightOperate(ObjectIndex, ObjectNum, ObjectIndex, ObjectNum);
	}
}

TSharedPtr<SSlAiContainerBaseWidget> SlAiPackageManager::LocateContainer(FVector2D MousePos, FGeometry PackGeo)
{
	//�����ҵ�ָ�������
	for (TArray<TSharedPtr<SSlAiContainerBaseWidget>>::TIterator It(ShortcutContainerList); It; ++It) {
		//��ȡ����
		FVector2D StartPos = PackGeo.AbsoluteToLocal((*It)->GetCachedGeometry().AbsolutePosition);
		FVector2D EndPos = StartPos + FVector2D(80.f, 80.f);
		//�ж����λ���Ƿ���������,�ڵĻ�ֱ�ӷ����������
		if (MousePos.X >= StartPos.X && MousePos.X <= EndPos.X && MousePos.Y >= StartPos.Y && MousePos.Y <= EndPos.Y)
		{
			return *It;
		}
	}
	for (TArray<TSharedPtr<SSlAiContainerBaseWidget>>::TIterator It(NormalContainerList); It; ++It) {
		//��ȡ����
		FVector2D StartPos = PackGeo.AbsoluteToLocal((*It)->GetCachedGeometry().AbsolutePosition);
		FVector2D EndPos = StartPos + FVector2D(80.f, 80.f);
		//�ж����λ���Ƿ���������,�ڵĻ�ֱ�ӷ����������
		if (MousePos.X >= StartPos.X && MousePos.X <= EndPos.X && MousePos.Y >= StartPos.Y && MousePos.Y <= EndPos.Y) {
			return *It;
		}
	}
	for (TArray<TSharedPtr<SSlAiContainerBaseWidget>>::TIterator It(InputContainerList); It; ++It) {
		//��ȡ����
		FVector2D StartPos = PackGeo.AbsoluteToLocal((*It)->GetCachedGeometry().AbsolutePosition);
		FVector2D EndPos = StartPos + FVector2D(80.f, 80.f);
		//�ж����λ���Ƿ���������,�ڵĻ�ֱ�ӷ����������
		if (MousePos.X >= StartPos.X && MousePos.X <= EndPos.X && MousePos.Y >= StartPos.Y && MousePos.Y <= EndPos.Y) {
			return *It;
		}
	}
	//���ﴦ�����������
	//��ȡ����
	FVector2D StartPos = PackGeo.AbsoluteToLocal(OutputContainer->GetCachedGeometry().AbsolutePosition);
	FVector2D EndPos = StartPos + FVector2D(80.f, 80.f);
	//�ж����λ���Ƿ���������,�ڵĻ�ֱ�ӷ����������
	if (MousePos.X >= StartPos.X && MousePos.X <= EndPos.X && MousePos.Y >= StartPos.Y && MousePos.Y <= EndPos.Y) {
		return OutputContainer;
	}
	//��󷵻ؿ�
	return nullptr;
}

void SlAiPackageManager::ThrowObject(int ObjectID, int Num)
{
	PlayerThrowObject.ExecuteIfBound(ObjectID, Num);
}


void SlAiPackageManager::PackShortChange(int ShortcutID, int ObjectID, int ObjectNum)
{
	//ִ��ί��,�󶨵ķ�����PlayerState��ChangeHandObject,��playercharacter�½��а�
	ChangeHandObject.ExecuteIfBound(ShortcutID, ObjectID, ObjectNum);
}




void SlAiPackageManager::CompoundOutput(int ObjectID, int Num)
{
	//�������Ϊ0,ֱ��return
	if (ObjectID == 0) return;
	//�ϳɱ�ṹ����
	TArray<int> TableMap;
	for (TArray<TSharedPtr<SSlAiContainerBaseWidget>>::TIterator It(InputContainerList); It; ++It) {
		TableMap.Add((*It)->GetIndex());
	}
	TableMap.Add(ObjectID);
	//��������������
	TArray<int> ExpendMap;
	//�����ҳ����ϵĺϳɱ����õ���������������
	for (TArray<TSharedPtr<CompoundTable>>::TIterator It(SlAiDataHandle::Get()->CompoundTableMap); It; ++It) {
		//����ҵ����ϵ�ֱ������ѭ��
		if ((*It)->DetectExpend(&TableMap, Num, ExpendMap)) break;
	}
	//�����������Ԫ����������9,ֱ��return
	if (ExpendMap.Num() != 9) return;
	//ѭ�����úϳ�����������
	for (int i = 0; i < 9; ++i) {
		//���ԭ��������ȥ���������Ѿ�С�ڵ���0,ֱ�Ӱ���ƷID����Ϊ0
		int InputID = (InputContainerList[i]->GetNum() - ExpendMap[i] <= 0) ? 0 : InputContainerList[i]->GetIndex();
		int InputNum = (InputID == 0) ? 0 : (InputContainerList[i]->GetNum() - ExpendMap[i]);
		//���ò���
		InputContainerList[i]->ResetContainerPara(InputID, InputNum);
	}
}

void SlAiPackageManager::CompoundInput()
{
	//��ȡ�ϳ�̨9����������Ʒid������д����������
	TArray<int> IDMap;
	TArray<int> NumMap;
	for (TArray<TSharedPtr<SSlAiContainerBaseWidget>>::TIterator It(InputContainerList); It; ++It) {
		IDMap.Add((*It)->GetIndex());
		NumMap.Add((*It)->GetNum());
	}
	//���������������ID������
	int OutputIndex = 0;
	int OutputNum = 0;
	//�����ϳɱ���м��
	for (TArray<TSharedPtr<CompoundTable>>::TIterator It(SlAiDataHandle::Get()->CompoundTableMap); It; ++It) {
		(*It)->DetectTable(&IDMap, &NumMap, OutputIndex, OutputNum);
		//�����������,ֱ������ѭ��
		if (OutputIndex != 0 && OutputNum != 0) break;
	}
	//�����OutputContainer���и�ֵ
	//���ж��Ƿ���Ե���
	if (MultiplyAble(OutputIndex)) {
		//���Ե��ӵĻ���ֱ�Ӹ�ֵ,����0,0
		OutputContainer->ResetContainerPara(OutputIndex, OutputNum);
	}
	else {
		//�����Ե��ӵĻ�ֻ���һ��
		OutputContainer->ResetContainerPara(OutputIndex, 1);
	}
}


bool SlAiPackageManager::MultiplyAble(int ObjectID)
{
	//��ȡ��Ʒ����
	TSharedPtr<ObjectAttribute> ObjectAttr = *SlAiDataHandle::Get()->ObjectAttrMap.Find(ObjectID);
	//�����Ƿ����������߹���
	return (ObjectAttr->ObjectType != EObjectType::Tool && ObjectAttr->ObjectType != EObjectType::Weapon);
}

bool SlAiPackageManager::SearchFreeSpace(int ObjectID, TSharedPtr<SSlAiContainerBaseWidget>& FreeContainer)
{
	//����������
	TSharedPtr<SSlAiContainerBaseWidget> EmptyContainer;
	
	for (TArray<TSharedPtr<SSlAiContainerBaseWidget>>::TIterator It(ShortcutContainerList); It; ++It) {
		//���������������
		if (!EmptyContainer.IsValid()) {
			if ((*It)->IsEmpty()) EmptyContainer = *It;
		}
		//�������������û������
		if (!FreeContainer.IsValid())
		{
			//��������������Ʒ��������ƷID��ͬ�����пռ�,ֱ��ָ��
			if ((*It)->RemainSpace(ObjectID)) 
			{
				FreeContainer = *It;
				return true;
			}
		}
	}

	for (TArray<TSharedPtr<SSlAiContainerBaseWidget>>::TIterator It(NormalContainerList); It; ++It) {
		//���������������
		if (!EmptyContainer.IsValid()) {
			if ((*It)->IsEmpty()) EmptyContainer = *It;
		}
		//�������������û������
		if (!FreeContainer.IsValid())
		{
			//��������������Ʒ��������ƷID��ͬ�����пռ�,ֱ��ָ��
			if ((*It)->RemainSpace(ObjectID))
			{
				FreeContainer = *It;
				return true;
			}
		}
	}
	//�����е�����˵����Ҫָ��������
	if (EmptyContainer.IsValid()) {
		FreeContainer = EmptyContainer;
		return true;
	}
	//�������������û��,����false
	return false;
}

bool SlAiPackageManager::SearchFreeSpace(int ObjectID)
{
	TSharedPtr<SSlAiContainerBaseWidget> FreeContainer;
	return SearchFreeSpace(ObjectID, FreeContainer);
}

void SlAiPackageManager::AddObject(int ObjectID)
{
	TSharedPtr<SSlAiContainerBaseWidget> FreeContainer;
	if (SearchFreeSpace(ObjectID, FreeContainer)) {
		//�����Ʒ������
		FreeContainer->AddObject(ObjectID);
	}
}

bool SlAiPackageManager::EatUpEvent(int ShortcutID)
{
	//��ȡ��Ʒ����
	TSharedPtr<ObjectAttribute> ObjectAttr = *SlAiDataHandle::Get()->ObjectAttrMap.Find(ShortcutContainerList[ShortcutID]->GetIndex());
	//��������Ʒ��ʳ��
	if (ObjectAttr->ObjectType == EObjectType::Food)
	{
		//�µ���Ʒ����
		int NewNum = (ShortcutContainerList[ShortcutID]->GetNum() - 1) < 0 ? 0 : (ShortcutContainerList[ShortcutID]->GetNum() - 1);
		//�µ���ƷID
		int NewIndex = NewNum == 0 ? 0 : ShortcutContainerList[ShortcutID]->GetIndex();
		//���������������
		ShortcutContainerList[ShortcutID]->ResetContainerPara(NewIndex, NewNum);
		//���سɹ��Ե�ʳ��
		return true;
	}
	return false;
}

void SlAiPackageManager::LoadRecord(TArray<int32>* InputIndex, TArray<int32>* InputNum, TArray<int32>* NormalIndex, TArray<int32>* NormalNum, TArray<int32>* ShortcutIndex, TArray<int32>* ShortcutNum)
{
	for (int i = 0; i < InputContainerList.Num(); ++i)
	{
		if ((*InputIndex)[i] != 0) InputContainerList[i]->ResetContainerPara((*InputIndex)[i], (*InputNum)[i]);
	}
	for (int i = 0; i < NormalContainerList.Num(); ++i) {
		if ((*NormalIndex)[i] != 0) NormalContainerList[i]->ResetContainerPara((*NormalIndex)[i], (*NormalNum)[i]);
	}
	for (int i = 0; i < ShortcutContainerList.Num(); ++i) {
		if ((*ShortcutIndex)[i] != 0) ShortcutContainerList[i]->ResetContainerPara((*ShortcutIndex)[i], (*ShortcutNum)[i]);
	}
}

void SlAiPackageManager::SaveData(TArray<int32>& InputIndex, TArray<int32>& InputNum, TArray<int32>& NormalIndex, TArray<int32>& NormalNum, TArray<int32>& ShortcutIndex, TArray<int32>& ShortcutNum)
{
	for (int i = 0; i < InputContainerList.Num(); ++i)
	{
		InputIndex.Add(InputContainerList[i]->GetIndex());
		InputNum.Add(InputContainerList[i]->GetNum());
	}
	for (int i = 0; i < NormalContainerList.Num(); ++i) {
		NormalIndex.Add(NormalContainerList[i]->GetIndex());
		NormalNum.Add(NormalContainerList[i]->GetNum());
	}
	for (int i = 0; i < ShortcutContainerList.Num(); ++i) {
		ShortcutIndex.Add(ShortcutContainerList[i]->GetIndex());
		ShortcutNum.Add(ShortcutContainerList[i]->GetNum());
	}
}

