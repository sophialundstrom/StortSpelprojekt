#pragma once
#include "Math.h"

struct Transform
{
protected:
	std::shared_ptr<Transform> parent;
	Matrix matrix;
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	Transform(Vector3 position, Vector3 rotation, Vector3 scale) :position(position), rotation(rotation), scale(scale) {}

	void UpdateMatrix()
	{
		Matrix s = Matrix::CreateScale(scale);

		Matrix t = Matrix::CreateTranslation(position);

		Matrix r = Matrix::CreateFromYawPitchRoll(rotation.y, rotation.x, rotation.z);

		this->matrix = s * r * t;

		if (parent)
			matrix *= parent->matrix;
	}
public:
	Vector3 GetPosition()	const { return this->position; }
	Vector3 GetRotation()	const { return this->rotation; }
	Vector3 GetScale()		const { return this->scale; }

	void SetPosition(Vector3 position) { this->position = position; }
	void SetPosition(float x, float y, float z) { this->position = Vector3(x, y, z); }

	void SetRotation(Vector3 rotation) { this->rotation = rotation; }
	void SetRotation(float x, float y, float z) { this->rotation = Vector3(x, y, z); }

	void SetScale(Vector3 scale) { this->scale = scale; }
	void SetScale(float x, float y, float z) { this->scale = Vector3(x, y, z); }
	void SetScale(float scale) { this->scale = Vector3(scale, scale, scale); }

	Matrix GetMatrix() const { return this->matrix; }

	void SetParent(std::shared_ptr<Transform> parent) { this->parent = parent; }
	bool HasParent() const { if (parent) return true; return false; }
};
