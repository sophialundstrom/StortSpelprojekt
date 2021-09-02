#pragma once
#include "ImGuiRenderer.h"
#include "Editor.h"

class EditorRenderer : public ImGuiRenderer
{
private:
	std::shared_ptr<Editor> editor;
public:
	EditorRenderer() {}

	void Bind(std::shared_ptr<Editor> editor) { this->editor = editor; }

	void Render()
	{
		Graphics::BeginFrame();
		ImGUI::BeginFrame();

		editor->Render();

		ImGUI::EndFrame();
		Graphics::EndFrame();
	}
};