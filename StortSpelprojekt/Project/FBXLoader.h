#pragma once
#include "MaterialLoader.h"
#include "ThreadPool.h"
#include <filesystem>

struct TempMeshData
{
	std::string name;
	UINT ID;
	UINT vertexCount;
	ID3D11Buffer* buffer;
};

class FBXLoader
{
private:
	Assimp::Importer importer;
	std::vector<TempMeshData*> tempMeshData;
	std::vector<Material*> tempMaterials;

public:
	FBXLoader(const std::string& directory)
	{
		//EX
		size_t numFBX = 0;
		std::vector<std::string> names;

		using directory_iterator = std::filesystem::directory_iterator;
		for (const auto& dirEntry : directory_iterator("Models/")) 
		{
			std::string path(dirEntry.path().u8string());
			if (path.substr(path.find_last_of(".") + 1) == "fbx")
			{
				names.emplace_back(path.substr(path.find_last_of("/") + 1));
				numFBX++;
			}
		}

		tempMeshData = std::vector<TempMeshData*>(numFBX);
		tempMaterials = std::vector<Material*>(numFBX);

		ThreadPool pool(10);

		for (UINT i = 0; i < numFBX; ++i){

			pool.Enqueue([=, &tempMeshData, &tempMaterials] {

				const aiScene* scene = importer.ReadFile("Models/" + names[i], aiProcess_FlipUVs);
				if (!scene)
				{
					Print("COULD NOT LOAD .FBX FILE: " + names[i]);
					return;
				}

				if (scene->HasMaterials())
					tempMaterials[i] = LoadMaterial(scene->mMaterials[0]);

				if (scene->HasMeshes())
					tempMeshData[i] = LoadMeshData(scene->mMeshes[0]);

				});

		}
		
		// render to screen 

		
		//CALL THINGY TO ACTUALLY CALL ALL RESOURCE INITS

		PassToResources();
	}

private:

	void LoadFBX(const std::string& fbx, UINT ID)
	{
		const aiScene* scene = importer.ReadFile("Models/" + fbx, aiProcess_FlipUVs);
		if (!scene)
		{
			Print("COULD NOT LOAD .FBX FILE: " + fbx);
			return;
		}

		if (scene->HasMaterials())
			tempMaterials[ID] = LoadMaterial(scene->mMaterials[0]);

		if (scene->HasMeshes())
			tempMeshData[ID] = LoadMeshData(scene->mMeshes[0]);
	}

	void PassToResources()
	{
		auto& resources = Resources::Inst();

		for (auto* meshData : tempMeshData)
		{
			resources.AddVertexBuffer(meshData->name, meshData->buffer, meshData->vertexCount);
			meshData->buffer = nullptr;
			delete meshData;
		}

		for (auto* material : tempMaterials)
		{
			resources.AddMaterial(material);
			material = nullptr;
		}

		tempMeshData.clear();
		tempMaterials.clear();
	}

	Material* LoadMaterial(aiMaterial* material)
	{
		Material* newMaterial = new Material();

		newMaterial->name = material->GetName().C_Str();

		aiString path;
		UINT numTextures = material->GetTextureCount(aiTextureType_DIFFUSE);
		if (numTextures > 0)
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				std::string fileName = std::string(path.C_Str()).substr(std::string(path.C_Str()).find_last_of("\\") + 1);
				newMaterial->diffuseTextures.emplace_back(new Texture("Textures/" + fileName, fileName));
			}

		float specularPower = 1.0f;
		aiColor3D rgb(0.0f, 0.0f, 0.0f);

		material->Get(AI_MATKEY_COLOR_DIFFUSE, rgb);
		newMaterial->data.diffuse = Vector4(rgb.r, rgb.g, rgb.b, 1.0f);

		material->Get(AI_MATKEY_COLOR_AMBIENT, rgb);
		newMaterial->data.ambient = Vector4(rgb.r, rgb.g, rgb.b, 1.0f);

		material->Get(AI_MATKEY_COLOR_SPECULAR, rgb);
		newMaterial->data.specular = Vector4(rgb.r, rgb.g, rgb.b, 1.0f);

		material->Get(AI_MATKEY_SHININESS, specularPower);
		newMaterial->data.specularPower = specularPower;

		return newMaterial;
	}

	TempMeshData* LoadMeshData(aiMesh* mesh)
	{
		TempMeshData* resource = new TempMeshData();

		resource->name = mesh->mName.C_Str();

		std::vector<Vertex> vertices(mesh->mNumVertices);
		for (UINT i = 0; i < mesh->mNumVertices; ++i)
		{
			aiVector3D val;

			val = mesh->mVertices[i];
			vertices[i].position = { val.x, val.y, val.z };

			val = mesh->mNormals[i];
			vertices[i].normal = { val.x, val.y, val.z };

			val = mesh->mTextureCoords[0][i];
			vertices[i].texCoords = { val.x, val.y };
		}

		resource->vertexCount = mesh->mNumVertices;

		//BUT WITH DEFERRED CONTEXT
		CreateVertexBuffer(resource->buffer, sizeof(Vertex), sizeof(Vertex) * resource->vertexCount, vertices.data());

		return resource;
	}
};