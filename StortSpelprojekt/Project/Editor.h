#include "ImGuiWin.h"

class Editor
{
protected:
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
public:
	Editor() = default;
	virtual void Update() = 0;
	virtual void Render() = 0;
	bool IsDone() { return done; }
};