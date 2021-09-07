#pragma once
#include "AssimpDirectXMathConverter.h"
#include "Skeleton.h"
#include "assimp\scene.h"
#include <map>

struct Channel
{
	std::string name;
	std::map<float, Vector3> positions;
	std::map<float, Quaternion> quaternions;
	std::map<float, Vector3> scalings;
};

class Animation
{
	std::string name;
	float ticksPerSecond;
	float duration;

	std::vector<Channel> channels;

	Animation(aiAnimation* animation)
	{
		ticksPerSecond = animation->mTicksPerSecond;
		duration = animation->mDuration;

		for (UINT i = 0; i < animation->mNumChannels; ++i)
		{
			aiNodeAnim* aiChannel = animation->mChannels[i];
	
			Channel channel;
			channel.name = aiChannel->mNodeName.C_Str();

			for (UINT j = 0; j < aiChannel->mNumPositionKeys; ++j)
			{
				aiVectorKey key = aiChannel->mPositionKeys[j];
				Vector3 value = AssimpToDX(key.mValue);
				float keyFrame = (float)key.mTime;
				
				channel.positions[keyFrame] = value;
			}

			for (UINT j = 0; j < aiChannel->mNumRotationKeys; ++j)
			{
				aiQuatKey key = aiChannel->mRotationKeys[j];
				Quaternion value = AssimpToDX(key.mValue);
				float keyFrame = (float)key.mTime;

				channel.quaternions[keyFrame] = value;
			}

			for (UINT j = 0; j < aiChannel->mNumScalingKeys; ++j)
			{
				aiVectorKey key = aiChannel->mScalingKeys[j];
				Vector3 value = AssimpToDX(key.mValue);
				float keyFrame = (float)key.mTime;

				channel.scalings[keyFrame] = value;
			}
		}
	}
};