// Fill out your copyright notice in the Description page of Project Settings.

#include "MicrosoftSpeechTTS.h"
#include "MSAudioContainer.h"

UMSAudioContainer::UMSAudioContainer(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{

}


UMSAudioContainer* UMSAudioContainer::Initialize(FString Text)
{
	TTSSoundWave = NewObject<UMSSoundWave>();
	TTSSoundWave->Initialize(Text);
	
	TTSAudioComponent = NewObject<UAudioComponent>();
	TTSAudioComponent->SetSound(TTSSoundWave);
	TTSAudioComponent->AttachTo(this);
	return this;
}

void UMSAudioContainer::Play()
{
	TTSAudioComponent->Play();
}

UMSAudioContainer::~UMSAudioContainer()
{
	delete TTSAudioComponent;
	delete TTSSoundWave;
}
