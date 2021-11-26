#pragma once
#include "Animator.h"
#include "DirectXHelp.h"

class AnimationStateMachine
{
private:
	struct State
	{
		Animation* animation = nullptr;
		std::string startBone = "root";
		float progress = 0.0f;
		bool loop = false;

		float weight = 1.0f;
	};

	std::map<std::string, State> activeAnimations;
	Animator* animator;

	ID3D11Buffer* structuredBuffer = nullptr;
	ID3D11ShaderResourceView* bufferSRV = nullptr;
	std::vector<Matrix> finalTransforms;
public:
	AnimationStateMachine(Animator* animator, const aiScene* scene, Skeleton& skeleton);

	void Update(Skeleton& skeleton, const aiScene* scene);

	void LoopAnimation(const std::string& name, const std::string& startBone = "root");
	void PlayAndHoldAnimation(const std::string& name, const std::string& startBone = "root");
	void PlayUntilEnd(const std::string& name, const std::string& startBone = "root");
	
	void BindMatrices() { Graphics::Inst().GetContext().VSSetShaderResources(0, 1, &bufferSRV); }
};