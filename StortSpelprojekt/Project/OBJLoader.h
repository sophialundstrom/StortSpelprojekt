#pragma once
#include "Mesh.h"
#include "Resources.h"

namespace OBJLoader
{
	inline void LoadModel(std::string file, std::vector<Mesh*>& meshes, Bounds& bounds)
	{
		//STORING DATA
		std::vector<Vector3> v;
		std::vector<Vector2> vt;
		std::vector<Vector3> vn;

		std::ifstream reader;
		reader.open("Models/" + file + "/" + file + ".obj", std::ios::beg);
		if (!reader.is_open())
		{
			Print("FAILED TO OPEN OBJ-FILE: " + file + ".obj");
			return;
		}

		Mesh* mesh = nullptr;				//START WITH NULL MESH

		std::string mtllib = "";
		std::string lastMaterial = "";
		std::string lastHeader = "";

		while (!reader.eof())
		{
			std::string text;
			reader >> text;					//READ FIRST STRING IN LINE

			if (text == "mtllib")
			{
				lastHeader = text;

				reader >> mtllib;

				Resources::CreateMaterialsFromFile(file, mtllib);		//CREATE MATERIAL FROM MTLLIB
			}

			if (text == "usemtl")
			{
				if (lastHeader == "f")
					meshes.push_back(mesh);		//PUSH BACK LAST MESH TO START A NEW ONE

				lastHeader = text;

				mesh = new Mesh(mtllib);

				reader >> lastMaterial;

				mesh->materialID = Resources::GetMaterialID(mtllib, lastMaterial);
			}

			if (text == "v")
			{
				if (lastHeader == "f")
					meshes.push_back(mesh);

				lastHeader = text;

				Vector3 position;

				reader >> position.x;
				reader >> position.y;
				reader >> position.z;

				//X
				if (position.x < bounds.Xmin)
					bounds.Xmin = position.x;
				if (position.x > bounds.Xmax)
					bounds.Xmax = position.x;
				//Y
				if (position.y < bounds.Ymin)
					bounds.Ymin = position.y;
				if (position.y > bounds.Ymax)
					bounds.Ymax = position.y;
				//Z
				if (position.z < bounds.Zmin)
					bounds.Zmin = position.z;
				if (position.z > bounds.Zmax)
					bounds.Zmax = position.z;

				v.push_back(position);
			}

			if (text == "vt")
			{
				lastHeader = text;

				Vector2 texCoord;

				reader >> texCoord.x;
				reader >> texCoord.y;

				texCoord.y = 1 - texCoord.y;

				vt.push_back(texCoord);
			}

			if (text == "vn")
			{
				lastHeader = text;

				Vector3 normal;

				reader >> normal.x;
				reader >> normal.y;
				reader >> normal.z;

				vn.push_back(normal);
			}

			if (text == "f")
			{
				if (lastHeader == "vn")		//IF NO MATERIAL ASSIGNED, START NEW MESH AND APPLY LAST MATERIAL
				{
					mesh = new Mesh(mtllib);
					mesh->materialID = Resources::GetMaterialID(mtllib, lastMaterial);
				}

				lastHeader = text;

				Face face;

				std::string temp;
				for (UINT i = 0; i < 3; ++i) // FORMAT: V/VT/VN X3
				{
					reader >> temp;

					unsigned int positionID = std::stoi(temp.substr(0, temp.find_first_of("/")));
					unsigned int texCoordsID = std::stoi(temp.substr(temp.find_first_of("/") + 1, temp.find_last_of("/")));
					unsigned int normalID = std::stoi(temp.substr(temp.find_last_of("/") + 1, temp.length()));

					face.vertices[i] = Vertex(v[positionID - 1u], vt[texCoordsID - 1u], vn[normalID - 1u]);
				}

				if (mesh)
					mesh->faces.push_back(face);
			}

			if (text == "")
				meshes.push_back(mesh); //LAST LINE
		}

		for (auto& mesh : meshes)
			CreateVertexBuffer(mesh->vertexBuffer, mesh->stride, mesh->stride * (UINT)mesh->faces.size() * 3, mesh->faces.data());
	}
}