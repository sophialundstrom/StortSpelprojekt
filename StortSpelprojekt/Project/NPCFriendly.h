#pragma once
#include "NPCBase.h"
#include "Model.h"
#include "Building.h"
#include "QuestMarker.h"
#include "Quest.h"

class FriendlyNPC : public NPC
{
public:
	FriendlyNPC(const std::string& name, const std::string& file);

	const std::string AddQuest(const std::string& name);

	void AddDialogue(const std::string& string);

	bool Interactable();

	bool CompletedConversation()					{ return finishedDialogue; }
	void SetCompletedConversation()					{ finishedDialogue = true; }
	void ActivateCurrentQuest();

	std::shared_ptr<QuestMarker> GetQuestMarker()	{ return questMarker; }
	const std::string GetCurrentDialogue();

	virtual void Update() override;
private:
	std::vector<std::string> dialogues;

	enum class DialogueState { HANDOUT, HELP, HANDIN, DONE };
	DialogueState currentDialogueState;
	bool finishedDialogue = false;

	UINT currentQuestID = 0;
	Quest* currentQuest = nullptr;
	std::vector<Quest*> quests;

	std::shared_ptr<QuestMarker> questMarker;
};