#pragma once
#include "Print.h"
#include <d3d11.h>
#include "Math.h"

struct Transform
{
protected:
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	Transform(Vector3 position, Vector3 rotation, Vector3 scale) :position(position), rotation(rotation), scale(scale) {}
public:
	Vector3 GetPosition() { return this->position; }
	Vector3 GetRotation() { return this->rotation; }
	Vector3 GetScale() { return this->scale; }

	void SetPosition(Vector3 position) { this->position = position; }
	void SetPosition(float x, float y, float z) { this->position = Vector3(x, y, z); }

	void SetRotation(Vector3 rotation) { this->rotation = rotation; }
	void SetRotation(float x, float y, float z) { this->rotation = Vector3(x, y, z); }

	void SetScale(Vector3 scale) { this->scale = scale; }
	void SetScale(float x, float y, float z) { this->scale = Vector3(x, y, z); }
	void SetScale(float scale) { this->scale = Vector3(scale, scale, scale); }
};

struct Drawable : public Transform
{
	Drawable(Vector3 position = { 0.0f, 0.0f, 0.0f }, Vector3 rotation = { 0.0f, 0.0f, 0.0f }, Vector3 scale = { 1.0f, 1.0f, 1.0f })
		:Transform(position, rotation, scale) {}

	virtual void Update() = 0;
};