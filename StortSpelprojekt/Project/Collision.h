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

class BoundingBox : public Collider
{
private:
	DirectX::BoundingOrientedBox bounds;
public:
	BoundingBox(Vector3 position = { 0.0f, 0.0f, 0.0f }, Vector3 extents = { 1.0f, 1.0f, 1.0f }, Quaternion orientation = { 0.0f, 0.0f, 0.0f, 1.0f })
		:Collider(ColliderType::BOX, position), bounds(position, extents, orientation) 
	{
		CreateDynamicVertexBuffer(vertexBuffer, sizeof(Vector3), sizeof(Vector3) * 8);
	}

	const DirectX::BoundingOrientedBox& GetBounds() const { return bounds; }

	// Inherited via Collider
	virtual void Update() override
	{
		UpdateMatrix();

		DirectX::BoundingOrientedBox tempBounds;

		bounds.Transform(tempBounds, matrix);

		Vector3 corners[8];
		tempBounds.GetCorners(corners);

		UpdateDynamicVertexBuffer(vertexBuffer, sizeof(Vector3) * 8, corners);
	}
};

class BoundingSphere : public Collider
{
private:
	DirectX::BoundingSphere bounds;
public:
	BoundingSphere(Vector3 position = { 0.0f, 0.0f, 0.0f }, float radius = 1)
		:Collider(ColliderType::SPHERE, position), bounds(position, radius) 
	{
	}

	const DirectX::BoundingSphere& GetBounds() const { return bounds; }

	// Inherited via Collider
	virtual void Update() override
	{
		UpdateMatrix();
		bounds.Transform(bounds, matrix);
	}
};