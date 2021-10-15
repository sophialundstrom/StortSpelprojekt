#pragma once
#include "Model.h"
#include "Print.h"
#include "Collision.h"
#include "Player.h"

class NPC : public Model
{
private:

protected:
	BoundingSphere boundingSphere;
public:
	NPC(const std::string& file);
	NPC(const Model& model);


	virtual void Update() override;
	bool Collided(Player &player);
	//void AddModel(std::map<std::string, std::shared_ptr<Drawable>> &drawables, const std::string& file);
	void debugPrint();
};