#pragma once
#include "ImGuiWinComponents.h"
#include <map>

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

	void AddRadioButtonComponent(const std::string& name, UINT activeID, UINT numButtons, const std::string names[])
	{ components.emplace_back(new RadioButtonComponent(name, activeID, numButtons, names)); this->names.emplace_back(name); }

	void AddSeperatorComponent()
	{ components.emplace_back(new SeperatorComponent()); this->names.emplace_back(""); }

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

	bool Changed(const std::string& name)
	{
		auto it = std::find(names.begin(), names.end(), name);
		int index = (int)std::distance(names.begin(), it);

		return components.at(index)->Changed();
	}
};
