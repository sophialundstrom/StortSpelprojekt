#pragma once
#include "Math.h"
#include "Time.h"
#include "Collision.h"

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

	float FOV;
	float nearZ, farZ;
	float pitch;
	float yaw;

	std::shared_ptr<RayCollider> camRay;
public:
	Camera();
	Camera(float FOV, float aspectRatio, float nearZ, float farZ, float rotationSpeed, float moveSpeed,
		Vector3 position = { 0.0f, 0.0f, 0.0f }, Vector3 forward = { 0.0f, 0.0f, 1.0f }, Vector3 up = { 0.0f, 1.0f, 0.0f });

	void MoveUp(int sign = 1);
	void MoveRight(int sign = 1);
	void MoveForward(int sign = 1);
	void Rotate(float dx, float dy);
	void SetSpeedMultiplier(float xSpeed);

	void SetPerspectiveMatrix(float width, float height);

	void UpdatePosOnly();

	void Update();

	void updatecamRay(Vector3 playerPosition, float camMaxLength)
	{
		camRay->SetPosition(playerPosition);
		camRay->origin = playerPosition;
		camRay->direction = (direction * -1);
		camRay->length = camMaxLength;
		camRay->Update();
	}

	std::shared_ptr<RayCollider> GetCamRay()
	{
		return camRay;
	}

	void MoveTowards(Vector3 position)
	{
		const Vector3 direction = position - this->position;
		const Vector3 distance = moveSpeed * direction * Time::GetDelta();
		if ((this->position - position).Length() < distance.Length())
			this->position = position;
		else
			this->position += moveSpeed * direction *  Time::GetDelta();
	}

	void SetPosition(Vector3 newPosition) { position = newPosition; };
	Vector3 GetDirection() const { return this->direction; }
	Vector3 GetPosition() const { return this->position; }
	Matrix GetMatrix() const { return (viewMatrix * perspectiveMatrix).Transpose(); }
	const Matrix& GetProjectionMatrix() const { return perspectiveMatrix; }
	const Matrix& GetViewMatrix() const { return viewMatrix; }
};