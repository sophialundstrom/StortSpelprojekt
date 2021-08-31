#pragma once
#include "Math.h"
#include "Time.h"
#include "Settings.h"

class DirectionalLight
{
private:
	int dir = 1;
	float range;
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
	DirectionalLight(float range, float startAngle = 0.5f, int startDir = 1)
		:range(range), dir(startDir), currentAngle(startAngle)
	{
		this->ortoMatrix = Matrix::CreateOrthographicOffCenter(-range, range, -range, range, 0.1f, range * 2);
	}

	void Update()
	{
		if (!Settings::UpdateDirectionalLight())
			currentAngle = Settings::LightAngle();
		else
		{
			currentAngle += Time::GetDelta() * 0.35f * dir;
			if (currentAngle > PI - 0.5 || currentAngle < 0 + 0.5)
				dir *= -1;

			Settings::SetLightAngle(currentAngle);
		}

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

	Vector3 GetRepresentativePosition() const { return this->position; }

	Matrix GetMatrix() const { return this->matrix; }
};

#define MAX_LIGHTS 8
struct PointLight
{
	Vector4 color;
	Vector3 position;
	float range;
	Vector3 attenuation;

	PointLight(Vector3 position, float range, Vector3 attenuation = { 0.05f, 0.05f, 0.05f }, Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f })
		:position(position), range(range), attenuation(attenuation), color(color) {}
};