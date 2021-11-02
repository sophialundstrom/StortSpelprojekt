#pragma once
#include "Graphics.h"
#include "Math.h"
#include "ImGui.h"
#include <optional>
#include <functional>

class ViewportPanel
{
public:
	std::optional<std::function<void(float, float)>> resizeCallback;
	Vector2 topLeft, bottomRight;
	Vector2 viewportSize;
	ID3D11ShaderResourceView* srv = nullptr;
	bool focused = false;
	bool hovered = false;
public:
	void SetResource(ID3D11ShaderResourceView* srv)
	{
		this->srv = srv;
	}

	float GetWidth() { return bottomRight.x - topLeft.x; }
	float GetHeight() { return bottomRight.y - topLeft.y; }

	bool Hovered() { return hovered; }
	bool Focused() { return focused; }

	Vector2 GetMousePosition()
	{
		auto cursor = ImGui::GetMousePos();

		std::cout << "=====================\n";

		std::cout << "CURSOR: " << cursor.x << " " << cursor.y << std::endl;

		std::cout << "TOP LEFT: " << topLeft.x << " " << topLeft.y << std::endl;

		auto temp = Vector2(cursor.x - topLeft.x, cursor.y - topLeft.y);

		std::cout << "WINDOW RELATIVE: " << temp.x << " " << temp.y << std::endl;

		return temp;
	}

	void SetResizeCallback(const std::function<void(float, float)>& callback) { resizeCallback = callback; }

	void Render()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Viewport");

		focused = ImGui::IsWindowFocused();
		hovered = ImGui::IsWindowHovered();

		const auto viewportOffset = ImGui::GetCursorPos(); // Include tab bar
		ImVec2 minBound = ImGui::GetWindowPos();
		minBound.x += viewportOffset.x;
		minBound.y += viewportOffset.y;

		const auto windowSize = ImGui::GetWindowSize();
		const ImVec2 maxBound = {
			minBound.x + windowSize.x - viewportOffset.x, minBound.y + windowSize.y - viewportOffset.y
		};

		topLeft = Vector2{ minBound.x, minBound.y };
		bottomRight = Vector2{ maxBound.x, maxBound.y };

		const auto tempViewportSize = bottomRight - topLeft;

		if (tempViewportSize != viewportSize && resizeCallback.has_value())
		{
			viewportSize = tempViewportSize;
			resizeCallback.value()(viewportSize.x, viewportSize.y);
		}

		if (srv)
			ImGui::Image(srv, { viewportSize.x, viewportSize.y });
	}
};