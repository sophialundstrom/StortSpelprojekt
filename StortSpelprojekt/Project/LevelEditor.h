#pragma once
#include "Editor.h"
#include "Scene.h"
#include "AnimatedModelRenderer.h"
#include "ModelRenderer.h"
#include "GameState.h"

class LevelEditor : public Editor, public GameState
{
private:
	Scene scene;

	//ADD RENDERERS
	AnimatedModelRenderer animatedModelRenderer;
	ModelRenderer modelRenderer;

	// Inherited via Editor
	virtual void Save(const std::string& file) override;
	virtual void Load(const std::string& file) override;
	virtual void Update() override;
	virtual void Render() override;
public:
	LevelEditor(UINT clientWidth, UINT clientHeight);
	~LevelEditor();
	
	// Inherited via GameState
	State Run();
};