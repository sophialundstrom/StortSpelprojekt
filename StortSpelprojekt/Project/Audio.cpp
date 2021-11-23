#include "Audio.h"

Microsoft::WRL::ComPtr<IXAudio2> Audio::MusicEngine;
IXAudio2MasteringVoice* Audio::pMasterVoice = nullptr;
WAVEFORMATEXTENSIBLE Audio::wfx = { 0 };
XAUDIO2_BUFFER Audio::audioBuffer[CAP] = { 0, 0, 0, 0,0,0,0,0,0,0, 0,0,0,0,0 };
IXAudio2SourceVoice* Audio::pSourceVoice[CAP] = { nullptr };
float Audio::volume = 0.5f;

void Audio::StartEngine()
{
	MusicEngine->StartEngine();
}

void Audio::StopEngine()
{
	MusicEngine->StopEngine();
}

void Audio::SetVolume(float volume, int slot)
{
	pSourceVoice[slot]->SetVolume(volume);
}

void Audio::StartAudio(int slot)
{
	pSourceVoice[slot]->Stop(0);
	pSourceVoice[slot]->FlushSourceBuffers();
	HRESULT hr;
	if (FAILED(hr = pSourceVoice[slot]->SubmitSourceBuffer(&audioBuffer[slot])))
		std::cout << "COULD NOT SUBMIT SOURCE BUFFER" << std::endl;
	if(FAILED(hr = pSourceVoice[slot]->Start(0)))
		std::cout << "COULD NOT START AUDIO" << std::endl;
}

void Audio::Initialize()
{	
	HRESULT hr;

	if(FAILED(hr = XAudio2Create(&MusicEngine, 0 , XAUDIO2_DEFAULT_PROCESSOR)))
		std::cout << "COULD NOT CREATE AUDIO ENGINE" << std::endl;

	if (FAILED(hr = MusicEngine->CreateMasteringVoice(&pMasterVoice)))
		std::cout << "COULD NOT CREATE MASTERING VOICE" << std::endl;
}

void Audio::AddAudio(std::wstring fileName, int slot, bool repeat)
{
	LPCWSTR strFileName = fileName.c_str();
	
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

	audioBuffer[slot].AudioBytes = audioChunkSize;
	audioBuffer[slot].pAudioData = dataBuffer;
	audioBuffer[slot].Flags = XAUDIO2_END_OF_STREAM;
	if (repeat)
		audioBuffer[slot].LoopCount = XAUDIO2_MAX_LOOP_COUNT;
	else
		audioBuffer[slot].LoopCount = 0;

	HRESULT hr;
	if (FAILED(hr = MusicEngine->CreateSourceVoice(&pSourceVoice[slot], (WAVEFORMATEX*)&wfx, 0, XAUDIO2_DEFAULT_FREQ_RATIO, NULL, NULL, NULL)))
	{
		ERROR("FAILED TO CREATE SOURCE VOICE");
	}
	if(FAILED(hr = pSourceVoice[slot]->SubmitSourceBuffer(&audioBuffer[slot])))
		std::cout << "COULD NOT SUBMIT SOURCE BUFFER" << std::endl;

	pSourceVoice[slot]->SetVolume(volume);
}

void Audio::StopAudio(int slot)
{
	if (pSourceVoice[slot] == nullptr)
		return;
	HRESULT hr;
	if (FAILED(hr = pSourceVoice[slot]->Stop(0)))
		std::cout << "COULD NOT START AUDIO" << std::endl;
}
