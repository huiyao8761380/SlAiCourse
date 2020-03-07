// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

struct ChatMessItem;

DECLARE_DELEGATE_TwoParams(FPushMessage, FText, FText)
/**
 * 
 */
class SLAICOURSE_API SSlAiChatRoomWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSlAiChatRoomWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	//�ύ�¼�
	void SubmitEvent(const FText& SubmitText, ETextCommit::Type CommitType);

	//��ť�¼�
	FReply SendEvent();

	//������Ϣ
	void AddMessage(FText NewName, FText NewContent);

	//�����������
	void ScrollToEnd();

public:

	FPushMessage PushMessage;

private:

	//��ȡGameStyle
	const struct FSlAiGameStyle *GameStyle;

	//������
	TSharedPtr<class SScrollBox> ScrollBox;

	//���������
	TSharedPtr<class SEditableTextBox> EditTextBox;

	//��������
	TArray<TSharedPtr<ChatMessItem>> MessageList;

};