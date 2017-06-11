#pragma once

#include <atlbase.h>
extern CComModule _Module;
#include <atlcom.h>
#include "sapi.h"
#pragma warning (disable:4996)
#include "sphelper.h"

namespace TextToSpeech
{
	class TextToSpeech
	{
		static void testHr(HRESULT hr);
	
	public:
		TextToSpeech();
		~TextToSpeech();
		static char* TextToPCM(wchar_t* textToRender, ULONG* bytesRead);


	};

}

