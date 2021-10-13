#pragma once
#include "Model.h"

class Arrow : public Model
{
private:
	float speed;
	float lifeTime;
	Vector3 direction;
	bool isShot = false;

public:
	Arrow(const std::string file);
	~Arrow();

	bool Shoot(Vector3 direction, Vector3 startPos, Vector3 rotation);
	void Update();
};