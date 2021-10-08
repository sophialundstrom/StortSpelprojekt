#pragma once
#include "Transform.h"

class Drawable : public Transform
{
protected:
	bool visible = true;
	int ID = 0;
public:
	Drawable(Vector3 position = { 0.0f, 0.0f, 0.0f }, Vector3 rotation = { 0.0f, 0.0f, 0.0f }, Vector3 scale = { 1.0f, 1.0f, 1.0f })
		:Transform(position, rotation, scale) {}
	
	bool IsVisible() const { return visible; }
	void SetID(int id) { this->ID = id; }
	int GetID() { return ID; }
	virtual void Update() = 0;
};