#pragma once
#include "player.h"
#include "Collision.h"

class AudioSource
{
private:
	Vector3 position;
	float reach;
	int audioSlot;
	bool active = false;
public:
	AudioSource(const Vector3& position, const float& reach, const int& audioSlot)
	{
		this->position = position;
		this->reach = reach;
		this->audioSlot = audioSlot;
	}

	bool CheckActive(const Vector3& position)
	{
		if ((position - this->position).Length() <= reach && !active)
		{
			active = true;
			PrintS("INSIDE");
			return true;
		}
		else if ((position - this->position).Length() > reach && active)
		{
			PrintS("OUTSIDE");
			active = false;
		}
		
		return false;
	}

	const Vector3& GetPosition() { return this->position; }
	void SetPosition(const Vector3& position) { this->position = position; }

	const float& GetReach() { return this->reach; }
	void SetReach(const float& reach) { this->reach = reach; }

	const int& GetAudioSlot() { return this->audioSlot; }
	void SetAudioSlot(const int& audioSlot) { this->audioSlot = audioSlot; }
};