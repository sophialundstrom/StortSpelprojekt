#pragma once
#include "Arrow.h"

class ArrowHandler
{
private:

	float pullbackFactor = 1.f;

public:

	std::vector<std::shared_ptr<Arrow>> arrows;

	// Call this function when you want to create a new arrow and add it to the vector.
	// This function creates an arrow instance and adds it to the vector of arrows. It is also bound to the model and collider renderer.
	void AddArrow(ModelRenderer& mRenderer, ColliderRenderer& cRenderer, const Vector3& direction, const Vector3& startPos, const Vector3& rotation);

	// Call this function inside the Update-function of the class owning an instance of this class.
	// This function calls the Update function for each of the arrows in the vector. It also checks if the arrow is supposed to be destroyed, and if that's the case, it unbinds the arrow from the renderers and removes it from the vector of arrows.
	void Update(ModelRenderer& mRenderer, ColliderRenderer& cRenderer);

	// Call this function when checking collision between arrows and a collider. The bool determines if the arrow should treat the colliding object as a static (immobile) or dynamic (moving) object.
	// Checks collision with the input collider and each arrow in the vector.
	bool CheckCollision(std::shared_ptr<Arrow> arrow, std::shared_ptr<Collider> collider, bool isDynamic = false);

	// This function removes all arrows from the input renderers.
	void ClearArrows(ModelRenderer& mRenderer, ColliderRenderer& cRenderer);

	// Sets the pullbackFactor variable that scales how much an arrow should be pulled back during collision.
	void SetPullbackFactor(const float& newFactor) { pullbackFactor = newFactor; }

	// Gets the pullbackFactor variable that scales how much an arrow should be pulled back during collision.
	const float& GetPullbackFactor() { return this->pullbackFactor; }
};

