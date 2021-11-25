#pragma once
#include "Drawable.h"
#include "Time.h"

#define MAX_LIGHTS 10

class DirectionalLight
{
private:
	int dir = 1;
	float range = 0;
	float currentAngle = 0.5;
	Vector3 position;
	Matrix viewMatrix;
	Matrix ortoMatrix;
	Matrix matrix;
public:
	struct Data
	{
		Vector4 color = { 1,1,1,1 };
		Vector3 direction;
		float padding = 0;
	} data;
public:
	DirectionalLight() = default;
	DirectionalLight(float range, Vector4 color, float startAngle = 0.5f, int startDir = 1)
		:range(range), dir(startDir), currentAngle(startAngle)
	{
		data.direction = { 1.0f, -1.0f, 0.0f };
		data.direction.Normalize();
		data.color = color;

		this->ortoMatrix = Matrix::CreateOrthographicOffCenter(-range, range, -range, range, 0.1f, range * 2);
		this->viewMatrix = Matrix::CreateLookAt(position + -data.direction * range, position, { 0.0f, 1.0f, 0.0f });
		this->matrix = (viewMatrix * ortoMatrix).Transpose();
	}

	void SetTargetPosition(const Vector3& position)
	{
		this->position = position + -data.direction * range;
		this->viewMatrix = Matrix::CreateLookAt(this->position, position, { 0.0f, 1.0f, 0.0f });
		this->matrix = (viewMatrix * ortoMatrix).Transpose();
	}

	void Update()
	{
		currentAngle += Time::GetDelta() * 0.35f * dir;
		if (currentAngle > PI - 0.5 || currentAngle < 0 + 0.5)
			dir *= -1;

		float x = cos(currentAngle);
		float y = sin(currentAngle);

		Vector3 direction = Vector3(x, y, 0);
		direction.Normalize();

		Vector3 target = { 0,0,0 };

		Vector3 up = { 0,1,0 };
		if (currentAngle > PI_DIV2)
			up = { 0, -1, 0 };

		position = direction * range;

		data.direction = -direction;

		this->viewMatrix = Matrix::CreateLookAt(position, target, up);

		this->matrix = (viewMatrix * ortoMatrix).Transpose();
	}
	float GetRange() { return this->range; }
	Vector3 GetRepresentativePosition() const { return this->position; }
	Matrix GetMatrix() const { return this->matrix; }
};

struct Light : public Drawable
{
	Light(Vector3 position = { 0.0f, 0.0f, 0.0f }, Vector3 rotation = { 0.0f, 0.0f, 0.0f }, Vector3 scale = { 0.0f, 0.0f, 0.0f })
		:Drawable(position, rotation, scale) {}
};

struct PointLight
{
	struct Data
	{
		Vector4 color;
		Vector3 position;
		float range;
		Vector3 attenuation;
	} data;

	PointLight(float range = 10.0f, Vector3 attenuation = { 1.0f, 1.0f, 1.0f }, Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f }, Vector3 position = { 0.0f, 0.0f, 0.0f }, Vector3 rotation = { 0.0f, 0.0f, 0.0f }, Vector3 scale = { 0.0f, 0.0f, 0.0f })
		{ data = { color, position, range, attenuation }; }
};

struct SpotLight : public Light
{
	struct Data
	{
		Vector4 color;
		float range;
		Vector3 attenuation;
		Vector3 position;
		float angle;
		Vector3 direction;
	} data;

	SpotLight(float range = 10.0f, float angle = PI_DIV4, Vector3 direction = { 0.0f, 0.0f, 1.0f }, Vector3 attenuation = { 1.0f, 1.0f, 1.0f }, Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f }, Vector3 position = { 0.0f, 0.0f, 0.0f }, Vector3 rotation = { 0.0f, 0.0f, 0.0f }, Vector3 scale = { 1.0f, 1.0f, 1.0f })
		:Light(position, rotation, scale) { data = { color, range, attenuation, position, angle, direction }; }

	// Inherited via Drawable
	virtual void Update() override
	{
		UpdateMatrix();
		data.position = position;
	}
};