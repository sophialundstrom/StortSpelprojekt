#pragma once
#include "Model.h"
#include "Print.h"
#include "Collision.h"
#include "Player.h"

class NPC : public Model
{
private:
	

protected:

	//Transform t;
	//std::shared_ptr<Model> model;
	BoundingSphere boundingSphere;
public:
	NPC(const std::string& file);

	virtual void Update() override;
	bool Collided(Player &player);
	//void AddModel(std::map<std::string, std::shared_ptr<Drawable>> &drawables, const std::string& file);
	void debugPrint();
};