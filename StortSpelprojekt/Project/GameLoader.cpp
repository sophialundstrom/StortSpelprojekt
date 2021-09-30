#include "GameLoader.h"

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

	while (!reader.eof())
	{
		char* temp = new char[MAX_STR];
		ReadName(temp);
		std::cout << temp << std::endl;
	}

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

		//writer.write((const char*)&model->position, sizeof(Vector3));
		//writer.write((const char*)&model->rotation, sizeof(Vector3));
		//writer.write((const char*)&model->scale, sizeof(Vector3));
		//writer.write((const char*)&model->mesh.bufferID, sizeof(UINT));
		//writer.write((const char*)&model->mesh.materialID, sizeof(UINT));
		const char* temp = model->mesh.name.c_str();
		int size = model->mesh.name.length();
		std::cout << temp << std::endl;
		WriteName(temp);

	}

	writer.close();
}
