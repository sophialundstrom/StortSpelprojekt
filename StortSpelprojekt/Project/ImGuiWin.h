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

	void AddTextComponent(const std::string& name, bool sameRow = false)
	{ components.emplace_back(new TextComponent(name, sameRow)); names.emplace_back(name); }

	void AddSliderFloatComponent(const std::string& name, float min = 0.0f, float max = 1.0f, float value = 0.0f, bool sameRow = false)
	{ components.emplace_back(new SliderFloatComponent(name, sameRow, min, max, value)); names.emplace_back(name); }

	void AddFloatComponent(const std::string& name, float value = 0.0f, bool sameRow = false)
	{ components.emplace_back(new FloatComponent(name, sameRow, value)); names.emplace_back(name); }

	void AddCheckBoxComponent(const std::string& name, bool value = true, bool sameRow = false)
	{ components.emplace_back(new CheckBoxComponent(name, sameRow, value)); names.emplace_back(name); }

	void AddIntComponent(const std::string& name, int value = 0, bool sameRow = false)
	{ components.emplace_back(new IntComponent(name, sameRow, value)); names.emplace_back(name); }

	void AddSliderIntComponent(const std::string& name, int min = 0, int max = 1, int value = 0, bool sameRow = false)
	{ components.emplace_back(new SliderIntComponent(name, sameRow, min, max, value)); names.emplace_back(name); }

	void AddVector3Component(const std::string& name, const Vector3& value = { 0.0f, 0.0f, 0.0f }, bool sameRow = false)
	{ components.emplace_back(new Vector3Component(name, sameRow, value)); names.emplace_back(name); }

	void AddSliderVector3Component(const std::string& name, const Vector3& value = { 0.0f, 0.0f, 0.0f }, float min = -500.0f, float max = 500.0f, bool sameRow = false)
	{ components.emplace_back(new SliderVector3Component(name, sameRow, value, min, max)); names.emplace_back(name); }

	void AddButtonComponent(const std::string& name, int width, int height, bool sameRow = false)
	{ components.emplace_back(new ButtonComponent(name, width, height, sameRow)); names.emplace_back(name); }

	void AddRadioButtonComponent(const std::string& name, UINT activeID, UINT numButtons, const std::string names[], bool sameRow = false)
	{ components.emplace_back(new RadioButtonComponent(name, activeID, numButtons, names, sameRow)); this->names.emplace_back(name); }

	void AddSeperatorComponent()
	{ components.emplace_back(new SeperatorComponent()); this->names.emplace_back(""); }

	void AddImageComponent(const std::string& name, bool sameLine, ID3D11ShaderResourceView* srv, float width, float height)
	{ components.emplace_back(new ImageComponent(name, sameLine, srv, width, height)); this->names.emplace_back(name); }

	void AddListBoxComponent(const std::string& name, bool sameLine)
	{ components.emplace_back(new ListBoxComponent(name, sameLine));  this->names.emplace_back(name);	}

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

	template <typename ComponentType>
	auto Get(const std::string& name)
	{
		for (UINT i = 0; i < names.size(); i++)
		{
			if (name == names[i])
			{
				return dynamic_cast<ComponentType*>(components[i]);
			}
		}

		return dynamic_cast<ComponentType*>(components[0]);
	}

	bool Changed(const std::string& name)
	{
		auto it = std::find(names.begin(), names.end(), name);
		int index = (int)std::distance(names.begin(), it);

		return components.at(index)->Changed();
	}
};
