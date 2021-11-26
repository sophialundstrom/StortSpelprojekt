#pragma once
#include "Model.h"
#include "Collision.h"

class Target : public Model
{
private:
	bool hit;
	std::shared_ptr<BoundingBox> collider;
	Vector3 markerColor;
public:
	Target(const std::string& file, const Vector3& position, const Vector3& rotation, UINT ID);
	void Hit() { hit = true; }
	bool GotHit() { return hit; }
	void Reset() { hit = false; }

	Vector3 GetMarkerColor() { return markerColor; }
	void SetMarkerColor(const Vector3& color) { this->markerColor = color; }

	std::shared_ptr<BoundingBox> GetCollider() { return collider; }
};