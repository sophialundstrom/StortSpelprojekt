#pragma once
#include "Event.h"
#include "Camera.h"

class Picker
{
private:
	unsigned int windowWidth, windowHeight;
public:
	Picker(unsigned int windowWidth, unsigned int windowHeight)
		:windowWidth(windowWidth), windowHeight(windowHeight)
	{}

	void Pick(const Camera& camera)
	{
		//2D => 3D VIEW
		Vector3 viewSpace;
		viewSpace.x = (((2.0f * Event::MousePosition().x) / windowWidth) - 1.0f) / camera.GetMatrix()._11;
		viewSpace.x = ((((2.0f * Event::MousePosition().y) / windowHeight) - 1.0f) - 1.0f )/ camera.GetMatrix()._12;
		viewSpace.z = 1.0f;

		//3D VIEW => 3D WORLD
		Matrix worldSpace;
		
	}

	void Intersects();
};