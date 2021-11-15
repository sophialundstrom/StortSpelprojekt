//#pragma once
//#include "Quest.h"
//
//class TalkQuest : public Quest
//{
//private:
//	std::string NPC;
//public:
//	TalkQuest(QuestType type, UINT ID, const std::string& name, bool active, const std::string& NPC)
//		:Quest(type, ID, name, active), NPC(NPC)
//	{}
//
//	std::string NPCName() { return NPC; }
//
//	// Inherited via Quest
//	virtual void Activate(std::shared_ptr<Player> player) override
//	{
//		active = true;
//	}
//
//	virtual void Update(std::shared_ptr<Player> player) override
//	{
//		//Complete();
//	}
//
//	virtual void UpdateUI(std::string& string) override
//	{
//		string = "Talk to " + NPC + ".";
//	}
//};