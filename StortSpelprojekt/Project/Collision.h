#pragma once
#include "Drawable.h"
#include <DirectXCollision.h>
#include "DirectXHelp.h"
#include "Math.h"

enum class ColliderType { SPHERE, BOX };

class Collider : public Drawable
{
private:
	const UINT stride = sizeof(Vector3);
	const UINT offset = 0;
protected:
	ColliderType type;
	ID3D11Buffer* vertexBuffer = nullptr;
public:
	Collider(ColliderType type, Vector3 position = { 0.0f, 0.0f, 0.0f }, Vector3 rotation = { 0.0f, 0.0f, 0.0f }, Vector3 scale = { 1.0f, 1.0f, 1.0f })
		:Drawable(position, rotation, scale), type(type) {}
	virtual ~Collider() { vertexBuffer->Release(); }

	void Bind() { Graphics::Inst().GetContext().IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset); }

	ColliderType Type() { return type; }

	virtual void Update() = 0;
};

//========================================================BOX========================================================
class BoundingBox : public Collider
{
private:
	DirectX::BoundingOrientedBox bounds;
public:
	static const UINT NUM_VERTICES = 8;

	BoundingBox(Vector3 position = { 0.0f, 0.0f, 0.0f }, Vector3 extents = { 1.0f, 1.0f, 1.0f }, Quaternion orientation = { 0.0f, 0.0f, 0.0f, 1.0f })
		:Collider(ColliderType::BOX, position), bounds(position, extents, orientation) 
	{
		CreateDynamicVertexBuffer(vertexBuffer, sizeof(Vector3), sizeof(Vector3) * NUM_VERTICES);
	}

	const DirectX::BoundingOrientedBox& GetBounds() const { return bounds; }

	// Inherited via Collider
	virtual void Update() override
	{
		const Matrix preMatrix = matrix;

		bounds.Transform(bounds, matrix.Invert());

		UpdateMatrix();

		bounds.Transform(bounds, matrix);

		if (preMatrix == matrix)
			return;

		Vector3 corners[NUM_VERTICES];
		bounds.GetCorners(corners);

		UpdateDynamicVertexBuffer(vertexBuffer, sizeof(Vector3) * NUM_VERTICES, corners);
	}
};

//========================================================SPHERE========================================================
class BoundingSphere : public Collider
{
private:
	DirectX::BoundingSphere bounds;

	void CalculateVerticesFromBounds(const DirectX::BoundingSphere& bounds, Vector3* vertices)
	{
		const float angles[] = { 0, PI_DIV4, PI_DIV2, PI_DIV4 + PI_DIV2, PI, PI + PI_DIV4, PI + PI_DIV2, 2 * PI - PI_DIV4 };

		for (UINT i = 0; i < NUM_VERTICES; ++i)
		{
			const float angle = angles[i % 8];
			
			if (i < 8)
				vertices[i] = position + Vector3{ cos(angle), 0.0f, sin(angle) } * bounds.Radius;

			else if (i < 16)
				vertices[i] = position + Vector3{ cos(angle), sin(angle), 0.0f } * bounds.Radius;

			else
				vertices[i] = position + Vector3{ 0.0f, sin(angle), cos(angle) } * bounds.Radius;
		}
	}
public:
	static const UINT NUM_VERTICES = 24;

	BoundingSphere(Vector3 position = { 0.0f, 0.0f, 0.0f }, float radius = 1)
		:Collider(ColliderType::SPHERE, position), bounds(position, radius) 
	{
		CreateDynamicVertexBuffer(vertexBuffer, sizeof(Vector3), sizeof(Vector3) * NUM_VERTICES);

		Vector3 vertices[NUM_VERTICES];
		CalculateVerticesFromBounds(bounds, vertices);

		UpdateDynamicVertexBuffer(vertexBuffer, sizeof(Vector3) * NUM_VERTICES, vertices);
	}

	const DirectX::BoundingSphere& GetBounds() const { return bounds; }

	// Inherited via Collider
	virtual void Update() override
	{
		const Matrix preMatrix = matrix;

		bounds.Transform(bounds, matrix.Invert());

		UpdateMatrix();

		bounds.Transform(bounds, matrix);

		if (preMatrix == matrix)
			return;

		Vector3 vertices[NUM_VERTICES];
		CalculateVerticesFromBounds(bounds, vertices);

		UpdateDynamicVertexBuffer(vertexBuffer, sizeof(Vector3) * NUM_VERTICES, vertices);
	}
};

namespace Collision
{
	//SPHERE/SPHERE INTERSECTION
	inline bool Intersection(const BoundingSphere& first, const BoundingSphere& second)
	{
		return first.GetBounds().Intersects(second.GetBounds());
	}

	//SPHERE/BOX INTERSECTION
	inline bool Intersection(const BoundingSphere& first, const BoundingBox& second)
	{
		return first.GetBounds().Intersects(second.GetBounds());
	}

	//BOX/BOX INTERSECTION
	inline bool Intersection(const BoundingBox& first, const BoundingBox& second)
	{
		return first.GetBounds().Intersects(second.GetBounds());
	}

	//RAY LENGTH = 0 WILL CHECK FOR INTERSECTION INFINITELY
	inline bool Intersection(const BoundingBox& box, Vector3 rayOrigin, Vector3 rayDirection, float rayLength = 0)
	{
		float temp;

		if (rayLength == 0)
			return box.GetBounds().Intersects(rayOrigin, rayDirection, temp);

		else
		{
			if ((box.GetPosition() - rayOrigin).Length() > rayLength)
				return;
			return box.GetBounds().Intersects(rayOrigin, rayDirection, temp);
		}
	}

	//RAY LENGTH = 0 WILL CHECK FOR INTERSECTION INFINITELY
	inline bool Intersection(const BoundingSphere& sphere, Vector3 rayOrigin, Vector3 rayDirection, float rayLength = 0)
	{
		float temp;

		if (rayLength == 0)
			return sphere.GetBounds().Intersects(rayOrigin, rayDirection, temp);

		else
		{
			if ((sphere.GetPosition() - rayOrigin).Length() > rayLength)
				return;
			return sphere.GetBounds().Intersects(rayOrigin, rayDirection, temp);
		}
	}
}