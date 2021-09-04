#pragma once
#include "Math.h"

class Camera
{
private:
	Matrix perspectiveMatrix;
	Matrix viewMatrix;

	Vector3 position;
	Vector3 direction;
	Vector3 up;
	Vector3 forward;

	float moveSpeed;
	float rotationSpeed;

	float pitch;
	float yaw;
public:
	Camera();
	Camera(float FOV, float aspectRatio, float nearZ, float farZ, float rotationSpeed, float moveSpeed,
		Vector3 position = { 0.0f, 0.0f, 0.0f }, Vector3 forward = { 0.0f, 0.0f, 1.0f }, Vector3 up = { 0.0f, 1.0f, 0.0f });

	void MoveUp(int sign = 1);
	void MoveRight(int sign = 1);
	void MoveForward(int sign = 1);
	void Rotate(float dx, float dy);

	void Update();

	Vector3 GetDirection() const { return this->direction; }
	Vector3 GetPosition() const { return this->position; }
	Matrix GetMatrix() const { return (viewMatrix * perspectiveMatrix).Transpose(); }
};