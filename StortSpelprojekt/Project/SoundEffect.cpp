#include "SoundEffect.h"

Microsoft::WRL::ComPtr<IXAudio2> SoundEffect::SoundEffectEngine;
IXAudio2MasteringVoice* SoundEffect::pMasterVoice = nullptr;
WAVEFORMATEXTENSIBLE SoundEffect::wfx = { 0 };
XAUDIO2_BUFFER SoundEffect::audioBuffer = { 0 };
IXAudio2SourceVoice* SoundEffect::pSourceVoice[CAP] = { nullptr };
float SoundEffect::volume = 0.5f;

void SoundEffect::StartEngine()
{
	SoundEffectEngine->StartEngine();
}

void SoundEffect::StopEngine()
{
	SoundEffectEngine->StopEngine();
}

void SoundEffect::SetVolume(float volume, int slot)
{
	pSourceVoice[slot]->SetVolume(volume);
}

void SoundEffect::StartAudio(int slot)
{
	HRESULT hr;
	if (FAILED(hr = pSourceVoice[slot]->Start(0)))
		std::cout << "COULD NOT START AUDIO" << std::endl;
}

void SoundEffect::Initialize()
{
	HRESULT hr;

	if (FAILED(hr = XAudio2Create(&SoundEffectEngine, 0, XAUDIO2_DEFAULT_PROCESSOR)))
		std::cout << "COULD NOT CREATE AUDIO ENGINE" << std::endl;

	if (FAILED(hr = SoundEffectEngine->CreateMasteringVoice(&pMasterVoice)))
		std::cout << "COULD NOT CREATE MASTERING VOICE" << std::endl;
}

void SoundEffect::AddAudio(std::wstring fileName, int slot, bool repeat)
{
	LPCWSTR strFileName = fileName.c_str();
	Initialize();

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

	audioBuffer.AudioBytes = audioChunkSize;
	audioBuffer.pAudioData = dataBuffer;
	audioBuffer.Flags = XAUDIO2_END_OF_STREAM;
	if (repeat)
		audioBuffer.LoopCount = XAUDIO2_MAX_LOOP_COUNT;
	else
		audioBuffer.LoopCount = 0;

	HRESULT hr;
	if (FAILED(hr = SoundEffectEngine->CreateSourceVoice(&pSourceVoice[slot], (WAVEFORMATEX*)&wfx, 0, XAUDIO2_DEFAULT_FREQ_RATIO, NULL, NULL, NULL)))
	{
		ERROR("FAILED TO CREATE SOURCE VOICE");
	}
	if (FAILED(hr = pSourceVoice[slot]->SubmitSourceBuffer(&audioBuffer)))
		std::cout << "COULD NOT SUBMIT SOURCE BUFFER" << std::endl;

	pSourceVoice[slot]->SetVolume(volume);
}

void SoundEffect::StopAudio(int slot)
{
	if (pSourceVoice[slot] == nullptr)
		return;
	HRESULT hr;
	if (FAILED(hr = pSourceVoice[slot]->Stop(0)))
		std::cout << "COULD NOT START AUDIO" << std::endl;
}
