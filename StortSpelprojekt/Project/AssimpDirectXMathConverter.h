#pragma once
#include "assimp\scene.h"
#include "Math.h"

inline Matrix AssimpToDX(const aiMatrix4x4& matrix)
{
	Matrix outMatrix = Matrix(matrix.a1, matrix.a2, matrix.a3, matrix.a4, matrix.b1, matrix.b2, matrix.b3, matrix.b4, matrix.c1, matrix.c2, matrix.c3, matrix.c4, matrix.d1, matrix.d2, matrix.d3, matrix.d4);
	return outMatrix.Transpose();
}

inline Vector3 AssimpToDX(const aiVector3D& vector)
{
	Vector3 outVector = Vector3(vector.x, vector.y, vector.z);
	return outVector;
}

inline Quaternion AssimpToDX(const aiQuaternion& quaternion)
{
	Quaternion outQuat = Quaternion(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
	return outQuat;
}