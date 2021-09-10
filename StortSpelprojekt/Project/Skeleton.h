#pragma once
#include "Math.h"
#include <vector>

#define MAX_JOINTS 10

struct Joint
{
	std::string name;
	Matrix offsetMatrix;
};

struct Skeleton
{
	std::vector<Matrix> transforms;
	std::vector<Joint> joints;

	const Joint& FindJoint(const std::string& name) const
	{
		for (auto& joint : joints)
			if (joint.name == name)
				return joint;

		return Joint();
	}

	void SetHierarchy(const aiScene* scene)
	{
		
	}
};