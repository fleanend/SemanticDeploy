// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "MicrosoftSpeechTTS.h"
#include "MicrosoftSpeechTTSBPLibrary.h"



UMicrosoftSpeechTTSBPLibrary::UMicrosoftSpeechTTSBPLibrary(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{

}

 UMSAudioContainer* UMicrosoftSpeechTTSBPLibrary::Speak(FString Text)
{
	UMSAudioContainer* TTSAC = NewObject<UMSAudioContainer>();
	TTSAC->Initialize(Text);
	return TTSAC;

}