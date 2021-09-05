#pragma once
#include "Editor.h"
#include "Time.h"
#include "Scene.h"
#include "Resources.h"

class DemoEditor : public Editor
{
private:
	
public:
	DemoEditor()
	{
		AddWindow("Window 1");
		windows["Window 1"].AddTextComponent("Text");
		windows["Window 1"].AddSliderFloatComponent("Float Slider", -24.0f, 60.0f, 4.0f);
		windows["Window 1"].AddFloatComponent("Float", 56.0f);
		windows["Window 1"].AddCheckBoxComponent("CheckBox", false);
		windows["Window 1"].AddIntComponent("Int", 7);
		windows["Window 1"].AddSliderIntComponent("Int Slider", -4, 50, 34);
		windows["Window 1"].AddVector3Component("Vector3", Vector3(1.0f, 3.0f, 70.0f));
		windows["Window 1"].AddSliderVector3Component("Vector3 Slider", Vector3(-20.0f, 30.0f, 56.0f), -700, 700);
		windows["Window 1"].AddButtonComponent("Button", 50, 20);

		AddWindow("Window 2");
		windows["Window 2"].AddTextComponent("Eh");
		//osv...
	}

	// Inherited via Editor
	virtual void Render() override
	{
		BeginFrame();
		EndFrame();
	}
	
	virtual void Update() override
	{
		if (windows["Window 1"].GetValue<ButtonComponent>("Button"))
			windows["Window 1"].AddTextComponent("Added a new TextComponent");

		Print(windows["Window 1"].GetValue<IntComponent>("Int"));

		windows["Window 1"].SetValue<FloatComponent, float>("Float", Time::Get());
	}
};