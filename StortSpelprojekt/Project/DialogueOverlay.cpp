#include "DialogueOverlay.h"

#include "Window.h"
#include "Event.h"
#include "Time.h"

DialogueOverlay::DialogueOverlay()
{
	auto center = D2D_VECTOR_2F{ Window::ClientWidth() / 2.0f, Window::ClientHeight() / 2.0f };
	AddImage({ center.x, Window::ClientHeight() - 200.0f }, "Background", "DialogueBackground.png");

	auto color = UI::Inst().GetBrush(UI::COLOR::YELLOW);

	dialogueName = new Text(L"TEMP NAME", { center.x - 450, Window::ClientHeight() - 310.0f }, UI::TEXTFORMAT::TITLE_SMALL, color, 900, 30);
	dialogueText = new Text(L"TEMP TEXT", { center.x - 450, Window::ClientHeight() - 290.0f}, UI::TEXTFORMAT::TITLE_SMALL, color, 900, 250);

	AddText({ Window::ClientWidth() / 2.0f + 150.0f, Window::ClientHeight() - 100.0f }, "AnyKey", "- Press E To Boost / Exit -", UI::COLOR::YELLOW, UI::TEXTFORMAT::TITLE_SMALL);
}

DialogueOverlay::~DialogueOverlay()
{
	delete dialogueName;
	delete dialogueText;
}

OVERLAYSTATE DialogueOverlay::Update()
{
	returnState = OVERLAYSTATE::NO_CHANGE;

	if (!NPC)
		return OVERLAYSTATE::RETURN;

	if (Event::KeyIsPressed('E'))
		speed = holdSpeed;
	else
		speed = regularSpeed;

	timeSinceChar += Time::GetDelta() * speed;

	if (numCharacters < dialogueText->GetString().length() && timeSinceChar >= timeBetweenChars)
	{
		timeSinceChar = 0;
		numCharacters++;
	}
		
	else
	{
		delay += Time::GetDelta();

		if (delay > doneDelay && Event::KeyIsPressed('E'))
		{
			lastInteraction = Time::Get();

			if (objective)
			{
				NPC->RemoveDialogueOverride();
				objective->Complete();
				objective = nullptr;
			}

			else
			{
				NPC->SetCompletedConversation();

				if (NPC->HasQuestsLeft())
					NPC->ActivateCurrentQuest();
			}

			NPC = nullptr;
			done = true;
			returnState = OVERLAYSTATE::RETURN;
		}
	}

	return returnState;
}

void DialogueOverlay::Render()
{
	UI::Inst().BeginFrame();

	DrawImages();
	DrawTexts();

	dialogueName->Draw();
	dialogueText->Draw(false, numCharacters);

	UI::Inst().EndFrame();
}

void DialogueOverlay::Set(std::shared_ptr<FriendlyNPC> NPC, TalkObjective* objective)
{
	delay = 0;
	numCharacters = 0;
	done = false;
	this->NPC = NPC;
	dialogueName->SetString(NPC->GetName(), true);

	if (objective && !objective->IsCompleted())
	{
		this->objective = objective;
		dialogueText->SetString(objective->GetString(), true);
	}
	
	else
		dialogueText->SetString(NPC->GetCurrentDialogue(), true);
}