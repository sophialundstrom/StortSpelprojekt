#pragma once
#include "Animator.h"
#include "DirectXHelp.h"
#include <deque>
#include "Camera.h"

class AnimationStateMachine
{
private:
public:
	struct Clip
	{
		Animation* animation = nullptr;
		std::string startBone = "root";
		bool hold = false;
		bool transitionIn = false;
		bool transitionOut = false;
		float transition = 0.0f;
	};

	std::deque<Clip> queuedAnimations;
	Clip overrideAnimation;

	Animator* animator;

	ID3D11Buffer* structuredBuffer = nullptr;
	ID3D11ShaderResourceView* bufferSRV = nullptr;
	std::vector<Matrix> finalTransforms;

	void CalculateOverrideAnimation(const aiScene* scene, Skeleton& skeleton, std::map<UINT, Matrix>& matrices);
	void CalculateCoreAnimation(const aiScene* scene, Skeleton& skeleton, std::map<UINT, Matrix>& matrices);
public:
	AnimationStateMachine(Animator* animator, const aiScene* scene, Skeleton& skeleton);
	~AnimationStateMachine();

	void Update(Skeleton& skeleton, const aiScene* scene, Camera* camera, const Quaternion& modelRotation, const std::string& rotationJoint = "");

	void PlayAnimation(const std::string& name);
	void PlayOverrideAnimation(const std::string& name, const std::string& startBone, bool hold = true, bool fullImpact = false);
	
	void BindMatrices() { Graphics::Inst().GetContext().VSSetShaderResources(0, 1, &bufferSRV); }
};