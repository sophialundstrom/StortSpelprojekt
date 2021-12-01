#include "AnimationStateMachine.h"

void AnimationStateMachine::CalculateOverrideAnimation(const aiScene* scene, Skeleton& skeleton, std::map<UINT, Matrix>& matrices)
{
	if (!overrideAnimation.animation)
		return;

	if (overrideAnimation.transition < 1.0f && overrideAnimation.transitionIn)
	{
		overrideAnimation.transition += Time::GetDelta() * 3.0f;
		if (overrideAnimation.transition > 1.0f)
			overrideAnimation.transition = 1.0f;
	}

	else if (overrideAnimation.transition > 0.0f && overrideAnimation.transitionOut)
	{
		overrideAnimation.transition -= Time::GetDelta() * 3.0f;
		if (overrideAnimation.transition < 0.0f)
			overrideAnimation.transition = 0.0f;
	}

	if (!overrideAnimation.animation->active && !overrideAnimation.hold)
		overrideAnimation.animation = nullptr;

	else
		animator->Update(overrideAnimation.animation, scene, skeleton, overrideAnimation.startBone, overrideAnimation.transition, matrices);
}

void AnimationStateMachine::CalculateCoreAnimation(const aiScene* scene, Skeleton& skeleton, std::map<UINT, Matrix>& matrices)
{
	if (queuedAnimations.size() == 1)
	{
		animator->Update(queuedAnimations.front().animation, scene, skeleton, queuedAnimations.front().startBone, 1.0f, matrices);
		return;
	}

	auto& firstClip = queuedAnimations.front();
	auto& secondClip = queuedAnimations[1];

	firstClip.transition -= Time::GetDelta() * 3.0f;

	if (firstClip.transition < 0.0f)
		firstClip.transition = 0.0f;

	secondClip.transition = 1.0f - firstClip.transition;

	animator->Update(firstClip.animation, scene, skeleton, firstClip.startBone, firstClip.transition, matrices);
	animator->Update(secondClip.animation, scene, skeleton, secondClip.startBone, secondClip.transition, matrices);

	if (firstClip.transition == 0.0f && firstClip.transitionOut)
		queuedAnimations.pop_front();
}

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

AnimationStateMachine::~AnimationStateMachine()
{
	structuredBuffer->Release();
	bufferSRV->Release();
}

void AnimationStateMachine::Update(Skeleton& skeleton, const aiScene* scene, Camera* camera, const Quaternion& modelRotation, const std::string& rotationJoint)
{
	if (queuedAnimations.empty())
		return;

	skeleton.transforms.clear();
	finalTransforms.clear();

	auto matrices = std::map<UINT, Matrix>();
	for (UINT i = 0; i < (UINT)skeleton.joints.size(); ++i)
		matrices[i] = Matrix(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	CalculateCoreAnimation(scene, skeleton, matrices);

	CalculateOverrideAnimation(scene, skeleton, matrices);
	
	for (auto& [joint, matrix] : matrices)
	{
		if (joint == skeleton.GetJointID(rotationJoint) && camera)
		{
			auto pitch = camera->GetPitch();

			auto maxMin = 10.0f * PI / 180.0f;

			if (cos(pitch) < 1.0f / sqrt(2.0f))
				pitch = acos(1.0f / sqrt(2.0f));

			if (pitch > maxMin)
				pitch = maxMin;

			Quaternion q = modelRotation;

			auto rotYaw = atan2(2 * q.y * q.w - 2 * q.x * q.z, 1 - 2 * q.y * q.y - 2 * q.z * q.z);
			rotYaw = (rotYaw < 0) ? (rotYaw + PI * 2) : rotYaw;

			auto yaw = 0.0f;// camera->GetJaw();

			//if (rotYaw > yaw)
			//{
			//	Print("PLAYER > CAMERA");
			//	if (rotYaw - yaw > PI_DIV4)
			//	{
			//		Print("SET YAW TO +");
			//		yaw = PI_DIV4;
			//	}
			//		
			//}

			//else
			//{
			//	Print("PLAYER < CAMERA");

			//	if (yaw -  rotYaw > PI_DIV4)
			//	{
			//		Print("SET YAW TO -");
			//		yaw = PI_DIV4;
			//	}
			//}

			////if (abs(rotYaw - yaw) > PI_DIV4)
			////{
			////	Print("TOO FAR");

			////	if (yaw > rotYaw)
			////		yaw = -PI_DIV4;
			////	else
			////		yaw = PI_DIV4;
			////}

			//yaw *= 0.5f;

			skeleton.PostTransformNodes(scene->mRootNode->FindNode(rotationJoint.c_str()), Matrix::CreateFromYawPitchRoll(yaw, pitch, 0.0f), matrices);
		}
		
		skeleton.transforms.emplace_back(matrix);
		finalTransforms.emplace_back((skeleton.joints[joint].offsetMatrix * matrix).Transpose());
	}

	skeleton.SetBuffer(scene->mRootNode->FindNode(skeleton.joints[0].name.c_str()));

	UpdateBuffer(structuredBuffer, finalTransforms.data(), (UINT)finalTransforms.size() * sizeof(Matrix));
}

void AnimationStateMachine::PlayAnimation(const std::string& name)
{
	auto animation = animator->animations[name];
	if (!animation)
		return;
	
	bool found = false;
	for (UINT i = 0; i < (UINT)queuedAnimations.size(); ++i)
	{
		if (queuedAnimations[i].animation == animation)
		{
			found = true;
			break;
		}
	}

	if (!found)
	{
		Clip clip
		{
			animation,
			"root",
			false,
			true,
			false,
			1.0f
		};

		animation->active = true;
		animation->repeat = true;

		//Print("PUSHED ANIMATION " + name);

		queuedAnimations.push_back(clip);
	}
	
	if (queuedAnimations.size() > 1)
	{
		queuedAnimations.front().transitionOut = true;
		queuedAnimations[1].transitionIn = true;
	}
}

void AnimationStateMachine::PlayOverrideAnimation(const std::string& name, const std::string& startBone, bool hold, bool fullImpact)
{
	auto animation = animator->animations[name];

	if ((!animation || animation == overrideAnimation.animation) && !fullImpact)
		return;

	animation->active = true;
	animation->timer = 0.0f;

	overrideAnimation.animation = animation;
	overrideAnimation.startBone = startBone;
	overrideAnimation.hold = hold;
	overrideAnimation.transitionIn = false;
	overrideAnimation.transitionOut = false;

	if (overrideAnimation.animation)
	{
		overrideAnimation.transition = 1.0f;
		if (!hold)
			overrideAnimation.transitionOut = true;
	}
		
	else
	{
		overrideAnimation.transition = 0.0f;
		overrideAnimation.transitionIn = true;
	}
	
	if (fullImpact)
		overrideAnimation.transition = 1.0f;
}