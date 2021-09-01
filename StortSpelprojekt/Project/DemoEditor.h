#pragma once
#include "Editor.h"

class DemoEditor : public Editor
{
private:

public:
	DemoEditor()
		:Editor("DEMO")
	{
		AddTextComponent("Text");
		AddSliderFloatComponent("Float Slider", -24.0f, 60.0f, 4.0f);
		AddFloatComponent("Float", 56.0f);
		AddCheckBoxComponent("CheckBox", false);
		AddIntComponent("Int", 7);
		AddSliderIntComponent("Int Slider", -4, 50, 34);
		AddVector3Component("Vector3", Vector3(1.0f, 3.0f, 70.0f));
		AddSliderVector3Component("Vector3 Slider", Vector3(-20.0f, 30.0f, 56.0f), -700, 700);
		AddButtonComponent("Button", 50, 20);
	}

	// Inherited via Editor
	virtual void Update() override
	{
		if (GetValue<ButtonComponent>("Button"))
			AddTextComponent("Added a new TextComponent");
	}
};