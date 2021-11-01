#pragma once
#include "Editor.h"
#include "Scene.h"
#include "AnimatedModelRenderer.h"
#include "ModelRenderer.h"
#include "TerrainRenderer.h"
#include "ApplicationState.h"
#include "Building.h"
#include "PRay.h"
#include "Collision.h"
#include "ColliderRenderer.h"
#include "IDRenderer.h"
#include "VolumeRenderer.h"

class LevelEditor : public Editor, public ApplicationState
{
private:
	std::string selectedObject;
	HWND appWindow;
	float wRatioX;
	float wRatioY;
	POINT cursor;
	Vector3 screenSpaceCoordinates;
	Scene scene;
	std::map<std::string, std::shared_ptr<BoundingSphere>> pickBoxes;
	Terrain* terrain;
	std::shared_ptr <Building> building;
	//ADD RENDERERS
	AnimatedModelRenderer animatedModelRenderer;
	TerrainRenderer terrainRenderer;
	ModelRenderer modelRenderer;
	ColliderRenderer colliderRenderer;
	IDRenderer idRenderer;
	VolumeRenderer volumeRenderer;

	UINT wWidth, wHeight;

	void BindDrawables();

	// Inherited via Editor
	virtual void Save(const std::string& file) override;
	virtual void Load(const std::string& file) override;
	virtual void Update() override;
	virtual void Render() override;
	void CreateBoundingBox();
	void CreateBoundingSphere();
	void RemoveItem(const std::string name);
	void ClearToolUI();
	void UpdateToolUI(std::string name);
	void FocusObject();
	void DuplicateObject();
	void GizmoEdit(std::string object);
public:
	LevelEditor(UINT clientWidth, UINT clientHeight, HWND window);

	~LevelEditor();
	
	// Inherited via GameState
	APPSTATE Run();
};




