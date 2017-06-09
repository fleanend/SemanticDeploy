// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/AudioComponent.h"
#include "MSSoundWave.h"
#include "MSAudioContainer.generated.h"

/**
 * 
 */
UCLASS()
class MICROSOFTSPEECHTTS_API UMSAudioContainer : public UAudioComponent
{
	GENERATED_BODY()
	
	public:
		UMSAudioContainer(const FObjectInitializer& ObjectInitializer);
		/** 
		 * Initialize the audio data and returns the initialized UTTSAudioContainer
		 * @param Text : FString - text to render
		 * @return this
		 */
		UMSAudioContainer* Initialize(FString Text);

		/** 
		 * Calls play() on the contained TTSAudioComponent
		 */
		UFUNCTION(BlueprintCallable, meta = (DisplayName = "Play TTS", Keywords = "MicrosoftSpeechTTS TTS Text-to-Speech Audio Speech"), Category="Text To Speech")
		void Play(float startTime);

		~UMSAudioContainer();
	private:
		UAudioComponent* TTSAudioComponent;
		UMSSoundWave* TTSSoundWave;

	
	
};
