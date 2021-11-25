#pragma once
#include <xaudio2.h>
#include <iostream>
#include <wrl.h>
#include "AudioChunks.h"
#include <vector>

enum class AUDIOTYPE {MUSIC, EFFECT, VOICE};

class Audio
{
private:
	static const int CAP = 18;
	static Microsoft::WRL::ComPtr<IXAudio2> MusicEngine;
	static IXAudio2MasteringVoice* pMasterVoice;
	static IXAudio2SourceVoice* pSourceVoice[CAP];
	static WAVEFORMATEXTENSIBLE wfx;
	static XAUDIO2_BUFFER audioBuffer[CAP];
	static float volume;

public:
	static std::vector<short int>musicSlots;
	static std::vector<short int>effectSlots;
	static std::vector<short int>voiceSlots;
	static void Initialize();
	static short int AddAudio(std::wstring fileName, short int slot, AUDIOTYPE type, bool repeat = false);
	static void StopAudio(int slot);
	static void StartEngine();
	static void StopEngine();
	static void SetVolume(float volume, int slot);
	static float GetVolume() { return volume; }
	static void StartAudio(int slot);
};