#pragma once
#include <xaudio2.h>
#include <iostream>
#include <wrl.h>
#include "AudioChunks.h"

class Audio
{
private:
	static Microsoft::WRL::ComPtr<IXAudio2> pXAudio2;
	static IXAudio2MasteringVoice* pMasterVoice;
	static IXAudio2SourceVoice* pSourceVoice;
	static WAVEFORMATEXTENSIBLE wfx;
	static XAUDIO2_BUFFER audioBuffer;
	static float volume;

public:
	static void Initialize();
	static void AddAudio(std::wstring fileName);
	static void StartEngine();
	static void StopEngine();
	static void SetVolume(float volume);
	static float GetVolume() { return volume; }
	static void StartAudio();
};