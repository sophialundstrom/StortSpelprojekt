#pragma once
#include "AssimpDirectXMathConverter.h"
#include "AnimatedVertex.h"
#include "Resources.h"
#include "Skeleton.h"

#include "assimp\scene.h"

struct AnimatedMesh
{
	std::string name = "";

	UINT bufferID = -1;
	UINT materialID = -1;
	UINT vertexCount = -1;

	AnimatedMesh() = default;
	AnimatedMesh(aiMesh* mesh, Skeleton& skeleton, const aiScene* scene)
		:name(mesh->mName.C_Str())
	{
		auto& resources = Resources::Inst();
		std::vector<AnimatedVertex> vertices(mesh->mNumVertices);

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

		for (UINT i = 0; i < mesh->mNumBones; ++i)
		{
			aiBone* bone = mesh->mBones[i];

			Joint joint =
			{
				bone->mName.C_Str(),
				AssimpToDX(bone->mOffsetMatrix)
			};

			skeleton.joints.emplace_back(joint);

			for (UINT j = 0; j < bone->mNumWeights; ++j)
			{
				UINT vertexID = bone->mWeights[j].mVertexId;
				float weight = bone->mWeights[j].mWeight;

				for (UINT k = 0; k < 4; ++k)
				{
					if (vertices[vertexID].boneIDs[k] == UNDEF)
					{
						vertices[vertexID].boneIDs[k] = i;
						vertices[vertexID].weights[k] = weight;
						break;
					}
				}
			}
		}

		vertexCount = (UINT)vertices.size();

		bufferID = (UINT)Resources::Inst().NumBuffers();

		ID3D11Buffer* buffer;
		CreateVertexBuffer(buffer, sizeof(AnimatedVertex), sizeof(AnimatedVertex) * vertexCount, vertices.data());

		resources.AddVertexBuffer(mesh->mName.C_Str(), buffer, vertexCount);
	}
};