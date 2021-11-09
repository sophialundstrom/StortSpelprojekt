#include "DialogueOverlay.h"

#include "Window.h"
#include "Time.h"

DialogueOverlay::DialogueOverlay()
{
	auto center = D2D_VECTOR_2F{ Window::ClientWidth() / 2.0f, Window::ClientHeight() / 2.0f };
	AddImage({ center.x, Window::ClientHeight() - 149.0f }, "Background", "DialogueBackground.png");

	auto color = UI::Inst().GetBrush(UI::COLOR::YELLOW);

	dialogueName = new Text(L"TEMP NAME", { center.x, Window::ClientHeight() - 400.0f }, UI::Inst().GetTextFormat(UI::TEXTFORMAT::TITLE_SMALL), color, 700, 30);
	dialogueText = new Text(L"TEMP TEXT", { center.x, Window::ClientHeight() - 300.0f}, UI::Inst().GetTextFormat(UI::TEXTFORMAT::DEFAULT), color, 700, 200);
}

DialogueOverlay::~DialogueOverlay()
{
	delete dialogueName;
	delete dialogueText;
}

void DialogueOverlay::Update()
{
	if (Event::KeyIsPressed('E'))
		speed = holdSpeed;
	else
		speed = regularSpeed;

	if (numCharacters < dialogueText->GetString().length())
		numCharacters += Time::GetDelta() * speed;

	else
	{
		static float delay = 0;
		delay += Time::GetDelta();

		if (delay > doneDelay)
			active = false;

		if (!active && Event::KeyIsPressed('E'))
			done = true;
	}
}

void DialogueOverlay::Render()
{
	Canvas::Render();

	dialogueName->Draw();
	dialogueText->Draw(false, numCharacters);
}

void DialogueOverlay::Set(const std::string& NPCName, const std::string& text)
{
	numCharacters = 0;
	active = true;
	done = false;
	dialogueName->SetString(NPCName, true);
	dialogueText->SetString(text, true);
}