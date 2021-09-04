#pragma once
#include "ImGui.h"
#include "Math.h"

struct ImGuiWinComponent
{
	std::string name = "";
	bool changed = false;

	ImGuiWinComponent(const std::string& name)
		:name(name) {}

	virtual bool Changed() = 0;
	virtual void Update() = 0;
};

struct TextComponent : public ImGuiWinComponent
{
	std::string value = "";
	TextComponent(const std::string& name)
		:ImGuiWinComponent(name), value(name) {}

	// Inherited via ImGuiWinComponent
	virtual bool Changed() override
	{
		return false;
	}

	virtual void Update() override
	{
		ImGui::Text(value.c_str());
	}
};

struct SliderFloatComponent : public ImGuiWinComponent
{
	float value, minValue, maxValue;
	SliderFloatComponent(const std::string& name, float min = 0.0f, float max = 1.0f, float value = 0.0f)
		:ImGuiWinComponent(name), minValue(min), maxValue(max), value(value) {}

	// Inherited via ImGuiWinComponent
	virtual bool Changed() override
	{
		return changed;
	}

	virtual void Update() override
	{
		changed = ImGui::SliderFloat(name.c_str(), &value, minValue, maxValue);
	}
};

struct FloatComponent : public ImGuiWinComponent
{
	float value;
	FloatComponent(const std::string& name, float value = 0.0f)
		:ImGuiWinComponent(name), value(value) {}

	// Inherited via ImGuiWinComponent
	virtual bool Changed() override
	{
		return changed;
	}

	virtual void Update() override
	{
		changed = ImGui::InputFloat(name.c_str(), &value);
	}
};

struct CheckBoxComponent : public ImGuiWinComponent
{
	bool value;
	CheckBoxComponent(const std::string& name, bool value = true)
		:ImGuiWinComponent(name), value(value) {}

	// Inherited via ImGuiWinComponent
	virtual bool Changed() override
	{
		return changed;
	}

	virtual void Update() override
	{
		changed = ImGui::Checkbox(name.c_str(), &value);
	}
};

struct Vector3Component : public ImGuiWinComponent
{
	float value[3];
	Vector3Component(const std::string& name, const Vector3& value = { 0.0f, 0.0f, 0.0f })
		:ImGuiWinComponent(name) {
		this->value[0] = value.x; this->value[1] = value.y; this->value[2] = value.z;
	}

	// Inherited via ImGuiWinComponent
	virtual bool Changed() override
	{
		return changed;
	}

	virtual void Update() override
	{
		changed = ImGui::InputFloat3(name.c_str(), value);
	}
};

struct SliderVector3Component : public ImGuiWinComponent
{
	float value[3];
	float minValue, maxValue;
	SliderVector3Component(const std::string& name, const Vector3& value = { 0.0f, 0.0f, 0.0f }, float min = -500.0f, float max = 500.0f)
		:ImGuiWinComponent(name), minValue(min), maxValue(max) {
		this->value[0] = value.x; this->value[1] = value.y; this->value[2] = value.z;
	}

	// Inherited via ImGuiWinComponent
	virtual bool Changed() override
	{
		return changed;
	}

	virtual void Update() override
	{
		changed = ImGui::SliderFloat3(name.c_str(), value, minValue, maxValue);
	}
};

struct IntComponent : public ImGuiWinComponent
{
	int value;
	IntComponent(const std::string& name, int value = 0)
		:ImGuiWinComponent(name), value(value) {}

	// Inherited via ImGuiWinComponent
	virtual bool Changed() override
	{
		return changed;
	}

	virtual void Update() override
	{
		changed = ImGui::InputInt(name.c_str(), &value);
	}
};

struct SliderIntComponent : public ImGuiWinComponent
{
	int value, minValue, maxValue;
	SliderIntComponent(const std::string& name, int min = 0, int max = 1, int value = 0)
		:ImGuiWinComponent(name), value(value), minValue(min), maxValue(max) {}

	// Inherited via ImGuiWinComponent
	virtual bool Changed() override
	{
		return changed;
	}

	virtual void Update() override
	{
		changed = ImGui::SliderInt(name.c_str(), &value, minValue, maxValue);
	}
};

struct ButtonComponent : public ImGuiWinComponent
{
	bool value = false;
	int width, height;
	ButtonComponent(const std::string& name, int width, int height)
		:ImGuiWinComponent(name), width(width), height(height) {}

	// Inherited via ImGuiWinComponent
	virtual bool Changed() override
	{
		return false;
	}

	virtual void Update() override
	{
		value = ImGui::Button(name.c_str(), ImVec2((float)width, (float)height));
	}
};

struct RadioButtonComponent : public ImGuiWinComponent
{
	static const UINT MAX_BUTTONS = 10u;
	UINT numButtons, value;
	std::string names[MAX_BUTTONS] = { "" };

	RadioButtonComponent(const std::string& name, UINT activeID, UINT numButtons, const std::string names[])
		:ImGuiWinComponent(name), value(activeID) , numButtons(numButtons)
	{
		for (UINT i = 0; i < numButtons; ++i)
			this->names[i] = names[i];
	}

	// Inherited via ImGuiWinComponent
	virtual bool Changed() override
	{
		return changed;
	}

	virtual void Update() override
	{
		UINT choice = value;

		changed = false;
		for (UINT i = 0; i < numButtons; ++i)
		{
			bool buttonActive = ImGui::RadioButton(names[i].c_str(), choice == i);
			if (buttonActive)
			{
				changed = true;
				value = i;
			}
				
			if (i < numButtons - 1)
				ImGui::SameLine();
		}
	}
};

struct SeperatorComponent : public ImGuiWinComponent
{
	int value;
	SeperatorComponent()
		:ImGuiWinComponent(""), value(0) {}

	// Inherited via ImGuiWinComponent
	virtual bool Changed() override
	{
		return false;
	}

	virtual void Update() override
	{
		ImGui::Separator();
		ImGui::Separator();
	}
};