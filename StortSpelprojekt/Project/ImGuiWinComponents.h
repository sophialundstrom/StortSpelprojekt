#pragma once
#include "ImGui.h"
#include "Math.h"

struct ImGuiWinComponent
{
	std::string name = "";
	bool changed = false;
	bool sameLine = false;

	ImGuiWinComponent(const std::string& name, bool sameLine)
		:name(name), sameLine(sameLine) {}

	virtual bool Changed() = 0;
	virtual void Update() = 0;
};

struct TextComponent : public ImGuiWinComponent
{
	std::string value = "";
	TextComponent(const std::string& name, bool sameLine)
		:ImGuiWinComponent(name, sameLine), value(name) {}

	// Inherited via ImGuiWinComponent
	virtual bool Changed() override
	{
		return false;
	}

	virtual void Update() override
	{
		if (sameLine)
			ImGui::SameLine();
		ImGui::Text(value.c_str());
	}
};

struct SliderFloatComponent : public ImGuiWinComponent
{
	float value, minValue, maxValue;
	SliderFloatComponent(const std::string& name, bool sameLine = false, float min = 0.0f, float max = 1.0f, float value = 0.0f)
		:ImGuiWinComponent(name, sameLine), minValue(min), maxValue(max), value(value) {}

	// Inherited via ImGuiWinComponent
	virtual bool Changed() override
	{
		return changed;
	}

	virtual void Update() override
	{
		if (sameLine)
			ImGui::SameLine();
		changed = ImGui::SliderFloat(name.c_str(), &value, minValue, maxValue);
	}
};

struct FloatComponent : public ImGuiWinComponent
{
	float value;
	FloatComponent(const std::string& name, bool sameLine, float value = 0.0f)
		:ImGuiWinComponent(name, sameLine), value(value) {}

	// Inherited via ImGuiWinComponent
	virtual bool Changed() override
	{
		return changed;
	}

	virtual void Update() override
	{
		if (sameLine)
			ImGui::SameLine();
		changed = ImGui::InputFloat(name.c_str(), &value);
	}
};

struct CheckBoxComponent : public ImGuiWinComponent
{
	bool value;
	CheckBoxComponent(const std::string& name, bool sameLine = false, bool value = true)
		:ImGuiWinComponent(name, sameLine), value(value) {}

	// Inherited via ImGuiWinComponent
	virtual bool Changed() override
	{
		return changed;
	}

	virtual void Update() override
	{
		if (sameLine)
			ImGui::SameLine();
		changed = ImGui::Checkbox(name.c_str(), &value);
	}
};

struct Vector3Component : public ImGuiWinComponent
{
	float value[3];
	Vector3Component(const std::string& name, bool sameLine = false, const Vector3& value = { 0.0f, 0.0f, 0.0f })
		:ImGuiWinComponent(name, sameLine) {
		this->value[0] = value.x; this->value[1] = value.y; this->value[2] = value.z;
	}

	// Inherited via ImGuiWinComponent
	virtual bool Changed() override
	{
		return changed;
	}

	virtual void Update() override
	{
		if (sameLine)
			ImGui::SameLine();
		changed = ImGui::InputFloat3(name.c_str(), value);
	}
};

struct SliderVector3Component : public ImGuiWinComponent
{
	float value[3];
	float minValue, maxValue;
	SliderVector3Component(const std::string& name, bool sameLine = false, const Vector3& value = { 0.0f, 0.0f, 0.0f }, float min = -500.0f, float max = 500.0f)
		:ImGuiWinComponent(name, sameLine), minValue(min), maxValue(max) {
		this->value[0] = value.x; this->value[1] = value.y; this->value[2] = value.z;
	}

	// Inherited via ImGuiWinComponent
	virtual bool Changed() override
	{
		return changed;
	}

	virtual void Update() override
	{
		if (sameLine)
			ImGui::SameLine();
		changed = ImGui::SliderFloat3(name.c_str(), value, minValue, maxValue);
	}
};

struct IntComponent : public ImGuiWinComponent
{
	int value;
	IntComponent(const std::string& name, bool sameLine = false, int value = 0)
		:ImGuiWinComponent(name, sameLine), value(value) {}

	// Inherited via ImGuiWinComponent
	virtual bool Changed() override
	{
		return changed;
	}

	virtual void Update() override
	{
		if (sameLine)
			ImGui::SameLine();
		changed = ImGui::InputInt(name.c_str(), &value);
	}
};

struct SliderIntComponent : public ImGuiWinComponent
{
	int value, minValue, maxValue;
	SliderIntComponent(const std::string& name, bool sameLine = false, int min = 0, int max = 1, int value = 0)
		:ImGuiWinComponent(name, sameLine), value(value), minValue(min), maxValue(max) {}

	// Inherited via ImGuiWinComponent
	virtual bool Changed() override
	{
		return changed;
	}

	virtual void Update() override
	{
		if (sameLine)
			ImGui::SameLine();
		changed = ImGui::SliderInt(name.c_str(), &value, minValue, maxValue);
	}
};

struct ButtonComponent : public ImGuiWinComponent
{
	bool value = false;
	int width, height;
	ButtonComponent(const std::string& name, int width, int height, bool sameLine = false)
		:ImGuiWinComponent(name, sameLine), width(width), height(height) {}

	// Inherited via ImGuiWinComponent
	virtual bool Changed() override
	{
		return false;
	}

	virtual void Update() override
	{
		if (sameLine)
			ImGui::SameLine();
		value = ImGui::Button(name.c_str(), ImVec2((float)width, (float)height));
	}
};

struct RadioButtonComponent : public ImGuiWinComponent
{
	static const UINT MAX_BUTTONS = 10u;
	UINT numButtons, value;
	std::string names[MAX_BUTTONS] = { "" };

	RadioButtonComponent(const std::string& name, UINT activeID, UINT numButtons, const std::string names[], bool sameLine = false)
		:ImGuiWinComponent(name, sameLine), value(activeID) , numButtons(numButtons)
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
		:ImGuiWinComponent("", false), value(0) {}

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


struct ImageComponent : public ImGuiWinComponent
{
	ID3D11ShaderResourceView* value = nullptr;

	float width, height;
	ImageComponent(const std::string& name, bool sameLine, ID3D11ShaderResourceView* srv, float width, float height)
		:ImGuiWinComponent(name, sameLine), width(width), height(height), value(srv)
	{
	}

	// Inherited via ImGuiWinComponent
	virtual bool Changed() override
	{
		return false;
	}

	virtual void Update() override
	{
		if (!value)
			return;
		if (sameLine)
			ImGui::SameLine();

		ImGui::Image(value, { width, height });
	}
};

struct ListBoxComponent : public ImGuiWinComponent
{
	std::vector<std::string> names;

	std::string value;
	bool changed = false;


	ListBoxComponent(const std::string& name, bool sameLine)
		:ImGuiWinComponent(name, sameLine)
	{
	}

	// Inherited via ImGuiWinComponent
	virtual bool Changed() override
	{
		return changed;
	}

	virtual void Update() override
	{
		ImGui::BeginListBox(value.c_str(), {300, 450});
		changed = false;
		for (auto name : names)
		{
			std::string& item_name = name;
			if (ImGui::Selectable(name.c_str()))
			{
				changed = true;
				value = name;
				ImGui::SetScrollHereY();
			}

		}

		ImGui::EndListBox();
	}

	void AddName(const std::string& name)
	{
		names.emplace_back(name);
	}

	void RemoveName(const std::string& name)
	{
		for (UINT i = 0; i < names.size(); i++)
		{
			if (names[i] == name)
			{
				names.erase(names.begin() + i);
				return;
			}
		}
	}
};