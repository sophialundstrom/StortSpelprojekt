#pragma once

#include "Pedestal.h"
#include <string>
#include <memory>

class Player
{
private:
	std::shared_ptr<Diamond> carriedDiamond;
	bool carriesDiamond = false;
	unsigned int numPlacedDiamonds = 0;
public:
	UINT DiamondsPlaced() const { return this->numPlacedDiamonds; }
	void PickUpDiamond(std::shared_ptr<Diamond> diamond) { diamond->PickUp(); carriedDiamond = diamond; carriesDiamond = true; }
	void PlaceDiamond(std::shared_ptr<Pedestal> pedestal) { pedestal->PlaceDiamond(carriedDiamond); numPlacedDiamonds++; carriesDiamond = false; carriedDiamond = nullptr; }
	bool IsCarryingObject() { return carriesDiamond; }
};