#pragma once
#include <fstream>
#include "Print.h"

class File
{
private:
	size_t readLocation = 0;
	size_t size = 0;
	std::ofstream output;
	std::ifstream input;
public:
	File(const std::string& path, bool input = true, bool binary = true)
	{
		if (input)
		{
			this->input.open(path, binary ? std::ios::binary : std::ios::beg);
			if (!this->input.is_open())
			{
				Print("FAILED TO OPEN FILE: " + path);
				return;
			}

			this->input.seekg(0, this->input.end);
			size = this->input.tellg();
			this->input.seekg(0, this->input.beg);
		}
			
		else
		{
			this->output.open(path, binary ? std::ios::binary : std::ios::beg);
			if (!this->output.is_open())
			{
				Print("FAILED TO OPEN FILE: " + path);
				return;
			}
		}
	}

	~File() { Close(); }

	void Close()
	{
		if (input.is_open())
			input.close();

		if (output.is_open())
			output.close();
	}

	size_t Size() { return size; }

	bool EndOfFile() { return (readLocation < size) ? false : true; }

	bool InputIsOpen()	{ return input.is_open(); }
	bool OutputIsOpen() { return output.is_open(); }

	//WRITE
	template <typename T>
	void Write(T data)
	{
		output.write((const char*)&data, sizeof(data));
	}

	void WriteString(const char* string)
	{
		size_t length = strlen(string) + 1;
		Write(length);
		output.write(string, length);
	}

	//READ
	template <typename T>
	void Read(T& data)
	{
		input.read((char*)&data, sizeof(data));
		readLocation += sizeof(data);
	}
	
	char* ReadString()
	{
		size_t length;
		Read(length);
		char* string = new char[length];
		input.read(string, length);
		readLocation += length;
		return string;
	}
};