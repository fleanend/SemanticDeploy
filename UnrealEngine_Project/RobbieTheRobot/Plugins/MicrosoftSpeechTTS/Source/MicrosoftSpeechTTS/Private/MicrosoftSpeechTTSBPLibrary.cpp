// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "MicrosoftSpeechTTS.h"
#include "MicrosoftSpeechTTSBPLibrary.h"



UMicrosoftSpeechTTSBPLibrary::UMicrosoftSpeechTTSBPLibrary(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{

}

 UMSSoundWave* UMicrosoftSpeechTTSBPLibrary::Speak(FString Text)
{
	UMSSoundWave* soundWave = NewObject<UMSSoundWave>();
	soundWave->Initialize(Text);
	return soundWave;

}