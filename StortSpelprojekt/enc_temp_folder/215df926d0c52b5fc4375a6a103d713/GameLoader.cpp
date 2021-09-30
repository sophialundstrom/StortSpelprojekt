#include "GameLoader.h"
#include <algorithm>

void GameLoader::Load(const std::string& filename)
{

	readLocation = 0;
	std::string saveFileLocatoin = FileSystem::ProjectDirectory::path + "\\SaveData\\" + filename + ".map";


	reader.open(saveFileLocatoin, std::ios::beg | std::ios::binary);
	if (!reader.is_open())
	{
		printf("FATAL ERROR READER IS NOT OPEN!");
		return;
	}

	reader.seekg(0, reader.end);
	UINT fileLength = reader.tellg();
	reader.seekg(0, reader.beg);

	while (readLocation < fileLength)
	{
		char message[MAX_STR];
		ReadStr(message);
		std::cout<< "read from file: " << message << std::endl;
		ReadStr(message);
		std::cout << "read from file: " << message << std::endl;
		ReadStr(message);
		std::cout << "read from file: " << message << std::endl;
		ReadStr(message);
		std::cout << "read from file: " << message << std::endl;
		ReadStr(message);
		std::cout << "read from file: " << message << std::endl;
		
		Vector3 vec;
		Read(vec);
		std::cout << "PosData " << vec.x << " " << vec.y << " " << vec.z << std::endl;

		Read(vec);
		std::cout << "RotData " << vec.x << " " << vec.y << " " << vec.z << std::endl;

		Read(vec);
		std::cout << "ScalData " << vec.x << " " << vec.y << " " << vec.z << std::endl;

		std::cout << "\n";
	}
	std::cout << "Closing reader\n";
	reader.close();

}

void GameLoader::Save(const std::string& filename, const std::map<std::string, std::shared_ptr<Drawable>>& drawables)
{
	std::string saveFileLocatoin = FileSystem::ProjectDirectory::path + "\\SaveData\\" + filename + ".map";

	
	writer.open(saveFileLocatoin, std::ios::trunc | std::ios::binary);
	if (!writer.is_open())
	{
		printf("FATAL ERROR WRITER IS NOT OPEN!");
		return;
	}

	for (auto& [name, drawable] : drawables)
	{
		auto model = std::dynamic_pointer_cast<Model>(drawable);


		char message[MAX_STR];

		strcpy_s(message, MAX_STR, name.c_str());
		WriteStr(message);
		std::cout << "Writing modelName: " << message << std::endl;

		if (!model->parent)
		{
			WriteStr("NO PARENT");
		}
		else
		{
			strcpy_s(message, MAX_STR, model->parent->name.c_str());
			WriteStr(message);
			std::cout << "Writing parentName " << message << std::endl;
		}

		

		strcpy_s(message, MAX_STR, model->mesh.name.c_str());
		WriteStr(message);
		std::cout << "Writing meshName: " << message << std::endl;

		strcpy_s(message, MAX_STR, Resources::Inst().GetBufferNameFromID(model->mesh.bufferID).c_str());
		WriteStr(message);
		std::cout << "Writing Buffer: " << message << std::endl;

		strcpy_s(message, MAX_STR, Resources::Inst().GetmaterialnamefromID(model->mesh.materialID).c_str());
		WriteStr(message);
		std::cout << "Writing material: " << message << std::endl;

		Write(model->position);
		std::cout << "PosData" << model->position.x << " " << model->position.y << " " << model->position.z << std::endl;

		Write(model->rotation);
		std::cout << "RotData" << model->rotation.x << " " << model->rotation.y << " " << model->rotation.z << std::endl;

		Write(model->scale);
		std::cout << "ScalData" << model->scale.x << " " << model->scale.y << " " << model->scale.z << std::endl;

		std::cout << "\n";
	}

	std::cout << "\n\n\n---\n\n\n";

	writer.close();
}
