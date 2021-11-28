#pragma once
#include "Animator.h"
#include "DirectXHelp.h"

class AnimationStateMachine
{
private:
	const float transitionTime = 0.5f;

	struct State
	{
		Animation* animation = nullptr;
		std::string startBone = "root";
		bool hold = false;
		bool transitionIn = false;
		bool transitionOut = false;
		float transition = 0.0f;
	};

	std::map<std::string, State> activeAnimations;
	State* latestAnimation;

	State* overrideAnimation;

	Animator* animator;

	ID3D11Buffer* structuredBuffer = nullptr;
	ID3D11ShaderResourceView* bufferSRV = nullptr;
	std::vector<Matrix> finalTransforms;
public:
	AnimationStateMachine(Animator* animator, const aiScene* scene, Skeleton& skeleton);

	void Update(Skeleton& skeleton, const aiScene* scene);

	void PlayAnimation(const std::string& name);
	void PlayOverrideAnimation(const std::string& name, const std::string& startBone, bool hold = true);
	
	void BindMatrices() { Graphics::Inst().GetContext().VSSetShaderResources(0, 1, &bufferSRV); }
};