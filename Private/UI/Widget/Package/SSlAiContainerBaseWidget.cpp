// Fill out your copyright notice in the Description page of Project Settings.

#include "SSlAiContainerBaseWidget.h"
#include "SlateOptMacros.h"
#include "SlAiStyle.h"
#include "SlAiGameWidgetStyle.h"

#include "SBorder.h"
#include "STextBlock.h"

#include "SSlAiContainerNormalWidget.h"
#include "SSlAiContainerShortcutWidget.h"
#include "SSlAiContainerInputWidget.h"
#include "SSlAiContainerOutputWidget.h"
#include "SlAiDataHandle.h"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSlAiContainerBaseWidget::Construct(const FArguments& InArgs)
{

	//��ȡGameStyle
	GameStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiGameStyle>("BPSlAiGameStyle");
	//��ȡ������
	WorkIndex = InArgs._WorkIndex;


	ChildSlot
		[
			SAssignNew(ContainerBorder, SBorder)
			.BorderImage(&GameStyle->NormalContainerBrush)
		.Padding(FMargin(8.f))
		[
			SAssignNew(ObjectImage, SBorder)
			.BorderImage(&GameStyle->EmptyBrush)
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Bottom)
		.Padding(FMargin(0.f, 0.f, 4.f, 0.f))
		[
			SAssignNew(ObjectNumText, STextBlock)
			.Font(GameStyle->Font_Outline_16)
		.ColorAndOpacity(GameStyle->FontColor_Black)
		]
		]
		];


	IsHover = false;

	//��ʼ��ʱ��Ʒ��������Ŷ�Ϊ0
	ObjectIndex = ObjectNum = 0;

}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

TSharedPtr<SSlAiContainerBaseWidget> SSlAiContainerBaseWidget::CreateContainer(EContainerType::Type NeedType, int WorkID)
{
	TSharedPtr<SSlAiContainerBaseWidget> ResultContainer;
	switch (NeedType)
	{
	case EContainerType::Output:
		SAssignNew(ResultContainer, SSlAiContainerOutputWidget).WorkIndex(WorkID);
		break;
	case EContainerType::Input:
		SAssignNew(ResultContainer, SSlAiContainerInputWidget).WorkIndex(WorkID);
		break;
	case EContainerType::Normal:
		SAssignNew(ResultContainer, SSlAiContainerNormalWidget).WorkIndex(WorkID);
		break;
	case EContainerType::Shortcut:
		SAssignNew(ResultContainer, SSlAiContainerShortcutWidget).WorkIndex(WorkID);
		break;
	}
	return ResultContainer;
}

void SSlAiContainerBaseWidget::UpdateHovered(bool IsHovered)
{
	//�������ƶ���������
	if (IsHovered) 
	{
		if (!IsHover) ContainerBorder->SetBorderImage(&GameStyle->ChoosedContainerBrush);
	}
	else 
	{
		if (IsHover) ContainerBorder->SetBorderImage(&GameStyle->NormalContainerBrush);
	}
	//���µ�ǰ״̬
	IsHover = IsHovered;
}

void SSlAiContainerBaseWidget::ResetContainerPara(int ObjectID, int Num)
{
	//�������ID����ͬ,������ͼ
	if (ObjectIndex != ObjectID) ObjectImage->SetBorderImage(SlAiDataHandle::Get()->ObjectBrushList[ObjectID]);

	ObjectIndex = ObjectID;
	ObjectNum = Num;

	//�����ƷIDΪ0
	if (ObjectIndex == 0) {
		ObjectNumText->SetText(FText::FromString(""));
	}
	else 
	{
		//�ж���Ʒ�Ƿ���Ե���,�ǵĻ���ʾ����
		if (MultiplyAble(ObjectIndex)) {
			//��ʾ����
			ObjectNumText->SetText(FText::FromString(FString::FromInt(ObjectNum)));
		}
		else 
		{
			//���ɵ��ӵĻ�����ʾ����
			ObjectNumText->SetText(FText::FromString(""));
		}
	}
}

int SSlAiContainerBaseWidget::GetIndex() const
{
	return ObjectIndex;
}

int SSlAiContainerBaseWidget::GetNum() const
{
	return ObjectNum;
}

void SSlAiContainerBaseWidget::LeftOperate(int InputID, int InputNum, int& OutputID, int& OutputNum)
{
	//���������Ʒ�뱾����Ʒ��ͬ���ҿ��Ե���
	if (InputID == ObjectIndex && MultiplyAble(ObjectIndex)) 
	{
		//���������жϷ��ص�ID
		OutputID = (ObjectNum + InputNum <= 64) ? 0 : InputID;
		//���С��64,����0, �����򷵻ز�ֵ
		OutputNum = (ObjectNum + InputNum <= 64) ? 0 : (ObjectNum + InputNum - 64);
		//���ñ�������,����Ϊ64
		ObjectNum = (ObjectNum + InputNum <= 64) ? (ObjectNum + InputNum) : 64;
		//��������
		ResetContainerPara(ObjectIndex, ObjectNum);
		//ֱ�ӷ���
		return;
	}

	//ֱ�Ӹ�������
	OutputID = ObjectIndex;
	OutputNum = ObjectNum;
	//��������
	ResetContainerPara(InputID, InputNum);
}

void SSlAiContainerBaseWidget::RightOperate(int InputID, int InputNum, int& OutputID, int& OutputNum)
{
	//�������Ϊ��,ֱ�Ӱѱ��ص�һ�����ȥ,ʹ�ý�һ��
	if (InputID == 0)
	{
		OutputID = ObjectIndex;
		//���ֵ���˫��
		OutputNum = (ObjectNum % 2 == 1) ? (ObjectNum / 2 + 1) : (ObjectNum / 2);
		//��������
		ResetContainerPara(ObjectNum - OutputNum == 0 ? 0 : ObjectIndex, ObjectNum - OutputNum);
		//ֱ�ӷ���
		return;
	}

	//�����Ʒ��ͬ������Ʒ���Ժϲ�  ���߱�����ƷΪ��,���һ��������
	if (ObjectIndex == 0 || (InputID == ObjectIndex && MultiplyAble(InputID)))
	{
		//���ݱ��������Ƿ񳬳���Χ���������
		OutputNum = (ObjectNum + 1 <= 64) ? (InputNum - 1) : InputNum;
		//���������Ƿ�Ϊ0�������ID
		OutputID = (OutputNum == 0) ? 0 : InputID;
		//��������
		ResetContainerPara(InputID, (ObjectNum + 1 <= 64) ? (ObjectNum + 1) : ObjectNum);
		//ֱ�ӷ���
		return;
	}

	//�����Ʒ����ͬ������ͬ���ǲ��ܺϲ�,ֱ�ӽ���
	OutputID = ObjectIndex;
	OutputNum = ObjectNum;
	//��������
	ResetContainerPara(InputID, InputNum);

}

bool SSlAiContainerBaseWidget::IsEmpty()
{
	return ObjectIndex == 0;
}

//������Ƿ�Ϊ�յ����,Ϊ�յ�������Ϸ��ķ������
bool SSlAiContainerBaseWidget::RemainSpace(int ObjectID)
{
	if (ObjectIndex == ObjectID && ObjectNum < 64 && MultiplyAble(ObjectIndex)) return true;
	return false;
}

void SSlAiContainerBaseWidget::AddObject(int ObjectID)
{
	if (ObjectIndex == 0)
	{
		ResetContainerPara(ObjectID, 1);
		return;
	}
	if (ObjectIndex == ObjectID && ObjectNum < 64 && MultiplyAble(ObjectIndex)) {
		ResetContainerPara(ObjectIndex, ObjectNum + 1);
	}
}

bool SSlAiContainerBaseWidget::MultiplyAble(int ObjectID)
{
	//��ȡ��Ʒ����
	TSharedPtr<ObjectAttribute> ObjectAttr = *SlAiDataHandle::Get()->ObjectAttrMap.Find(ObjectID);
	//�����Ƿ����������߹���
	return (ObjectAttr->ObjectType != EObjectType::Tool && ObjectAttr->ObjectType != EObjectType::Weapon);
}
