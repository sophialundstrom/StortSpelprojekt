#pragma once
#include "ImGuiWin.h"
#include "DockSpacePanel.h"
#include "ViewportPanel.h"
#include "Camera.h"

class Editor
{
private:
	void UpdateViewport(float width, float height);
protected:
	ID3D11RenderTargetView* rtv = nullptr;
	ID3D11DepthStencilView* dsv = nullptr;
	ID3D11ShaderResourceView* srv = nullptr;
	D3D11_VIEWPORT viewport = {};

	Camera* camera;

	std::map<std::string, ImGuiWin> windows;

	//DOCKING
	DockSpacePanel dockPanel;

	//VIEWPORT PANEL
	ViewportPanel viewportPanel;

	void AddWindow(std::string name)
	{ windows.emplace(name, ImGuiWin(name)); }

	void BeginFrame()
	{
		Graphics::Inst().BeginFrame();
		ImGUI::BeginFrame();
		dockPanel.Begin();
		viewportPanel.Render();
	}

	void EndFrame()
	{
		for (auto& [name, window] : windows)
			window.Render();

		dockPanel.End();
		ImGUI::EndFrame();
		Graphics::Inst().EndFrame();
	}

	void ClearWindows() { windows.clear(); }

	virtual void Save(const std::string& file) = 0;
	virtual void Load(const std::string& file) = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
public:
	void BeginViewportFrame()
	{
		Graphics::Inst().BeginFrame(rtv, dsv, viewport);
	}

	Editor() 
	{
		viewportPanel.SetResizeCallback([this](float width, float height) { UpdateViewport(width, height); });
		UpdateViewport(5, 5);
	}

	void InitCamera(Camera* camera) { this->camera = camera; }

	virtual ~Editor();
};