#pragma once
#include "Model.h"

class Arrow : public Model
{
private:
	float speed;
	float lifeTime;
	float lifeLength = 0;
	bool isShot = false;
	Vector3 direction;
	float xRadius = 0;

public:
	Arrow(const std::string file);
	~Arrow();

	bool Shoot(Vector3 direction, Vector3 startPos, Vector3 rotation);
	void Update();
};