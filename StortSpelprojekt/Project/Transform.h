#pragma once
#include "Math.h"

struct Transform
{
	friend class GameLoader;
protected:
	std::string name;
	std::shared_ptr<Transform> parent;
	Matrix matrix;
	Vector3 position;
	Quaternion rotation;
	Vector3 scale;

	Transform(Vector3 position, Vector3 rotation, Vector3 scale) :position(position), rotation(rotation), scale(scale) {}

	void UpdateMatrix()
	{
		Matrix s = Matrix::CreateScale(scale);

		Matrix t = Matrix::CreateTranslation(position);

		Matrix r = Matrix::CreateFromQuaternion(rotation);
		//Matrix r = Matrix::CreateFromYawPitchRoll(rotation.y, rotation.x, rotation.z);

		this->matrix = s * r * t;

		if (parent)
			this->matrix *= parent->matrix;
	}
public:
	Vector3 GetPosition()		const { return this->position; }
	Vector3& GetPositionRef()	{ return this->position; }
	Quaternion GetRotation()	const { return this->rotation; }
	Vector3 GetScale()			const { return this->scale; }

	void SetPosition(Vector3 position) { this->position = position; }
	void SetPosition(float x, float y, float z) { this->position = Vector3(x, y, z); }

	void SetRotation(Quaternion quaternion) { this->rotation = quaternion; }
	void SetRotation(Vector3 rotation) { this->rotation = Quaternion::CreateFromYawPitchRoll(rotation.y, rotation.x, rotation.z); }
	void SetRotation(float x, float y, float z) { this->rotation = Quaternion::CreateFromYawPitchRoll(y, x, z); }

	void SetScale(Vector3 scale) { this->scale = scale; }
	void SetScale(float x, float y, float z) { this->scale = Vector3(x, y, z); }
	void SetScale(float scale) { this->scale = Vector3(scale, scale, scale); }

	Matrix GetMatrix() const { return this->matrix.Transpose(); }
	void SetMatrix(const Matrix& matrix) { this->matrix = matrix; }

	void SetParent(std::shared_ptr<Transform> parent) { this->parent = parent; }
	bool HasParent() const { if (parent) return true; return false; }

	void SetName(const std::string& newName) { this->name = newName; }
	const std::string GetName() { return this->name; }
};
