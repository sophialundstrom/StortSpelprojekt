#pragma once
#include <Windows.h>
#include <filesystem>
#include <string>
#include "Print.h"

namespace FileSystem
{
	struct ProjectDirectory
	{
		static std::string path;
	};
	inline std::string ProjectDirectory::path = "";

	inline void SetProjectDirectory() { ProjectDirectory::path = std::filesystem::current_path().string(); }

	inline std::string LoadFile(std::string fileDirectory = "")
	{
		std::string path = ProjectDirectory::path + "\\" + fileDirectory;
		
		//OPEN EXPORER AND RETURN SELECTED FILENAME
		OPENFILENAMEA ofn;
		CHAR fileName[260] = { 0 };
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrInitialDir = LPCSTR(path.c_str());
		ofn.lpstrFile = fileName;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrTitle = "Select A File";
		ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

		if (GetOpenFileNameA(&ofn))
		{
			std::filesystem::path path(ofn.lpstrFile);
			return path.string();
		}

		else
		{
			Print("NO FILE SELECTED");
			return "";
		}
	}

	inline std::string SaveFile(std::string fileDirectory = "")
	{
		std::string path = ProjectDirectory::path + "\\" + fileDirectory;

		//OPEN EXPORER AND SAVE SELECTED FILENAME
		OPENFILENAMEA ofn;
		CHAR fileName[260] = { 0 };
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrInitialDir = LPCSTR(path.c_str());
		ofn.lpstrFile = fileName;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrTitle = "Select A File";
	
		if (GetSaveFileNameA(&ofn))
		{
			std::filesystem::path path(ofn.lpstrFile);
			return path.string();
		}

		else
		{
			Print("NO FILE SELECTED");
			return "";
		}
	}
}