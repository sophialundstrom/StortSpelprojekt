#pragma once
#include "Math.h"
#include "Time.h"

class Camera
{
private:
	Matrix perspectiveMatrix;
	Matrix viewMatrix;

	Vector3 position;
	Vector3 direction;
	Vector3 up;
	Vector3 forward;

	float speedMultiplier = 1;
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
	void SetSpeedMultiplier(float xSpeed);

	void Update();


	void MoveTowards(Vector3 position)
	{
		const Vector3 direction = position - this->position;
		this->position += moveSpeed * direction * Time::GetDelta();
	}

	void SetPosition(Vector3 newPosition) { position = newPosition; };
	Vector3 GetDirection() const { return this->direction; }
	Vector3 GetPosition() const { return this->position; }
	Matrix GetMatrix() const { return (viewMatrix * perspectiveMatrix).Transpose(); }
	Matrix GetProjectionMatrix() const { return perspectiveMatrix; }
	Matrix GetViewMatrix() const { return viewMatrix; }
};