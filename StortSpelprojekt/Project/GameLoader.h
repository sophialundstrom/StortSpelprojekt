#pragma once
#include<string>
#include<fstream>
#include"Scene.h"
#include"FileSystem.h"

#define MAX_STR 64

class GameLoader
{
public:
	//GameLoader();
	void Load(const std::string& filename);
	void Save(const std::string& filename, const std::map<std::string, std::shared_ptr<Drawable>>& drawables);
	


private:
	size_t readLocation = 0;
	std::ifstream reader;
	std::ofstream writer;

	template <typename T>
	void Write(T data)
	{
		writer.write((const char*)&data, sizeof(data));
	}

	template <typename T>
	void Read(T &data)
	{
		reader.read((char*)&data, sizeof(data));
		readLocation += sizeof(data);
	}

	void WriteStr(const char* name)
	{
		writer.write(name, MAX_STR);
	}

	void ReadStr(char* name)
	{
		reader.read(name, MAX_STR);
		readLocation += MAX_STR;
	}

	enum TYPE
	{
		MODEL
		
	};


};