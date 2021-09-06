#pragma once
#include "Editor.h"
#include "Scene.h"
#include "ModelRenderer.h"
#include "GameState.h"

class LevelEditor : public Editor, public GameState
{
private:
	Scene scene;

	//ADD RENDERERS
	ModelRenderer<Forward, false> modelRenderer;

	// Inherited via Editor
	virtual void Save(const std::string& file) override;
	virtual void Load(const std::string& file) override;
	virtual void Update() override;
	virtual void Render() override;
public:
	LevelEditor(UINT windowWidth, UINT windowHeight);
	~LevelEditor();
	
	// Inherited via GameState
	State Run();
};