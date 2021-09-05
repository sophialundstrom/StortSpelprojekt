#pragma once
#include "Editor.h"
#include "Scene.h"
#include "ForwardModelRenderer.h"

class LevelEditor : public Editor
{
private:
	Scene scene;

	//ADD RENDERERS
	ForwardModelRenderer modelRenderer;

	// Inherited via Editor
	virtual void Save(const std::string& file) override;
	virtual void Load(const std::string& file) override;
public:
	LevelEditor() = default;

	// Inherited via Editor
	virtual void Initialize(UINT windowWidth, UINT windowHeight);
	virtual void Update() override;
	virtual void Render() override;
	virtual void Reset() override;
};