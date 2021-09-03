#pragma once
#include "ForwardModelRenderer.h"

class LevelEditor : public Editor
{
private:
	Scene scene;
	ForwardModelRenderer modelRenderer;
public:
	LevelEditor() = default;
	LevelEditor(UINT windowWidth, UINT windowHeight)
	{
		scene.SetCamera(PI_DIV4, float(windowWidth) / float(windowHeight), 0.1f, 100.0f, 1.0f, 2.0f);
		scene.SetDirectionalLight(40);

		scene.AddModel("world");

		scene.AddModel("world");

		modelRenderer.Bind(scene.Get<Drawable>("world"));

		AddWindow("Scene");
		windows["Scene"].AddTextComponent("Scene Name");

		AddWindow("Game Object");
		windows["Game Object"].AddTextComponent("Game Object Name");

		AddWindow("Material");
		windows["Material"].AddTextComponent("Material Name");
	}

	// Inherited via Editor
	virtual void Update() override
	{
		scene.Update();
	}

	virtual void Render() override
	{
		BeginFrame();

		modelRenderer.Render();

		EndFrame();
	}
};