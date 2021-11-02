#include "DockSpacePanel.h"

void DockSpacePanel::Begin() const
{
	auto* viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGuiWindowFlags hostWindowFlags = 0;
	hostWindowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_MenuBar;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
	ImGui::Begin("DockSpaceViewport_%08X", nullptr, hostWindowFlags);
	ImGui::PopStyleVar(3);

	const auto dockspaceId = ImGui::GetID("DockSpace");
	ImGui::DockSpace(dockspaceId, {0.0f, 0.0f}, ImGuiDockNodeFlags_None);
}

void DockSpacePanel::End()
{
	ImGui::End();
}
