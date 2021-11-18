#pragma once
#include "Drawable.h"
#include <DirectXCollision.h>
#include "DirectXHelp.h"
#include "Math.h"

enum class ColliderType { SPHERE, BOX, FRUSTUM, RAY };

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

	ColliderType Type()const { return type; }

	virtual void Update() = 0;
};

class FrustumCollider : public Collider
{
private:
	DirectX::BoundingFrustum bounds;
public:
	FrustumCollider(float left, float right, float bottom, float top, float pNear, float pFar)
		:Collider(ColliderType::FRUSTUM), bounds({0, 0, 0}, {0, 0, 0, 1}, right, left, top, bottom, pNear, pFar)
	{
		CreateDynamicVertexBuffer(vertexBuffer, sizeof(Vector3), sizeof(Vector3) * DirectX::BoundingFrustum::CORNER_COUNT);
	}

	const DirectX::BoundingFrustum& GetBounds() const { return bounds; }

	// Inherited via Collider
	virtual void Update() override
	{
		Quaternion quat = bounds.Orientation;
		quat.Normalize();
		bounds.Orientation = quat;

		const Matrix preMatrix = matrix;

		bounds.Transform(bounds, matrix.Invert());

		UpdateMatrix();

		bounds.Transform(bounds, matrix);

		if (preMatrix == matrix)
			return;

		Vector3 corners[DirectX::BoundingFrustum::CORNER_COUNT];
		bounds.GetCorners(corners);

		UpdateDynamicVertexBuffer(vertexBuffer, sizeof(Vector3) * DirectX::BoundingFrustum::CORNER_COUNT, corners);
	}
};

struct RayCollider : public Collider
{
	Vector3 origin;
	Vector3 direction;
	float length = 0;

	RayCollider()
		:Collider(ColliderType::RAY) 
	{
		CreateDynamicVertexBuffer(vertexBuffer, sizeof(Vector3), sizeof(Vector3) * 2);
	}

	// Inherited via Collider
	virtual void Update() override
	{
		Vector3 points[2];

		points[0] = origin;
		if (length > 0)
			points[1] = origin + direction * length;
		else
			points[1] = origin + direction * 1000;

		UpdateDynamicVertexBuffer(vertexBuffer, sizeof(Vector3) * 2, points);
	}
};

//========================================================BOX========================================================
class BoundingBox : public Collider
{
private:
	DirectX::BoundingOrientedBox bounds;
public:
	static const UINT NUM_VERTICES = 8;

	BoundingBox(Matrix matrix = Matrix::Identity, Vector3 position = { 0.0f, 0.0f, 0.0f }, Vector3 extents = { 1.0f, 1.0f, 1.0f }, Quaternion orientation = { 0.0f, 0.0f, 0.0f, 1.0f })
		:Collider(ColliderType::BOX, position), bounds(position, extents, orientation) 
	{
		bounds.Transform(bounds, matrix);

		CreateDynamicVertexBuffer(vertexBuffer, sizeof(Vector3), sizeof(Vector3) * NUM_VERTICES);

		Quaternion quat = bounds.Orientation;
		quat.Normalize();
		bounds.Orientation = quat;

		Vector3 corners[NUM_VERTICES];
		bounds.GetCorners(corners);

		UpdateDynamicVertexBuffer(vertexBuffer, sizeof(Vector3) * NUM_VERTICES, corners);
	}

	const DirectX::BoundingOrientedBox& GetBounds() const { return bounds; }

	// Inherited via Collider
	virtual void Update() override
	{
		const Matrix preMatrix = matrix;

		bounds.Transform(bounds, matrix.Invert());

		Quaternion quat = bounds.Orientation;
		quat.Normalize();
		bounds.Orientation = quat;

		UpdateMatrix();

		bounds.Transform(bounds, matrix);

		quat = bounds.Orientation;
		quat.Normalize();
		bounds.Orientation = quat;

		if (preMatrix == matrix)
			return;

		quat = bounds.Orientation;
		quat.Normalize();
		bounds.Orientation = quat;

		Vector3 corners[NUM_VERTICES];
		bounds.GetCorners(corners);

		UpdateDynamicVertexBuffer(vertexBuffer, sizeof(Vector3) * NUM_VERTICES, corners);
	}

	void GetPlanes(Plane* planes)
	{
		Vector3 points[8];
		bounds.GetCorners(points);

		planes[0] = Plane(points[0], points[2], points[3]);
		planes[1] = Plane(points[4], points[0], points[3]);
		planes[2] = Plane(points[7], points[2], points[6]);
		planes[3] = Plane(points[5], points[4], points[7]);
		planes[4] = Plane(points[1], points[5], points[6]);
		planes[5] = Plane(points[0], points[4], points[5]);

		for (UINT i = 0; i < 6; ++i)
			planes[i].Normalize();
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
				vertices[i] = bounds.Center + Vector3{ cos(angle), 0.0f, sin(angle) } * bounds.Radius;

			else if (i < 16)
				vertices[i] = bounds.Center + Vector3{ cos(angle), sin(angle), 0.0f } * bounds.Radius;

			else
				vertices[i] = bounds.Center + Vector3{ 0.0f, sin(angle), cos(angle) } * bounds.Radius;
		}
	}
public:
	static const UINT NUM_VERTICES = 24;

	BoundingSphere(Matrix matrix = Matrix::Identity, Vector3 position = { 0.0f, 0.0f, 0.0f }, float radius = 1)
		:Collider(ColliderType::SPHERE, position), bounds(position, radius) 
	{
		bounds.Transform(bounds, matrix);

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
				return false;
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
				return false;
			return sphere.GetBounds().Intersects(rayOrigin, rayDirection, temp);
		}
	}
	struct RayResults
	{
		bool didHit = false;
		float distance;
	};


	//RAY LENGTH = 0 WILL CHECK FOR INTERSECTION INFINITELY
	inline RayResults Intersection(const BoundingSphere& sphere, const RayCollider& ray)
	{
		RayResults result;

		if (ray.direction == Vector3(0.f, 0.f, 0.f))
			return result;

		if (ray.length == 0)
			result.didHit = sphere.GetBounds().Intersects(ray.origin, ray.direction, result.distance);
		else
		{
			result.didHit = sphere.GetBounds().Intersects(ray.origin, ray.direction, result.distance);
			if (result.distance > ray.length)
				result.didHit = false;
		}

		return result;
	}

	//RAY LENGTH = 0 WILL CHECK FOR INTERSECTION INFINITELY
	inline RayResults Intersection(const BoundingBox& box, const RayCollider& ray)
	{
		RayResults result;

		if (ray.direction == Vector3(0.f, 0.f, 0.f))
			return result;

		if (ray.length == 0)
			result.didHit = box.GetBounds().Intersects(ray.origin, ray.direction, result.distance);
		else
		{
			result.didHit = box.GetBounds().Intersects(ray.origin, ray.direction, result.distance);
			if (result.distance > ray.length)
				result.didHit = false;
		}

		return result;
	}

	inline bool Intersection(const BoundingBox& box, const FrustumCollider& frustum)
	{
		return box.GetBounds().Intersects(frustum.GetBounds());
	}

	inline bool Contains(const BoundingBox& box, const Vector3& point)
	{
		return box.GetBounds().Contains(point);
	}

	inline bool Contains(const BoundingSphere& sphere, const Vector3& point)
	{
		return sphere.GetBounds().Contains(point);
	}

	inline bool Intersection(const BoundingSphere& sphere, const FrustumCollider& frustum)
	{
		return sphere.GetBounds().Intersects(frustum.GetBounds());
	}

	inline bool Intersection(std::shared_ptr<BoundingBox> box, std::shared_ptr<BoundingSphere> sphere)
	{
		return box->GetBounds().Intersects(sphere->GetBounds());
	}

	inline bool Intersection(std::shared_ptr<BoundingSphere> sphere, std::shared_ptr<BoundingBox> box)
	{
		return box->GetBounds().Intersects(sphere->GetBounds());
	}

	inline bool Intersection(std::shared_ptr<BoundingSphere> sphere, std::shared_ptr<BoundingSphere> otherSphere)
	{
		return otherSphere->GetBounds().Intersects(sphere->GetBounds());
	}

	inline bool Intersection(std::shared_ptr<BoundingBox> box, std::shared_ptr<BoundingBox> otherBox)
	{
		return otherBox->GetBounds().Intersects(box->GetBounds());
	}
}