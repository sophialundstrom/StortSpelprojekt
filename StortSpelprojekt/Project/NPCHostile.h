#pragma once
#include "NPCBase.h"
#include "ModelRenderer.h"
#include "ArrowHandler.h"
#include "State.h"

class NPCState;

class HostileNPC : public NPC
{
private:
	
	//std::shared_ptr<Player> player;


	ArrowHandler arrowHandler;

	ModelRenderer* mRend = nullptr;
	ColliderRenderer* cRend = nullptr;
	void HandleStates();
public:
	HostileNPC(const std::string& file, std::shared_ptr<Player> player, ModelRenderer& mRenderer, ColliderRenderer& cRenderer);
	HostileNPC(const Model& model);

	virtual void Update() override;
	void Update(const std::shared_ptr<Player> player);
	void CheckPlayerCollision(std::shared_ptr<Player> player);
	ArrowHandler& GetArrowHandler() { return this->arrowHandler; }
	std::shared_ptr<Player> GetPlayer() { return this->player; }
	ModelRenderer* GetMRenderer() { return this->mRend; }
	ColliderRenderer* GetCRenderer() {	return this->cRend;	}
private:
	void WeaponSlash();


	// Remove if we don't use pathing for these NPC's later
public:
	void SetPathVar(Pathfinding* path)				{ this->pathing = path; }
	void SetPlayerPtr(std::shared_ptr<Player> p)	{ this->player = p; }
	void SetState(NPCState &newState)				{ currentState = &newState; }
private:
	NPCState* currentState;
	float speed = 9.0f;
	std::vector<Vector3> path;
	Pathfinding* pathing;
	std::shared_ptr<Player> player;                                                                                                                                                                                                                                                                
};