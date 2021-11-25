#pragma once
#include <xaudio2.h>
#include <iostream>
#include <wrl.h>
#include "AudioChunks.h"
#include <vector>
#include <map>

enum class AUDIOTYPE {MUSIC, EFFECT, VOICE};

class Audio
{
private:
	static const int CAP = 18;
	static Microsoft::WRL::ComPtr<IXAudio2> MusicEngine;
	static IXAudio2MasteringVoice* pMasterVoice;
	static WAVEFORMATEXTENSIBLE wfx;
	static float volume;

public:
	static std::map<std::string, XAUDIO2_BUFFER> audioBuffers;
	static std::map<std::string, IXAudio2SourceVoice*> sMusic;
	static std::map<std::string, IXAudio2SourceVoice*> sEffects;
	static std::map<std::string, IXAudio2SourceVoice*> sVoices;
	static void Initialize();
	static short int AddAudio(const std::wstring& path, const std::string& name, short int slot, AUDIOTYPE type, bool repeat = false);
	static void StopMusic(const std::string& name);
	static void StopEffect(const std::string& name);
	static void StopVoice(const std::string& name);
	static void StartEngine();
	static void StopEngine();
	static void SetVolume(const std::string& name, float volume);
	static float GetVolume() { return volume; }
	static void StartMusic(const std::string& name);
	static void StartEffect(const std::string& name);
	static void StartVoice(const std::string& name);
};