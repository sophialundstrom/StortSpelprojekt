#pragma once
#include <xaudio2.h>
#include <iostream>
#include <wrl.h>
#include "AudioChunks.h"

class Audio
{
private:
	static const int CAP = 8;
	static Microsoft::WRL::ComPtr<IXAudio2> MusicEngine;
	static IXAudio2MasteringVoice* pMasterVoice;
	static IXAudio2SourceVoice* pSourceVoice[CAP];
	static WAVEFORMATEXTENSIBLE wfx;
	static XAUDIO2_BUFFER audioBuffer;
	static float volume;

public:
	static void Initialize();
	static void AddAudio(std::wstring fileName, int slot);
	static void StopAudio(int slot);
	static void StartEngine();
	static void StopEngine();
	static void SetVolume(float volume, int slot);
	static float GetVolume() { return volume; }
	static void StartAudio(int slot);
};