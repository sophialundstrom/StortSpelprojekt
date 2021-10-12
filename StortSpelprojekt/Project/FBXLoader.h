#pragma once
#include "MaterialLoader.h"
#include "FileSystem.h"
#include "ThreadPool.h"
#include "Time.h"

struct TempMeshData
{
	std::string name;
	UINT vertexCount;
	ID3D11Buffer* buffer;
};

class FBXLoader
{
private:
public:
	FBXLoader(const std::string& directory)
	{
		Timer timer;
		timer.Start();
		size_t numFBX = 0;
		std::vector<std::string> names;

		using directory_iterator = std::filesystem::directory_iterator;
		for (const auto& dirEntry : directory_iterator("Models/"))
		{
			if (dirEntry.path().extension() == ".fbx")
			{
				names.emplace_back(dirEntry.path().string());
				numFBX++;
			}
		}

		std::vector<TempMeshData*> tempMeshData = std::vector<TempMeshData*>(numFBX);
		std::vector<Material*> tempMaterials = std::vector<Material*>(numFBX);

		{
			ThreadPool pool(10);
			
			for (UINT i = 0; i < numFBX; ++i) {
				pool.Enqueue([=, &tempMaterials, &tempMeshData] {

					Assimp::Importer importer;
					const aiScene* scene = importer.ReadFile(names[i], aiProcess_FlipUVs);
					if (!scene)
					{
						Print("COULD NOT LOAD .FBX FILE: " + names[i]);
						return;
					}

					if (scene->HasMaterials())
						tempMaterials[i] = LoadMaterial(scene->mMeshes[0]->mName.C_Str(), scene->mMaterials[0]);

					if (scene->HasMeshes())
						tempMeshData[i] = LoadMeshData(scene->mMeshes[0]);

					});

			}
		}

		// render to screen 

		PassToResources(tempMeshData, tempMaterials);

		Print(timer.DeltaTime());
	}

private:
	void PassToResources(const std::vector<TempMeshData*>& tempMeshData, const std::vector<Material*>& tempMaterials)
	{
		auto& resources = Resources::Inst();

		for (auto* meshData : tempMeshData)
			resources.AddVertexBuffer(meshData->name, meshData->buffer, meshData->vertexCount);

		for (auto* material : tempMaterials)
			resources.AddMaterial(material);
	}

	Material* LoadMaterial(const std::string& meshName, aiMaterial* material)
	{
		Material* newMaterial = new Material();

		newMaterial->name = meshName + "_" + material->GetName().C_Str();

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