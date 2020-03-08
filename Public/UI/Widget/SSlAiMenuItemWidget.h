// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlAiTypes.h"
#include "Widgets/SCompoundWidget.h"

//1定义委托 
DECLARE_DELEGATE_OneParam(FItemClicked, const EMenuItem::Type)
//创建MenuItemWidget 不是物品栏 是主界面插槽； 头文件不引用会冲突
/**
 *
 */
class SLAICOURSE_API SSlAiMenuItemWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSlAiMenuItemWidget)
	{}

	SLATE_ATTRIBUTE(FText, ItemText)
		//2写个事件的委托传进去 从SSlAiMenuWidget传入OnClicked 
		SLATE_EVENT(FItemClicked, OnClicked)

		SLATE_ATTRIBUTE(EMenuItem::Type, ItemType)

		SLATE_END_ARGS()

		/** Constructs this widget with InArgs */
		void Construct(const FArguments& InArgs);

	//重写组件的OnMouseButtonDown方法
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;// 对应HUD蓝图里的Function
	//重写按钮起来的方法
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	//鼠标离开
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;//在这里设置按钮的颜色


private:

	//获取颜色
	FSlateColor GetTintColor() const;

private:


	//获取MenuStyle
	const struct FSlAiMenuStyle *MenuStyle;

	//3按下事件委托
	FItemClicked OnClicked;
	//保存按钮类型
	EMenuItem::Type ItemType;

	//按钮是否已经按下
	bool IsMouseButtonDown;

};
