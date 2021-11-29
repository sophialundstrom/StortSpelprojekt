#include "GameLoader.h"
#include <algorithm>

void GameLoader::Load(const std::string& filename, std::map<std::string, std::shared_ptr<Drawable>>& drawables)
{
	readLocation = 0;
	std::string path = FileSystem::ProjectDirectory::path + "\\SaveData\\" + filename + ".objs";

	reader.open(path, std::ios::beg | std::ios::binary);
	if (!reader.is_open())
	{
		printf("FATAL ERROR READER IS NOT OPEN!");
		return;
	}

	reader.seekg(0, reader.end);
	UINT fileLength = (UINT)reader.tellg();
	reader.seekg(0, reader.beg);

	UINT header;
	while (readLocation < fileLength)
	{
		Read(header);

		switch((TYPE)header)
		{
			case MODEL: 
			{
				auto model = ReadModel();
				drawables.emplace(model->name, model);
			
				break;
			}
			
			case PARTICLE_SYSTEM:
			{
				break;
			}

			case PARENT:
			{
				char drawable[MAX_STR];
				ReadStr(drawable);

				char parent[MAX_STR];
				ReadStr(parent);

				drawables[drawable]->parent = drawables[parent];
				break;
			}

			case VOLUMEBOX:
			{
				auto box = ReadBoxVolume();
				drawables.emplace(box->GetName(), box);
				break;
			}

			case VOLUMESPHERE:
			{
				auto sphere = ReadSphereVolume();
				drawables.emplace(sphere->GetName(), sphere);
				break;
			}
		}
	}

	reader.close();
}

void GameLoader::Save(const std::string& filename, const std::map<std::string, std::shared_ptr<Drawable>>& drawables)
{
	std::string saveFileLocatoin = FileSystem::ProjectDirectory::path + "\\SaveData\\" + filename + ".objs";

	writer.open(saveFileLocatoin, std::ios::trunc | std::ios::binary);
	if (!writer.is_open())
	{
		printf("FATAL ERROR WRITER IS NOT OPEN!");
		return;
	}

	char message[MAX_STR];

	for (auto& [name, drawable] : drawables)
	{
		auto model = std::dynamic_pointer_cast<Model>(drawable);

		if (model)
		{
			Write(MODEL);

			strcpy_s(message, MAX_STR, name.c_str());
			WriteStr(message);
			std::cout << "Writing modelName: " << message << std::endl;

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
			std::cout << "ScaleData" << model->scale.x << " " << model->scale.y << " " << model->scale.z << std::endl;
			continue;
		}

		auto boundingVolume = std::dynamic_pointer_cast<BoundingVolume>(drawable);
		if (boundingVolume)
		{
			if (boundingVolume->type == VolumeType::BOX)
				Write(VOLUMEBOX);
			else
				Write(VOLUMESPHERE);

			strcpy_s(message, MAX_STR, name.c_str());
			WriteStr(message);
			Write(boundingVolume->GetMatrix());
			Write(boundingVolume->GetPosition());
			Write(boundingVolume->GetRotation());
			Write(boundingVolume->GetScale());
		}

	}

	for (auto& [name, drawable] : drawables)
	{
		if (!drawable->parent)
			continue;

		Write(PARENT);

		Print("PARENT");

		strcpy_s(message, MAX_STR, name.c_str());
		WriteStr(message);

		Print(message);

		strcpy_s(message, MAX_STR, drawable->parent->name.c_str());
		WriteStr(message);

		Print(message);
	}

	writer.close();
}

std::shared_ptr<Model> GameLoader::ReadModel()
{
	auto model = std::make_shared<Model>();

	char string[MAX_STR];

	//ModelName
	ReadStr(string);	
	model->SetName(string);

	//MeshName
	ReadStr(string);	
	model->ApplyMesh(string);

	//Material
	ReadStr(string);	
	model->ApplyMaterial(string);

	Vector3 vec;

	Read(vec);	//PosData
	model->SetPosition(vec);
	
	Quaternion quaternion;
	Read(quaternion);  //RotData
	model->SetRotation(quaternion);

	Read(vec);  //ScaleData
	model->SetScale(vec);

	return model;
}

std::shared_ptr<BoundingBox> GameLoader::ReadBoxVolume()
{
	char string[MAX_STR];

	//ModelName
	ReadStr(string);
	Matrix matrix;
	Read(matrix);
	auto box = std::make_shared<BoundingBox>(matrix.Transpose());
	box->SetName(string);

	Vector3 vector;
	Read(vector);
	box->SetPosition(vector);

	Quaternion quaternion;
	Read(quaternion);
	box->SetRotation(quaternion);

	Read(vector);
	box->SetScale(vector);

	return box;
}

std::shared_ptr<BoundingSphere> GameLoader::ReadSphereVolume()
{
	char string[MAX_STR];

	//ModelName
	ReadStr(string);
	Matrix matrix;
	Read(matrix);

	auto sphere = std::make_shared<BoundingSphere>(matrix.Transpose());
	sphere->SetName(string);

	Vector3 vector;
	Read(vector);
	sphere->SetPosition(vector);

	Quaternion quaternion;
	Read(quaternion);
	sphere->SetRotation(quaternion);

	Read(vector);
	sphere->SetScale(vector);

	return sphere;
}
