// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "MicrosoftSpeechTTS.h"
#include "MicrosoftSpeechTTSBPLibrary.h"

#define _ATL_APARTMENT_THREADED

#include <atlbase.h>
extern CComModule _Module;
#include <atlcom.h>
#include <sapi.h>
#pragma warning (disable:4996)
#include <sphelper.h>




UMicrosoftSpeechTTSBPLibrary::UMicrosoftSpeechTTSBPLibrary(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{

}

 UMSAudioContainer* UMicrosoftSpeechTTSBPLibrary::Speak(FString Text)
{
	UMSAudioContainer* TTSAC = NewObject<UMSAudioContainer>();
	TTSAC->Initialize(Text);
	return TTSAC;

}

void UMicrosoftSpeechTTSBPLibrary::testHr(HRESULT hr)
{
	if (FAILED(hr))
	{
		throw 10;
	}
}

 char* UMicrosoftSpeechTTSBPLibrary::TextToPCM(wchar_t* textToRender, ULONG* bytesRead)
{
	HRESULT hr = S_OK;
	CComPtr<ISpVoice> cpVoice; //Will send data to ISpStream
	CComPtr<ISpStream> cpStream; //Will contain IStream
	CComPtr<IStream> cpBaseStream; //Will contain raw data

	CComPtr<IEnumSpObjectTokens> cpIEnum;
	CComPtr<ISpObjectToken> cpToken;
	CComPtr<ISpObjectTokenCategory> cpTokenCat;

	GUID guidFormat;
	WAVEFORMATEX* pWavFormatEx = nullptr;

	try
	{
		if (FAILED(::CoInitialize(NULL)))
			return FALSE;

		hr = SpGetCategoryFromId(SPCAT_VOICES, &cpTokenCat);

		testHr(hr);

		hr = cpTokenCat->SetDefaultTokenId(L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Speech Server\\v11.0\\Voices\\Tokens\\TTS_MS_en-US_ZiraPro_11.0");
				
		testHr(hr);

		hr = cpVoice.CoCreateInstance(CLSID_SpVoice);

		testHr(hr);
		hr = cpVoice->SetVoice(cpToken);

		testHr(hr);

		// Create Stream
		hr = cpStream.CoCreateInstance(CLSID_SpStream);
		testHr(hr);

		// Create BaseStream
		hr = CreateStreamOnHGlobal(NULL, TRUE, &cpBaseStream);
		testHr(hr);

		// Set audio format
		hr = SpConvertStreamFormatEnum(SPSF_44kHz16BitMono, &guidFormat, &pWavFormatEx);
		testHr(hr);

		// Set BaseStream on Stream
		hr = cpStream->SetBaseStream(cpBaseStream, guidFormat, pWavFormatEx);
		cpBaseStream.Release();
		testHr(hr);

		// Set Voice output
		hr = cpVoice->SetOutput(cpStream, FALSE);
		testHr(hr);
			
		// Set voice falgs and generate audio
		SpeechVoiceSpeakFlags voiceFlags = SpeechVoiceSpeakFlags::SVSFlagsAsync;
		hr = cpVoice->Speak(textToRender, voiceFlags, NULL);
		testHr(hr);
		hr = cpVoice->WaitUntilDone(1000);
		testHr(hr);

		// Uncomment below to directly output speech

		//cpVoice->SetOutput(NULL, FALSE);
		//cpVoice->SpeakStream(cpStream, SPF_DEFAULT, NULL);

		// Reset stream position
		_LARGE_INTEGER a = { 0 };
		hr = cpStream->Seek(a, STREAM_SEEK_SET, NULL);

		// Get the base istream from the ispstream
		IStream* pIstream;
		hr = cpStream->GetBaseStream(&pIstream);

		// Calculate the size that is to be read
		STATSTG stats;
		pIstream->Stat(&stats, STATFLAG_NONAME);
		ULONG sSize = stats.cbSize.QuadPart;
			
		// Read the data in a buffer to be returned
		char* pBuffer = new char[sSize];
		pIstream->Read(pBuffer, sSize, bytesRead);

		// Release allocated COM
		cpStream.Release();
		cpVoice.Release();
		cpToken.Release();
		cpTokenCat.Release();
		cpIEnum.Release();
		::CoUninitialize();
		return pBuffer;
	}
	catch (int e)
	{
		if (e == 10)
		{
			// In case of errors, release allocations and return empty buffer
			char* pBuffer = new char[0];
			cpStream.Release();
			cpVoice.Release();
			cpToken.Release();
			cpTokenCat.Release();
			cpIEnum.Release();
			::CoUninitialize();
			return pBuffer;
		}
		else
		{
			throw e;
		}
	}
}