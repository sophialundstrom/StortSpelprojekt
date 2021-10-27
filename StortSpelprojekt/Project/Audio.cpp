#include "Audio.h"
Audio::Audio()
{

}

Audio::~Audio()
{
}

void Audio::Initialize()
{	
	HRESULT hr;
	hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (FAILED(hr))
		std::cout << "COULD NOT COINITIALIZE" << std::endl;

	if(FAILED(hr = XAudio2Create(&pXAudio2, 0 , XAUDIO2_DEFAULT_PROCESSOR)))
		std::cout << "COULD NOT CREATE AUDIO ENGINE" << std::endl;

	if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasterVoice)))
		std::cout << "COULD NOT CREATE MASTERING VOICE" << std::endl;
}
