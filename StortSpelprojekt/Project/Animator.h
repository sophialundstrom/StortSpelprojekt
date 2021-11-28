#pragma once
#include "Animation.h"

class Animator
{
	friend class AnimationStateMachine;
private:
	Animation* currentAnimation = nullptr;
	std::map<std::string, Animation*> animations;

	std::vector<Matrix> jointMatrices;

	ID3D11Buffer* structuredBuffer = nullptr;
	ID3D11ShaderResourceView* bufferSRV = nullptr;

	void SetBindPose(const aiScene* scene, Skeleton& skeleton)
	{
		jointMatrices = std::vector<Matrix>(MAX_JOINTS);

		const aiNode* root = scene->mRootNode->FindNode(skeleton.joints[0].name.c_str());
		if (!root)
			return;

		skeleton.transforms.clear();
		skeleton.SetBindPose(root, Matrix::Identity);
		skeleton.SetBuffer(root);

		UpdateBuffer(structuredBuffer, jointMatrices.data(), (UINT)jointMatrices.size() * sizeof(Matrix));
	}
public:
	Animator(const aiScene* scene, Skeleton& skeleton)
	{
		jointMatrices = std::vector<Matrix>(MAX_JOINTS);
		CreateStructuredBuffer(structuredBuffer, bufferSRV, sizeof(Matrix), sizeof(Matrix) * MAX_JOINTS);
		SetBindPose(scene, skeleton);
	}

	~Animator()
	{
		structuredBuffer->Release();
		bufferSRV->Release();
		for (auto& [name, animation] : animations)
			delete animation;
	}

	void AddAnimation(aiAnimation* animation)
	{
		animations.emplace(animation->mName.C_Str(), new Animation(animation));
	}

	void PlayAnimation(const std::string& name, bool onRepeat, float speedFactor = 1.f)
	{
		if (currentAnimation == animations[name]) // compare if they are the same
			return;

		currentAnimation = animations[name];
		currentAnimation->Play(onRepeat);
		currentAnimation->speedFactor = speedFactor;
	}

	bool HasActiveAnimation() { if (currentAnimation) return true; return false; }

	void ReadNodeHeriarchy(const aiNode* node, Skeleton& skeleton, const Matrix& parentTransform)
	{
		const std::string nodeName(node->mName.C_Str());
		Matrix globalTransform = parentTransform;

		if (nodeName.find('_') == std::string::npos)
		{
			Matrix localMatrix;

			currentAnimation->Update(nodeName, localMatrix);

			if (localMatrix == Matrix::Identity)
				localMatrix = AssimpToDX(node->mTransformation);

			globalTransform = localMatrix * parentTransform;

			const Matrix finalMatrix = (skeleton.FindJoint(nodeName).offsetMatrix * globalTransform).Transpose();

			jointMatrices.emplace_back(finalMatrix);
			skeleton.transforms.emplace_back(globalTransform);
		}

		for (UINT i = 0; i < node->mNumChildren; ++i)
			ReadNodeHeriarchy(node->mChildren[i], skeleton, globalTransform);
	}

	void ReadNodeHeriarchy(Animation* animation, const aiNode* node, Skeleton& skeleton, const Matrix& parentTransform, float weight, std::map<std::string, Matrix>& matrices)
	{
		const std::string nodeName(node->mName.C_Str());

		Matrix globalTransform = parentTransform;

		if (nodeName.find('_') == std::string::npos)
		{
			Matrix localMatrix;

			animation->Update(nodeName, localMatrix);

			if (localMatrix == Matrix::Identity)
				localMatrix = AssimpToDX(node->mTransformation);

			globalTransform = localMatrix * parentTransform;

			if (weight != 1.0f)
				matrices[nodeName] += globalTransform * weight;
			else
				matrices[nodeName] = globalTransform;
		}

		for (UINT i = 0; i < node->mNumChildren; ++i)
			ReadNodeHeriarchy(animation, node->mChildren[i], skeleton, globalTransform, weight, matrices);
	}

	void Update(Animation* animation, const aiScene* scene, Skeleton& skeleton, const std::string& startBone, float weight, std::map<std::string, Matrix>& matrices)
	{
		const aiNode* root = scene->mRootNode->FindNode(skeleton.FindJoint(startBone).name.c_str());
		if (!root)
			return;

		ReadNodeHeriarchy(animation, root, skeleton, Matrix::Identity, weight, matrices);

		animation->timer += Time::GetDelta();
	}

	void Update(const aiScene* scene, Skeleton& skeleton)
	{
		if (!currentAnimation)
			return;

		if (currentAnimation->active == false)
		{
			currentAnimation = nullptr;
			SetBindPose(scene, skeleton);
			return;
		}

		jointMatrices.clear();
		skeleton.transforms.clear();

		const aiNode* root = scene->mRootNode->FindNode(skeleton.joints[0].name.c_str());
		if (!root)
			return;

		Matrix matrix = Matrix::Identity;

		ReadNodeHeriarchy(root, skeleton, matrix);

		skeleton.SetBuffer(root);

		UpdateBuffer(structuredBuffer, jointMatrices.data(), (UINT)jointMatrices.size() * sizeof(Matrix));
	}

	void BindMatricesBuffer()
	{
		Graphics::Inst().GetContext().VSSetShaderResources(0, 1, &bufferSRV);
	}

	const std::vector<Matrix>& GetJointMatrices() { return jointMatrices; }
};