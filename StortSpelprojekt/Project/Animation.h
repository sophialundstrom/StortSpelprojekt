#pragma once
#include "AssimpDirectXMathConverter.h"
#include "Skeleton.h"
#include "assimp\scene.h"
#include "Time.h"
#include <map>

struct Channel
{
	std::map<float, Vector3> positions;
	std::map<float, Quaternion> quaternions;
	std::map<float, Vector3> scalings;
};

struct Animation
{
	float speedFactor = 1.f;
	bool repeat = false;
	bool active = false;
	std::string name = "";
	float ticksPerSecond = 0;
	float duration = 0;
	float timer = 0;

	std::map<std::string, Channel> channels;

	Animation() = default;
	Animation(aiAnimation* animation)
	{
		name = animation->mName.C_Str();
		ticksPerSecond = (float)animation->mTicksPerSecond;
		duration = (float)animation->mDuration;
		for (UINT i = 0; i < animation->mNumChannels; ++i)
		{
			aiNodeAnim* aiChannel = animation->mChannels[i];

			std::string jointName = aiChannel->mNodeName.C_Str();
			Print(jointName);
			jointName = jointName.substr(0, jointName.find_first_of('_'));

			for (UINT j = 0; j < aiChannel->mNumPositionKeys; ++j)
			{

				aiVectorKey key = aiChannel->mPositionKeys[j];
				Vector3 value = AssimpToDX(key.mValue);
				float keyFrame = (float)key.mTime;

				if (keyFrame < 0)
					continue;

				channels[jointName].positions[keyFrame] = value;
			}

			for (UINT j = 0; j < aiChannel->mNumRotationKeys; ++j)
			{
				aiQuatKey key = aiChannel->mRotationKeys[j];
				Quaternion value = AssimpToDX(key.mValue);
				float keyFrame = (float)key.mTime;

				if (keyFrame < 0)
					continue;

				channels[jointName].quaternions[keyFrame] = value;
			}

			for (UINT j = 0; j < aiChannel->mNumScalingKeys; ++j)
			{
				aiVectorKey key = aiChannel->mScalingKeys[j];
				Vector3 value = AssimpToDX(key.mValue);
				float keyFrame = (float)key.mTime;

				if (keyFrame < 0)
					continue;

				channels[jointName].scalings[keyFrame] = value;
			}
		}
	}

	void UpdateTime()
	{
		timer += Time::GetDelta();
	}

	void Update(const std::string& joint, Matrix& localMatrix)
	{
		if (!active)
			return;

		float timeInTicks = timer * ticksPerSecond * speedFactor; 
		float frameTime = fmod(timeInTicks, duration);

		if (timeInTicks > duration)
		{
			timer = 0.0f;
			if (!repeat)
				active = false;
			return;
		}

		auto& map = channels[joint].positions;
		if (map.empty())
			return;

		auto lower = map.upper_bound(frameTime - 1);
		auto higher = map.upper_bound(frameTime);

		float lowerTimestamp = lower->first;
		float higherTimestamp = higher->first;
		float weight = (frameTime - lowerTimestamp) / (higherTimestamp - lowerTimestamp);

		Quaternion Q = Quaternion::Slerp(channels[joint].quaternions[lowerTimestamp], channels[joint].quaternions[higherTimestamp], weight);
		Vector3 T = Vector3::Lerp(channels[joint].positions[lowerTimestamp], channels[joint].positions[higherTimestamp], weight);
		Vector3 S = Vector3::Lerp(channels[joint].scalings[lowerTimestamp], channels[joint].scalings[higherTimestamp], weight);

		const Matrix translation = Matrix::CreateTranslation(T);
		const Matrix quaternion = Matrix::CreateFromQuaternion(Q);
		const Matrix scaling = Matrix::CreateScale(S);

		localMatrix = scaling * quaternion * translation;
	}

	void Play(const bool& onRepeat)
	{
		active = true;
		repeat = onRepeat;
	}

	void Stop()
	{
		active = false;
	}
};