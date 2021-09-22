#pragma once
#include "AssimpDirectXMathConverter.h"
#include "Skeleton.h"
#include "assimp\scene.h"
#include <map>

struct Channel
{
	std::map<float, Vector3> positions;
	std::map<float, Quaternion> quaternions;
	std::map<float, Vector3> scalings;
};

struct Animation
{
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
		ticksPerSecond = animation->mTicksPerSecond;
		duration = animation->mDuration;

		for (UINT i = 0; i < animation->mNumChannels; ++i)
		{
			aiNodeAnim* aiChannel = animation->mChannels[i];

			std::string jointName = aiChannel->mNodeName.C_Str();
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

	void Update(const std::string& joint, Matrix& localMatrix)
	{
		if (!active)
			return;

		timer += Time::GetDelta();
		float timeInTicks = timer * ticksPerSecond;
		float frameTime = fmod(timeInTicks, duration);

		if (timer > duration)
		{
			timer = 0;
			active = false;
			return;
		}

		auto& map = channels[joint].positions;
		if (map.empty())
			return;

		auto lower = map.lower_bound(frameTime);
		//std::cout << lower->first << std::endl;

		//auto upper = map.upper_bound(timer);
		//std::cout << upper->first << std::endl;

		if (lower == map.end())
			lower = map.begin();
	
		const Matrix translation = Matrix::CreateTranslation(channels[joint].positions[lower->first]);
		const Matrix quaternion = Matrix::CreateFromQuaternion(channels[joint].quaternions[lower->first]);
		const Matrix scaling = Matrix::CreateScale(channels[joint].scalings[lower->first]);

		localMatrix = scaling * quaternion * translation;
	}

	void Play() 
	{
		active = true;
	}
};