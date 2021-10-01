#pragma once
#include "Transform.h"

class Drawable : public Transform
{
protected:
	bool visible = true;
	UINT ID;
public:
	Drawable(Vector3 position = { 0.0f, 0.0f, 0.0f }, Vector3 rotation = { 0.0f, 0.0f, 0.0f }, Vector3 scale = { 1.0f, 1.0f, 1.0f })
		:Transform(position, rotation, scale) {}
	
	bool IsVisible() const { return visible; }
	virtual void Update() = 0;
	UINT GetID() { return ID; }
	void SetID(UINT ID) { this->ID = ID; }
};