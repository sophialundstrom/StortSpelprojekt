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
}

void AnimationStateMachine::LoopAnimation(const std::string& name, const std::string& startBone)
{
}

void AnimationStateMachine::PlayAndHoldAnimation(const std::string& name, const std::string& startBone)
{
}

void AnimationStateMachine::PlayUntilEnd(const std::string& name, const std::string& startBone)
{
}
