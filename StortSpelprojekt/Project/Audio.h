#pragma once
#include <xaudio2.h>
#include <iostream>

class Audio
{
private:
	IXAudio2* pXAudio2 = nullptr;
	IXAudio2MasteringVoice* pMasterVoice = nullptr;
public:
	Audio();
	~Audio();
	void Initialize();
};