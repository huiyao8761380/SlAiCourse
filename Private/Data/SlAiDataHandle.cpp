// Fill out your copyright notice in the Description page of Project Settings.

#include "SlAiDataHandle.h"
#include "Internationalization.h"
#include "SlAiSingleton.h"
#include "SlAiJsonHandle.h"
#include "SlAiHelper.h"
#include "SlAiStyle.h"
#include "SlAiMenuWidgetStyle.h"
#include "SlAiGameWidgetStyle.h"
#include "Sound/SoundCue.h"

#include "AudioDevice.h"
#include "Sound/SoundMix.h"
#include "Sound/SoundClass.h"


TSharedPtr<SlAiDataHandle> SlAiDataHandle::DataInstance = NULL;

void SlAiDataHandle::Initialize()
{
	if (!DataInstance.IsValid()) {
		DataInstance = Create();
	}
}

TSharedPtr<SlAiDataHandle> SlAiDataHandle::Get()
{
	Initialize();
	return DataInstance;
}


TSharedRef<SlAiDataHandle> SlAiDataHandle::Create()
{
	TSharedRef<SlAiDataHandle> DataRef = MakeShareable(new SlAiDataHandle());
	return DataRef;
}

SlAiDataHandle::SlAiDataHandle()
{
	//��ʼ���浵����
	InitRecordData();
	//��ʼ����������
	InitializedMenuAudio();

}

void SlAiDataHandle::ChangeLocalizationCulture(ECultureTeam Culture)
{
	switch (Culture)
	{
	case ECultureTeam::EN:
		FInternationalization::Get().SetCurrentCulture(TEXT("en"));
		break;
	case ECultureTeam::ZH:
		FInternationalization::Get().SetCurrentCulture(TEXT("zh"));
		break;
	}
	//��ֵ
	CurrentCulture = Culture;
	//���´浵����
	SlAiSingleton<SlAiJsonHandle>::Get()->UpdateRecordData(GetEnumValueAsString<ECultureTeam>(FString("ECultureTeam"), CurrentCulture), MusicVolume, SoundVolume, &RecordDataList);
}


void SlAiDataHandle::ResetMenuVolume(float MusicVol, float SoundVol)
{
	if (MusicVol > 0)
	{
		MusicVolume = MusicVol;
		//ѭ�����ñ�������
		for (TArray<USoundCue*>::TIterator It(MenuAudioResource.Find(FString("Music"))->CreateIterator()); It; ++It)
		{
			//��������
			(*It)->VolumeMultiplier = MusicVolume;
		}
	}
	if (SoundVol > 0)
	{
		SoundVolume = SoundVol;
		for (TArray<USoundCue*>::TIterator It(MenuAudioResource.Find(FString("Sound"))->CreateIterator()); It; ++It)
		{
			//ָ���ָ��
			(*It)->VolumeMultiplier = SoundVolume;
		}
	}
	//���´浵����
	SlAiSingleton<SlAiJsonHandle>::Get()->UpdateRecordData(GetEnumValueAsString<ECultureTeam>(FString("ECultureTeam"), CurrentCulture), MusicVolume, SoundVolume, &RecordDataList);
}



void SlAiDataHandle::ResetGameVolume(float MusicVol, float SoundVol)
{
	if (MusicVol > 0)
	{
		MusicVolume = MusicVol;
		//ʹ�û�����������
		AudioDevice->SetSoundMixClassOverride(SlAiSoundMix, SlAiMusicClass, MusicVolume, 1.f, 0.2f, false);
	}
	if (SoundVol > 0)
	{
		SoundVolume = SoundVol;
		//ʹ�û�����������
		AudioDevice->SetSoundMixClassOverride(SlAiSoundMix, SlAiSoundClass, SoundVolume, 1.f, 0.2f, false);
	}
	//���´浵����
	SlAiSingleton<SlAiJsonHandle>::Get()->UpdateRecordData(GetEnumValueAsString<ECultureTeam>(FString("ECultureTeam"), CurrentCulture), MusicVolume, SoundVolume, &RecordDataList);
}

template<typename TEnum>
FString SlAiDataHandle::GetEnumValueAsString(const FString& Name, TEnum Value)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
	if (!EnumPtr) {
		return FString("InValid");
	}
	return EnumPtr->GetEnumName((int32)Value);
}

template<typename TEnum>
TEnum SlAiDataHandle::GetEnumValueFromString(const FString& Name, FString Value)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
	if (!EnumPtr) {
		return TEnum(0);
	}
	return (TEnum)EnumPtr->GetIndexByName(FName(*FString(Value)));
}

void SlAiDataHandle::InitRecordData()
{
	RecordName = FString("");
	//��ȡ����
	FString Culture;
	//��ȡ�浵����
	SlAiSingleton<SlAiJsonHandle>::Get()->RecordDataJsonRead(Culture, MusicVolume, SoundVolume, RecordDataList);

	//��ʼ������
	ChangeLocalizationCulture(GetEnumValueFromString<ECultureTeam>(FString("ECultureTeam"), Culture));


}

void SlAiDataHandle::InitializedMenuAudio()
{
	//��ȡMenuStyle
	MenuStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiMenuStyle>("BPSlAiMenuStyle");

	//������Դ�ļ�����Դ�б�
	TArray<USoundCue*> MusicList;
	MusicList.Add(Cast<USoundCue>(MenuStyle->MenuBackgroundMusic.GetResourceObject()));

	TArray<USoundCue*> SoundList;
	SoundList.Add(Cast<USoundCue>(MenuStyle->StartGameSound.GetResourceObject()));
	SoundList.Add(Cast<USoundCue>(MenuStyle->ExitGameSound.GetResourceObject()));
	SoundList.Add(Cast<USoundCue>(MenuStyle->MenuItemChangeSound.GetResourceObject()));

	//������Դ��Map
	MenuAudioResource.Add(FString("Music"), MusicList);
	MenuAudioResource.Add(FString("Sound"), SoundList);

	//����һ������
	ResetMenuVolume(MusicVolume, SoundVolume);
}

void SlAiDataHandle::InitializeGameData()
{
	//��ʼ����Ʒ����ͼ
	InitObjectAttr();
	//��ʼ����Դ����ͼ
	InitResourceAttrMap();
	//��ʼ���ϳɱ�ͼ
	InitCompoundTableMap();
	//��ʼ����Ϸ��������
	InitializeGameAudio();
}

void SlAiDataHandle::AddNewRecord()
{
	//�����ڵĴ浵�����ӵ�����
	RecordDataList.Add(RecordName);
	//����json����
	SlAiSingleton<SlAiJsonHandle>::Get()->UpdateRecordData(GetEnumValueAsString<ECultureTeam>(FString("ECultureTeam"), CurrentCulture), MusicVolume, SoundVolume, &RecordDataList);
}

void SlAiDataHandle::InitObjectAttr()
{
	SlAiSingleton<SlAiJsonHandle>::Get()->ObjectAttrJsonRead(ObjectAttrMap);
	//��ȡGameStyle
	GameStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiGameStyle>("BPSlAiGameStyle");
	//����ˢ����
	ObjectBrushList.Add(&GameStyle->EmptyBrush);
	ObjectBrushList.Add(&GameStyle->ObjectBrush_1);
	ObjectBrushList.Add(&GameStyle->ObjectBrush_2);
	ObjectBrushList.Add(&GameStyle->ObjectBrush_3);
	ObjectBrushList.Add(&GameStyle->ObjectBrush_4);
	ObjectBrushList.Add(&GameStyle->ObjectBrush_5);
	ObjectBrushList.Add(&GameStyle->ObjectBrush_6);
	ObjectBrushList.Add(&GameStyle->ObjectBrush_7);

	////��̬����Object��ͼƬBrush,��δ����������
	//for (int i = 1; i < ObjectAttrMap.Num(); ++i) {
	//	//���Ժ���,��̬����FSlateBrush,һ��Ҫ����ָ��,������ں�������ʱ������Դ
	//	FSlateBrush* ObjectBrush = new FSlateBrush();
	//	ObjectBrush->ImageSize = FVector2D(80.f, 80.f);
	//	ObjectBrush->DrawAs = ESlateBrushDrawType::Image;
	//	UTexture2D* ObjectTex = LoadObject<UTexture2D>(NULL, *(*ObjectAttrMap.Find(i))->TexPath);
	//	ObjectBrush->SetResourceObject(ObjectTex);
	//	ObjectBrushList.Add(ObjectBrush);
	//}
}

void SlAiDataHandle::InitResourceAttrMap()
{
	SlAiSingleton<SlAiJsonHandle>::Get()->ResourceAttrJsonRead(ResourceAttrMap);
}

void SlAiDataHandle::InitCompoundTableMap()
{
	SlAiSingleton<SlAiJsonHandle>::Get()->CompoundTableJsonRead(CompoundTableMap);
}

void SlAiDataHandle::InitializeGameAudio()
{
	//��ȡ������������
	SlAiSoundMix = LoadObject<USoundMix>(NULL, TEXT("SoundMix'/Game/Blueprint/Sound/SlAiSoundMix.SlAiSoundMix'"));
	SlAiMusicClass = LoadObject<USoundClass>(NULL, TEXT("SoundClass'/Game/Blueprint/Sound/SlAiMusicClass.SlAiMusicClass'"));
	SlAiSoundClass = LoadObject<USoundClass>(NULL, TEXT("SoundClass'/Game/Blueprint/Sound/SlAiSoundClass.SlAiSoundClass'"));

	//��ȡ�����豸
	AudioDevice = GEngine->GetMainAudioDevice();

	//���ͻ��������豸
	AudioDevice->PushSoundMixModifier(SlAiSoundMix);

	//������������һ������
	ResetGameVolume(MusicVolume, SoundVolume);
}
