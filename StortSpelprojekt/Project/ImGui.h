#pragma once

#include "Graphics.h"

#include <ImGui\imgui.h>
#include <ImGui\imgui_impl_dx11.h>
#include <ImGui\imgui_impl_win32.h>

struct ImGUI
{
	static void BeginFrame()
	{
		ImGui::GetIO().DisplaySize = { 1600, 900 };
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	static void EndFrame()
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	static void Initialize()
	{
		ImGui::CreateContext();
		ImGui_ImplWin32_Init(GetActiveWindow());
		ImGui_ImplDX11_Init(&Graphics::GetDevice(), &Graphics::GetContext());
	}

	static void ShutDown()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}
};