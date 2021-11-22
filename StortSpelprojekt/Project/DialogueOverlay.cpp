#include "DialogueOverlay.h"

#include "Window.h"
#include "Event.h"
#include "Time.h"

DialogueOverlay::DialogueOverlay()
{
	auto center = D2D_VECTOR_2F{ Window::ClientWidth() / 2.0f, Window::ClientHeight() / 2.0f };
	AddImage({ center.x, Window::ClientHeight() - 149.0f }, "Background", "DialogueBackground.png");

	auto color = UI::Inst().GetBrush(UI::COLOR::YELLOW);

	dialogueName = new Text(L"TEMP NAME", { center.x - 366, Window::ClientHeight() - 230.0f }, UI::TEXTFORMAT::TITLE_SMALL, color, 652, 30);
	dialogueText = new Text(L"TEMP TEXT", { center.x - 366, Window::ClientHeight() - 200.0f}, UI::TEXTFORMAT::DEFAULT, color, 652, 200);
}

DialogueOverlay::~DialogueOverlay()
{
	delete dialogueName;
	delete dialogueText;
}

void DialogueOverlay::Update()
{
	if (!NPC)
		return;

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
				NPC->ActivateCurrentQuest();
			}

			NPC = nullptr;
			done = true;
		}
	}
}

void DialogueOverlay::Render()
{
	UI::Inst().BeginFrame();

	DrawImages();

	dialogueName->Draw();
	dialogueText->Draw(false, numCharacters);

	UI::Inst().EndFrame();
}

void DialogueOverlay::Set(std::shared_ptr<FriendlyNPC> NPC, TalkObjective* objective)
{
	if (Time::Get() - lastInteraction < 1.0f)
		return;

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