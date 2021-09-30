#pragma once
#include "Animation.h"

class Animator
{
private:
	Animation* currentAnimation = nullptr;
	std::map<std::string, Animation*> animations;
	std::vector<Matrix> jointMatrices;

	ID3D11Buffer* structuredBuffer = nullptr;
	ID3D11ShaderResourceView* bufferSRV = nullptr;
public:
	Animator()
	{
		jointMatrices = std::vector<Matrix>(MAX_JOINTS);
		CreateStructuredBuffer(structuredBuffer, bufferSRV, sizeof(Matrix), sizeof(Matrix) * MAX_JOINTS);
	}

	void AddAnimation(aiAnimation* animation) 
	{ 
		animations.emplace(animation->mName.C_Str(), new Animation(animation)); 
	}

	void PlayAnimation(const std::string& name)
	{
		currentAnimation = animations[name];
		currentAnimation->Play();
	}

	void ReadNodeHeriarchy(const aiNode* node, const Skeleton& skeleton, const Matrix& parentTransform)
	{
		const std::string nodeName(node->mName.C_Str());

		Matrix localMatrix;
		currentAnimation->Update(nodeName, localMatrix);

		const Matrix globalTransform = localMatrix * parentTransform;

		const Matrix finalMatrix = (globalTransform * skeleton.FindJoint(nodeName).offsetMatrix).Transpose();

		jointMatrices.emplace_back(finalMatrix);

		for (UINT i = 0; i < node->mNumChildren; ++i)
			ReadNodeHeriarchy(node->mChildren[i], skeleton, globalTransform);
	}

	void Update(const aiScene* scene, const Skeleton& skeleton)
	{
		if (!currentAnimation)
			return;

		if (currentAnimation->active == false)
		{
			currentAnimation = nullptr;
			jointMatrices = std::vector<Matrix>(MAX_JOINTS);
			return;
		}

		jointMatrices.clear();

		const aiNode* root = scene->mRootNode->FindNode(skeleton.joints[0].name.c_str());

		Matrix matrix = Matrix::Identity;

		ReadNodeHeriarchy(root, skeleton, matrix);

		UpdateBuffer(structuredBuffer, jointMatrices.data(), (UINT)jointMatrices.size() * sizeof(Matrix));
	}

	void BindMatricesBuffer()
	{
		Graphics::Inst().GetContext().VSSetShaderResources(0, 1, &bufferSRV);
	}

	const std::vector<Matrix>& GetJointMatrices() { return jointMatrices; }
};