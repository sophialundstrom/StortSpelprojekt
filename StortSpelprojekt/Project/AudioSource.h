#pragma once
#include "player.h"
#include "Collision.h"

class AudioSource
{
private:
	Vector3 position;
	float reach;
	std::string audioName;
	bool active = false;
public:
	AudioSource(const Vector3& position, const float& reach, const std::string& audioName)
	{
		this->position = position;
		this->reach = reach;
		this->audioName = audioName;
	}

	bool CheckActive(const Vector3& position)
	{
		//PrintNumber((position - this->position).Length(), "LEN: ");
		if ((position - this->position).Length() <= reach && !active)
		{
			PrintS("INSIDE");
			Audio::StartEffect(this->audioName);
			Audio::SetVolume(this->audioName, 1.f);
			active = true;
			return true;
		}
		else if ((position - this->position).Length() > reach && active)
		{
			PrintS("OUTSIDE");
			Audio::StopEffect(this->audioName);
			active = false;
		}
		
		return false;
	}

	const Vector3& GetPosition() { return this->position; }
	void SetPosition(const Vector3& position) { this->position = position; }

	const float& GetReach() { return this->reach; }
	void SetReach(const float& reach) { this->reach = reach; }

	const std::string& GetAudioSlot() { return this->audioName; }
	void SetAudioSlot(const int& audioSlot) { this->audioName = audioSlot; }
};