#pragma once
#include "ImGui.h"
#include "Math.h"

#include <map>

struct ImGuiWinComponent
{
	std::string name = "";

	ImGuiWinComponent(const std::string& name)
	 :name(name) {}

	virtual void Update() = 0;
};

struct TextComponent : public ImGuiWinComponent
{
	std::string value = "";
	TextComponent(const std::string& name)
		:ImGuiWinComponent(name), value(name) {}

	// Inherited via ImGuiWinComponent
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
	virtual void Update() override
	{
		ImGui::SliderFloat(name.c_str(), &value, minValue, maxValue);
	}
};

struct FloatComponent : public ImGuiWinComponent
{
	float value;
	FloatComponent(const std::string& name, float value = 0.0f)
		:ImGuiWinComponent(name), value(value) {}

	// Inherited via ImGuiWinComponent
	virtual void Update() override
	{
		ImGui::InputFloat(name.c_str(), &value);
	}
};

struct CheckBoxComponent : public ImGuiWinComponent
{
	bool value;
	CheckBoxComponent(const std::string& name, bool value = true)
		:ImGuiWinComponent(name), value(value) {}

	// Inherited via ImGuiWinComponent
	virtual void Update() override
	{
		ImGui::Checkbox(name.c_str(), &value);
	}
};

struct Vector3Component : public ImGuiWinComponent
{
	float value[3];
	Vector3Component(const std::string& name, const Vector3& value = { 0.0f, 0.0f, 0.0f })
		:ImGuiWinComponent(name) { this->value[0] = value.x; this->value[1] = value.y; this->value[2] = value.z; }

	// Inherited via ImGuiWinComponent
	virtual void Update() override
	{
		ImGui::InputFloat3(name.c_str(), value);
	}
};

struct SliderVector3Component : public ImGuiWinComponent
{
	float value[3];
	float minValue, maxValue;
	SliderVector3Component(const std::string& name, const Vector3& value = { 0.0f, 0.0f, 0.0f }, float min = -500.0f, float max = 500.0f)
		:ImGuiWinComponent(name), minValue(min), maxValue(max) { this->value[0] = value.x; this->value[1] = value.y; this->value[2] = value.z; }

	// Inherited via ImGuiWinComponent
	virtual void Update() override
	{
		ImGui::SliderFloat3(name.c_str(), value, minValue, maxValue);
	}
};

struct IntComponent : public ImGuiWinComponent
{
	int value;
	IntComponent(const std::string& name, int value = 0)
		:ImGuiWinComponent(name), value(value) {}

	// Inherited via ImGuiWinComponent
	virtual void Update() override
	{
		ImGui::InputInt(name.c_str(), &value);
	}
};

struct SliderIntComponent : public ImGuiWinComponent
{
	int value, minValue, maxValue;
	SliderIntComponent(const std::string& name, int min = 0, int max = 1, int value = 0)
		:ImGuiWinComponent(name), value(value), minValue(min), maxValue(max) {}

	// Inherited via ImGuiWinComponent
	virtual void Update() override
	{
		ImGui::SliderInt(name.c_str(), &value, minValue, maxValue);
	}
};

struct ButtonComponent : public ImGuiWinComponent
{
	bool value = false;
	int width, height;
	ButtonComponent(const std::string& name, int width, int height)
		:ImGuiWinComponent(name), width(width), height(height) {}

	// Inherited via ImGuiWinComponent
	virtual void Update() override
	{
		value = ImGui::Button(name.c_str(), ImVec2((float)width, (float)height));
	}
};

class ImGuiWin
{
protected:
	std::string name;

	std::vector<ImGuiWinComponent*> components;
	std::vector<std::string> names;
public:
	ImGuiWin() = default;
	ImGuiWin(const std::string& name) :name(name) {}

	~ImGuiWin()
	{
		for (auto* component : components)
			delete component;
	}

	void Render()
	{
		ImGui::Begin(name.c_str());
		
		for (auto* component : components)
			component->Update();

		ImGui::End();
	}

	void AddTextComponent(const std::string& name)
	{ components.emplace_back(new TextComponent(name)); names.emplace_back(name); }

	void AddSliderFloatComponent(const std::string& name, float min = 0.0f, float max = 1.0f, float value = 0.0f)
	{ components.emplace_back(new SliderFloatComponent(name, min, max, value)); names.emplace_back(name); }

	void AddFloatComponent(const std::string& name, float value = 0.0f)
	{ components.emplace_back(new FloatComponent(name, value)); names.emplace_back(name); }

	void AddCheckBoxComponent(const std::string& name, bool value = true)
	{ components.emplace_back(new CheckBoxComponent(name, value)); names.emplace_back(name); }

	void AddIntComponent(const std::string& name, int value = 0)
	{ components.emplace_back(new IntComponent(name, value)); names.emplace_back(name); }

	void AddSliderIntComponent(const std::string& name, int min = 0, int max = 1, int value = 0)
	{ components.emplace_back(new SliderIntComponent(name, min, max, value)); names.emplace_back(name); }

	void AddVector3Component(const std::string& name, const Vector3& value = { 0.0f, 0.0f, 0.0f })
	{ components.emplace_back(new Vector3Component(name, value)); names.emplace_back(name); }

	void AddSliderVector3Component(const std::string& name, const Vector3& value = { 0.0f, 0.0f, 0.0f }, float min = -500.0f, float max = 500.0f)
	{ components.emplace_back(new SliderVector3Component(name, value, min, max)); names.emplace_back(name); }

	void AddButtonComponent(const std::string& name, int width, int height)
	{ components.emplace_back(new ButtonComponent(name, width, height)); names.emplace_back(name); }

	template <typename ComponentType>
	auto GetValue(const std::string& name)
	{
		auto it = std::find(names.begin(), names.end(), name);
		int index = (int)std::distance(names.begin(), it);

		ImGuiWinComponent* component = components.at(index);

		return dynamic_cast<ComponentType*>(component)->value;
	}

	template <typename ComponentType, typename ValueType>
	void SetValue(const std::string& name, const ValueType& value)
	{
		auto it = std::find(names.begin(), names.end(), name);
		int index = (int)std::distance(names.begin(), it);

		ImGuiWinComponent* component = components.at(index);

		dynamic_cast<ComponentType*>(component)->value = value;
	}
};
