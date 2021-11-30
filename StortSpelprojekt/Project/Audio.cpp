#include "Audio.h"
#include "FileSystem.h"
#include "Text.h"
#include "ThreadPool.h"
#include "Time.h"
#include "Print.h"
#include "HardwareSupport.h"

Microsoft::WRL::ComPtr<IXAudio2> Audio::MusicEngine;
IXAudio2MasteringVoice* Audio::pMasterVoice = nullptr;
WAVEFORMATEXTENSIBLE Audio::wfx = { 0 };
std::map<std::string, IXAudio2SourceVoice*> Audio::sMusic;
std::map<std::string, IXAudio2SourceVoice*> Audio::sEffects;
std::map<std::string, IXAudio2SourceVoice*> Audio::sVoices;
std::map<std::string, XAUDIO2_BUFFER> Audio::audioBuffers;
float Audio::volume = 0.5f;
float Audio::masterVolume = 0.25f;
float Audio::musicVolume = 0.5f;
float Audio::effectsVolume = 0.5f;
float Audio::voiceVolume = 0.5f;

void Audio::StopAudio()
{
	/*for (auto& [key, audio] : sMusic)
	{
		audio
	}*/
}

bool Audio::MuteAllVolume()
{
	return false;
}

void Audio::SetMasterVolume(float volume)
{
	pMasterVoice->SetVolume(volume);
	masterVolume = volume;
	Print(masterVolume);
}

void Audio::SetMusicVolume(float volume)
{
	for (auto& [key, source] : sMusic)
	{
		source->SetVolume(volume);
		musicVolume = volume;
	}
	Print(volume);
}

void Audio::SetSoundEffectsVolume(float volume)
{
	for (auto& [key, source] : sEffects)
	{
		source->SetVolume(volume);
		effectsVolume = volume;
	}
}

void Audio::SetVoiceVolume(float volume)
{
	for (auto& [key, source] : sVoices)
	{
		source->SetVolume(volume);
		voiceVolume = volume;
	}
}

void Audio::StartEngine()
{
	MusicEngine->StartEngine();
}

void Audio::StopEngine()
{
	MusicEngine->StopEngine();
	sMusic.clear();
	sEffects.clear();
	sVoices.clear();
}

void Audio::SetVolume(const std::string& name, float volume)
{

	if (sMusic.count(name) == 1)
	{

		sMusic[name]->SetVolume(volume);
		return;
	}
	if (sEffects.count(name) == 1)
	{
		sEffects[name]->SetVolume(volume);
		return;
	}
	if (sVoices.count(name) == 1)
	{
		sVoices[name]->SetVolume(volume);
		return;
	}
}

void Audio::StartMusic(const std::string& name)
{
	for (auto& music : sMusic)
	{
		music.second->Stop(0);
	}
	
	sMusic[name]->FlushSourceBuffers();
	HRESULT hr;
	if (FAILED(hr = sMusic[name]->SubmitSourceBuffer(&audioBuffers[name])))
		std::cout << "COULD NOT START MUSIC - SOURCE BUFFER FAILED" << std::endl;
	if (FAILED(hr = sMusic[name]->Start(0)))
		std::cout << "COULD NOT START MUSIC - COULD NOT START AUDIO" << std::endl;
}

void Audio::StartEffect(const std::string& name)
{
	sEffects[name]->Stop(0);
	sEffects[name]->FlushSourceBuffers();
	HRESULT hr;
	if (FAILED(hr = sEffects[name]->SubmitSourceBuffer(&audioBuffers[name])))
		std::cout << "COULD NOT SUBMIT SOURCE BUFFER" << std::endl;
	if (FAILED(hr = sEffects[name]->Start(0)))
		std::cout << "COULD NOT START AUDIO" << std::endl;
}

void Audio::StartVoice(const std::string& name)
{
	sVoices[name]->Stop(0);
	sVoices[name]->FlushSourceBuffers();
	HRESULT hr;
	if (FAILED(hr = sVoices[name]->SubmitSourceBuffer(&audioBuffers[name])))
		std::cout << "COULD NOT SUBMIT SOURCE BUFFER" << std::endl;
	if (FAILED(hr = sVoices[name]->Start(0)))
		std::cout << "COULD NOT START AUDIO" << std::endl;
}

void Audio::Initialize(bool mtLoading)
{	

	HRESULT hr;

	if(FAILED(hr = XAudio2Create(&MusicEngine, 0 , XAUDIO2_DEFAULT_PROCESSOR)))
		std::cout << "COULD NOT CREATE AUDIO ENGINE" << std::endl;

	if (FAILED(hr = MusicEngine->CreateMasteringVoice(&pMasterVoice)))
		std::cout << "COULD NOT CREATE MASTERING VOICE" << std::endl;

	std::vector<std::string> names;

	using directory_iterator = std::filesystem::directory_iterator;

	if (mtLoading)
	{
		Timer timer;
		timer.Start();

		ThreadPool tp(HardwareSupport::numThreads);

		for (const auto& dirEntry : directory_iterator("Audio/Music/"))
		{
			if (dirEntry.path().extension() == ".wav")
			{
				std::string fileName = dirEntry.path().filename().string();
				std::wstring path = to_wstr("Audio/Music/" + fileName);
				tp.Enqueue([=] {

					AddAudio(path, fileName, 0, AUDIOTYPE::MUSIC, true);
					});
			}
		}
		for (const auto& dirEntry : directory_iterator("Audio/EffectsOnce/"))
		{
			if (dirEntry.path().extension() == ".wav")
			{
				std::string fileName = dirEntry.path().filename().string();
				std::wstring path = to_wstr("Audio/EffectsOnce/" + fileName);
				tp.Enqueue([=] {

					AddAudio(path, fileName, 0, AUDIOTYPE::EFFECT);
					});
			}
		}
		for (const auto& dirEntry : directory_iterator("Audio/EffectsRepeat/"))
		{
			if (dirEntry.path().extension() == ".wav")
			{
				std::string fileName = dirEntry.path().filename().string();
				std::wstring path = to_wstr("Audio/EffectsRepeat/" + fileName);
				tp.Enqueue([=] {

					AddAudio(path, fileName, 0, AUDIOTYPE::EFFECT, true);
					});
			}
		}
		PrintNumber(timer.DeltaTime(), "MULTITHREADED TIME: ");
	}
	else
	{
		Timer timer;
		timer.Start();
		for (const auto& dirEntry : directory_iterator("Audio/Music/"))
		{
			if (dirEntry.path().extension() == ".wav")
			{
				std::string fileName = dirEntry.path().filename().string();
				std::wstring path = to_wstr("Audio/Music/" + fileName);
				AddAudioNoneMT(path, fileName, 0, AUDIOTYPE::MUSIC, true);
			}
		}
		for (const auto& dirEntry : directory_iterator("Audio/EffectsOnce/"))
		{
			if (dirEntry.path().extension() == ".wav")
			{
				std::string fileName = dirEntry.path().filename().string();
				std::wstring path = to_wstr("Audio/EffectsOnce/" + fileName);
				AddAudioNoneMT(path, fileName, 0, AUDIOTYPE::EFFECT);
			}
		}
		for (const auto& dirEntry : directory_iterator("Audio/EffectsRepeat/"))
		{
			if (dirEntry.path().extension() == ".wav")
			{
				std::string fileName = dirEntry.path().filename().string();
				std::wstring path = to_wstr("Audio/EffectsRepeat/" + fileName);
				AddAudioNoneMT(path, fileName, 0, AUDIOTYPE::EFFECT, true);
			}
		}
		PrintNumber(timer.DeltaTime(), "SINGLETHREADED TIME: ");
	}

	StartEngine();

}

void Audio::AddAudioNoneMT(const std::wstring& path, const std::string& name, short int slot, AUDIOTYPE type, bool repeat)
{

	LPCWSTR strFileName = path.c_str();

	HANDLE hFile = CreateFile(strFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
		ERROR("INVALID HANDLE VALUE");

	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		ERROR("INVALID SET FILE POINTER");

	DWORD audioChunkSize;
	DWORD audioChunkPosition;
	FindChunk(hFile, fourccRIFF, audioChunkSize, audioChunkPosition);
	DWORD fileType;
	ReadChunkData(hFile, &fileType, sizeof(DWORD), audioChunkPosition);
	if (fileType != fourccWAVE)
		ERROR("This isn't WAVE file!\n");
	else ERROR("WAVE format!\n");

	FindChunk(hFile, fourccFMT, audioChunkSize, audioChunkPosition);
	ReadChunkData(hFile, &wfx, audioChunkSize, audioChunkPosition);

	FindChunk(hFile, fourccDATA, audioChunkSize, audioChunkPosition);
	BYTE* dataBuffer = new BYTE[audioChunkSize];
	ReadChunkData(hFile, dataBuffer, audioChunkSize, audioChunkPosition);

	XAUDIO2_BUFFER audioBuffer = {};

	audioBuffer.AudioBytes = audioChunkSize;
	audioBuffer.pAudioData = dataBuffer;
	audioBuffer.Flags = XAUDIO2_END_OF_STREAM;
	if (repeat)
		audioBuffer.LoopCount = XAUDIO2_MAX_LOOP_COUNT;
	else
		audioBuffer.LoopCount = 0;

	IXAudio2SourceVoice* pSource = nullptr;

	HRESULT hr;
	if (FAILED(hr = MusicEngine->CreateSourceVoice(&pSource, (WAVEFORMATEX*)&wfx, 0, XAUDIO2_DEFAULT_FREQ_RATIO, NULL, NULL, NULL)))
	{
		ERROR("FAILED TO CREATE SOURCE VOICE");
	}
	if (FAILED(hr = pSource->SubmitSourceBuffer(&audioBuffer)))
		std::cout << "COULD NOT SUBMIT SOURCE BUFFER" << std::endl;

	pSource->SetVolume(volume);

	audioBuffers[name] = audioBuffer;


	switch (type)
	{
	case AUDIOTYPE::MUSIC:
		sMusic[name] = pSource;
		break;
	case AUDIOTYPE::EFFECT:
		sEffects[name] = pSource;
		break;
	case AUDIOTYPE::VOICE:
		sVoices[name] = pSource;
		break;

	}

}

void Audio::AddAudio(const std::wstring& path, const std::string& name, short int slot, AUDIOTYPE type, bool repeat)
{
	static std::mutex lock;

	LPCWSTR strFileName = path.c_str();
	
	HANDLE hFile = CreateFile(strFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
		ERROR("INVALID HANDLE VALUE");

	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		ERROR("INVALID SET FILE POINTER");

	DWORD audioChunkSize;
	DWORD audioChunkPosition;
	FindChunk(hFile, fourccRIFF, audioChunkSize, audioChunkPosition);
	DWORD fileType;
	ReadChunkData(hFile, &fileType, sizeof(DWORD), audioChunkPosition);
	if (fileType != fourccWAVE)
		ERROR("This isn't WAVE file!\n");
	else ERROR("WAVE format!\n");

	FindChunk(hFile, fourccFMT, audioChunkSize, audioChunkPosition);
	ReadChunkData(hFile, &wfx, audioChunkSize, audioChunkPosition);

	FindChunk(hFile, fourccDATA, audioChunkSize, audioChunkPosition);
	BYTE* dataBuffer = new BYTE[audioChunkSize];
	ReadChunkData(hFile, dataBuffer, audioChunkSize, audioChunkPosition);

	XAUDIO2_BUFFER audioBuffer = {};

	audioBuffer.AudioBytes = audioChunkSize;
	audioBuffer.pAudioData = dataBuffer;
	audioBuffer.Flags = XAUDIO2_END_OF_STREAM;
	if (repeat)
		audioBuffer.LoopCount = XAUDIO2_MAX_LOOP_COUNT;
	else
		audioBuffer.LoopCount = 0;

	IXAudio2SourceVoice* pSource = nullptr;

	lock.lock();
	HRESULT hr;
	if (FAILED(hr = MusicEngine->CreateSourceVoice(&pSource, (WAVEFORMATEX*)&wfx, 0, XAUDIO2_DEFAULT_FREQ_RATIO, NULL, NULL, NULL)))
	{
		ERROR("FAILED TO CREATE SOURCE VOICE");
	}
	if(FAILED(hr = pSource->SubmitSourceBuffer(&audioBuffer)))
		std::cout << "COULD NOT SUBMIT SOURCE BUFFER" << std::endl;

	pSource->SetVolume(volume);
	
	audioBuffers[name] = audioBuffer;
	

	switch (type)
	{
	case AUDIOTYPE::MUSIC:
		sMusic[name] = pSource;
		break;
	case AUDIOTYPE::EFFECT:
		sEffects[name] = pSource;
		break;
	case AUDIOTYPE::VOICE:
		sVoices[name] = pSource;
		break;

	}	
	lock.unlock();

}

void Audio::StopMusic(const std::string& name)
{
	if (sMusic[name] == nullptr)
		return;
	HRESULT hr;
	if (FAILED(hr = sMusic[name]->Stop(0)))
		std::cout << "COULD NOT START AUDIO" << std::endl;
}

void Audio::StopEffect(const std::string& name)
{
	if (sEffects[name] == nullptr)
		return;
	HRESULT hr;
	if (FAILED(hr = sEffects[name]->Stop(0)))
		std::cout << "COULD NOT START AUDIO" << std::endl;
}

void Audio::StopVoice(const std::string& name)
{
	if (sVoices[name] == nullptr)
		return;
	HRESULT hr;
	if (FAILED(hr = sVoices[name]->Stop(0)))
		std::cout << "COULD NOT START AUDIO" << std::endl;
}