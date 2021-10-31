#include "Audio.h"

Microsoft::WRL::ComPtr<IXAudio2> Audio::pXAudio2;
IXAudio2MasteringVoice* Audio::pMasterVoice = nullptr;
WAVEFORMATEXTENSIBLE Audio::wfx = { 0 };
XAUDIO2_BUFFER Audio::audioBuffer = { 0 };
IXAudio2SourceVoice* Audio::pSourceVoice = nullptr;
float Audio::volume = 0.5f;

void Audio::StartEngine()
{
	pXAudio2->StartEngine();
}

void Audio::StopEngine()
{
	pXAudio2->StopEngine();
}

void Audio::SetVolume(float volume)
{
	pSourceVoice->SetVolume(volume);
}

void Audio::StartAudio()
{
	HRESULT hr;
	if(FAILED(hr=pSourceVoice->Start(0)))
		std::cout << "COULD NOT START AUDIO" << std::endl;
}

void Audio::Initialize()
{	
	HRESULT hr;

	//hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	//if (FAILED(hr))
	//	std::cout << "COULD NOT COINITIALIZE" << std::endl;

	if(FAILED(hr = XAudio2Create(&pXAudio2, 0 , XAUDIO2_DEFAULT_PROCESSOR)))
		std::cout << "COULD NOT CREATE AUDIO ENGINE" << std::endl;

	if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasterVoice)))
		std::cout << "COULD NOT CREATE MASTERING VOICE" << std::endl;
}

void Audio::AddAudio(std::wstring fileName)
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
	audioBuffer.LoopCount = XAUDIO2_MAX_LOOP_COUNT;

	HRESULT hr;
	if (FAILED(hr = pXAudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&wfx, 0, XAUDIO2_DEFAULT_FREQ_RATIO, NULL, NULL, NULL)))
	{
		ERROR("FAILED TO CREATE SOURCE VOICE");
	}
	if(FAILED(hr = pSourceVoice->SubmitSourceBuffer(&audioBuffer)))
		std::cout << "COULD NOT SUBMIT SOURCE BUFFER" << std::endl;

	pSourceVoice->SetVolume(5);
}
