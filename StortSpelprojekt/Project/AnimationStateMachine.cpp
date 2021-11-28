#include "AnimationStateMachine.h"

AnimationStateMachine::AnimationStateMachine(Animator* animator, const aiScene* scene, Skeleton& skeleton)
	:animator(animator)
{
	finalTransforms = std::vector<Matrix>(MAX_JOINTS);
	CreateStructuredBuffer(structuredBuffer, bufferSRV, sizeof(Matrix), sizeof(Matrix) * MAX_JOINTS);
	
	const aiNode* root = scene->mRootNode->FindNode(skeleton.joints[0].name.c_str());
	if (!root)
		return;

	skeleton.transforms.clear();
	skeleton.SetBindPose(root, Matrix::Identity);
	skeleton.SetBuffer(root);

	UpdateBuffer(structuredBuffer, finalTransforms.data(), (UINT)finalTransforms.size() * sizeof(Matrix));
}

void AnimationStateMachine::Update(Skeleton& skeleton, const aiScene* scene)
{
	skeleton.transforms.clear();
	finalTransforms.clear();

	auto matrices = std::map<std::string, Matrix>();

	for (auto& [animation, state] : activeAnimations)
	{
		if (state.transitionIn)
			state.transition += Time::GetDelta();
		if (state.transitionOut)
			state.transition -= Time::GetDelta();

		animator->Update(state.animation, scene, skeleton, state.startBone, 1.0f / (float)activeAnimations.size(), matrices);
	}

	if (overrideAnimation->animation)
	{
		if (!overrideAnimation->animation->active)
			overrideAnimation->animation = nullptr;

		else
			animator->Update(overrideAnimation->animation, scene, skeleton, overrideAnimation->startBone, 1.0f, matrices);
	}

	for (auto& [joint, matrix] : matrices)
	{
		skeleton.transforms.emplace_back(matrix);
		finalTransforms.emplace_back((skeleton.FindJoint(joint).offsetMatrix * matrix).Transpose());
	}

	skeleton.SetBuffer(scene->mRootNode->FindNode(skeleton.joints[0].name.c_str()));

	UpdateBuffer(structuredBuffer, finalTransforms.data(), (UINT)finalTransforms.size() * sizeof(Matrix));
}

void AnimationStateMachine::PlayAnimation(const std::string& name)
{
	if (activeAnimations.find(name) == activeAnimations.end())
	{
		auto animation = animator->animations[name];
		animation->timer = 0.0f;
		animation->active = true;

		activeAnimations[name].animation = animation;
		activeAnimations[name].transition = 0.0f;
		activeAnimations[name].transitionIn = true;
	}
}

void AnimationStateMachine::PlayOverrideAnimation(const std::string& name, const std::string& startBone, bool hold)
{
	auto animation = animator->animations[name];
	animation->active = true;
	animation->timer = 0.0f;

	overrideAnimation->animation = animation;
	overrideAnimation->startBone = startBone;
	overrideAnimation->hold = hold;
}