#pragma once
#include "ImGuiWin.h"

class Editor
{
protected:
	bool firstFrame = true;
	bool done = false;
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

	virtual void Initialize(UINT windowWidth, UINT windowHeight) = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Reset() = 0;

	bool IsDone() const	{ return done; }
};