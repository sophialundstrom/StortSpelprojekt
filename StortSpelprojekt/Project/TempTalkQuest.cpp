//#include "TempTalkQuest.h"
//
//TalkQuest::TalkQuest(const std::string& name, bool active, bool completed, std::shared_ptr<FriendlyNPC> NPC)
//	:Quest(name, Quest::Type::Talk, active, completed), NPC(NPC) {}
//
//void TalkQuest::Verify()
//{
//	if (NPC->CompletedConversation())
//	{
//		//MAKE BOOL ^ FALSE
//		completed = true;
//	}
//}
//
//void TalkQuest::SetString(std::string& string)
//{
//	string = "Talk to " + NPC->GetName();
//}