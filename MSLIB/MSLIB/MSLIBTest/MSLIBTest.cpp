// MSLIBTest.cpp : definisce il punto di ingresso dell'applicazione console.
//

#include "stdafx.h"
#include "TextToSpeech.h"
#include <fstream>


int main()
{
	ULONG bytes = 0;
	char* AudioBuffer = TextToSpeech::TextToSpeech::TextToPCM(L"Stefano is a butt!", &bytes);

	std::ofstream ofs;
	ofs.open( "sound.raw", std::ios::out | std::ios :: binary );
	ofs.write( AudioBuffer , bytes );    
	ofs.close();

	return 0;
}

