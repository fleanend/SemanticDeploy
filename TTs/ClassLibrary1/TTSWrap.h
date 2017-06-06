
#pragma once


#include <iostream>
#include <stdlib.h>
#include <string>
#include <vcclr.h> 
using namespace System;
using namespace System::Collections;
using namespace System::IO;


namespace TTSWrap {

	public ref class TTSWrap
	{
	public:
		static int TextToPCM(wchar_t* text, unsigned char* buffer, int textsize, int voiceIndex, bool voiceIntroduce) {
			
			//buffersize 40000 byte di PCM per ogni byte di lettera: euristica di Edoardo Ferrante©
			int buffersize = 40000 * textsize; //massima lunghezza del file audio
			
			//Prendo MemoryStream con dentro PCM del testo convertito in audio
			int audioLength; //la vera lunghezza del file audio
			MemoryStream^ memStream = gcnew MemoryStream(buffersize);
			String^ str = gcnew String(text);

			memStream = TTS::TextToSpeechHandler::TextToSpeech(str, voiceIndex, voiceIntroduce);
		
			audioLength = memStream->Length;

			//traduce i dati dal flusso di memoria ad un array di caratteri
			array<unsigned char>^ convert = memStream->GetBuffer();
			
			for (int i = 0; i < convert->Length; i++)
			{
				buffer[i] = convert[i];
			}

			memStream->Close();


			return audioLength;
		};
		static int TextToPCM(wchar_t* text, unsigned char* buffer, int textsize, int voiceIndex) {
			return TextToPCM(text, buffer, textsize, voiceIndex, false);
		};
		static int TextToPCM(wchar_t* text, unsigned char* buffer, int textsize) {
			return TextToPCM(text, buffer, textsize, 0, false);
		};
	};
}
