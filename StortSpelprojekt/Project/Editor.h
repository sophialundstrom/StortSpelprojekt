#pragma once
#include "ImGuiWin.h"

class Editor
{
protected:
	std::map<std::string, ImGuiWin> windows;

	void AddWindow(std::string name)
	{ windows.emplace(name, ImGuiWin(name)); }

	void BeginFrame()
	{
		Graphics::Inst().BeginFrame();
		ImGUI::BeginFrame();
	}

	void EndFrame()
	{
		for (auto& [name, window] : windows)
			window.Render();

		ImGUI::EndFrame();
		Graphics::Inst().EndFrame();
	}

	void ClearWindows() { windows.clear(); }
private:
	virtual void Save(const std::string& file) = 0;
	virtual void Load(const std::string& file) = 0;
public:
	Editor() = default;
	virtual ~Editor() = default;

	virtual void Update() = 0;
	virtual void Render() = 0;
};