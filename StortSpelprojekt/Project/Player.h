#pragma once
#include "Camera.h"

class Player
{
private:
	Camera camera;
public:
	Player() = default;
	Player(float FOV, float aspectRatio, float nearZ, float farZ, float rotationSpeed, float moveSpeed,
		Vector3 position = { 0.0f, 0.0f, 0.0f }, Vector3 forward = { 0.0f, 0.0f, 1.0f }, Vector3 up = { 0.0f, 1.0f, 0.0f })
		:camera(FOV, aspectRatio, nearZ, farZ, rotationSpeed, moveSpeed, position, forward, up) {}

	void Update()
	{
		camera.Update();
	}

	Camera& GetCamera() { return camera; }
};