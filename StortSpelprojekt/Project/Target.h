#pragma once
#include "Model.h"
#include "Collision.h"

class Target : public Model
{
private:

public:
	bool GotHit() { return true; }
};