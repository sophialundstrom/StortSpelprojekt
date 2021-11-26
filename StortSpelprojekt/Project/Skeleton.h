#pragma once
#include "DirectXHelp.h"
#include "Math.h"
#include <vector>

#define MAX_JOINTS 30

struct Joint
{
	std::string name;
	Matrix offsetMatrix;
};

struct Skeleton
{
	ID3D11Buffer* jointBuffer = nullptr;

	std::vector<Matrix> transforms;

	const UINT stride = sizeof(Vector3);
	const UINT offset = 0;
	std::vector<Joint> joints;
	int tempIndex = 0;
	UINT vertexCount = 0;

	const Joint& FindJoint(const std::string& name) const
	{
		for (auto& joint : joints)
			if (joint.name == name)
				return joint;

		return joints[0];
	}

	void SetVertex(const aiNode* node, std::vector<Vector3>& vertices)
	{
		bool extraNode = false;
		if (std::string(node->mName.C_Str()).find('_') != std::string::npos)
			extraNode = true;

		Vector3 position;
		if (!extraNode)
		{
			position = Vector3::Transform(position, transforms[tempIndex]);
			tempIndex++;

			if (node->mName.C_Str() != joints[0].name)
				vertices.emplace_back(position);

			if (node->mNumChildren == 0)
				return;
		}

		for (UINT i = 0; i < node->mNumChildren; ++i)
		{
			if (!extraNode)
				vertices.emplace_back(position);
			SetVertex(node->mChildren[i], vertices);
		}
	}

	void SetBindPose(const aiNode* node, const Matrix& parentMatrix)
	{
		Matrix worldMatrix = AssimpToDX(node->mTransformation) * parentMatrix;

		transforms.emplace_back(worldMatrix);

		for (UINT i = 0; i < node->mNumChildren; ++i)
			SetBindPose(node->mChildren[i], worldMatrix);
	}

	void SetBuffer(const aiNode* root)
	{
		if (!jointBuffer)
			CreateDynamicVertexBuffer(jointBuffer, sizeof(Vector3), sizeof(Vector3) * MAX_JOINTS * 3);

		std::vector<Vector3> vertices;
		SetVertex(root, vertices);
		UpdateDynamicVertexBuffer(jointBuffer, (UINT)vertices.size() * sizeof(Vector3), vertices.data());

		vertexCount = (UINT)vertices.size();

		tempIndex = 0;
	}

	void BindBuffer()
	{
		if (!jointBuffer)
			return;

		Graphics::Inst().GetContext().IASetVertexBuffers(0, 1, &jointBuffer, &stride, &offset);
		Graphics::Inst().GetContext().Draw(vertexCount, 0);
	}
};