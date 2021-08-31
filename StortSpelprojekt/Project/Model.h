#pragma once
#include "Drawable.h"
#include "Mesh.h"
#include "Resources.h"

#include <memory>

class Model : public Drawable, public std::enable_shared_from_this<Model>
{
protected:
	std::string name;

	Matrix worldMatrix;

	bool UVAnim = false;
	UINT meshCount = 0;
	std::vector<Mesh*> meshes;

	Bounds bounds;
public:
	Model(std::string file, Vector3 position = { 0.0f, 0.0f, 0.0f }, Vector3 rotation = { 0.0f, 0.0f, 0.0f }, Vector3 scale = { 1.0f, 1.0f, 1.0f });
	Model(const Model& model);
	virtual ~Model() { for (auto& mesh : meshes) delete mesh; }

	virtual void Update();

	//MATRIX
	Matrix GetWorldMatrix() const { return this->worldMatrix; }

	//UV ANIMATION
	bool HasUVAnimation() const { return this->UVAnim; }
	void HasUVAnimation(bool value) { this->UVAnim = value; }

	//TEXTURES
	bool HasDisplacement() const;
	void AddTexture(std::string file);
	void AddDisplacementTexture(std::string file);

	//RENDERING
	void BindToRenderGraph();

	UINT MeshCount() const { return this->meshCount; }
	void BindMeshBuffer(UINT index = 0);
	void BindMeshTextures(UINT index = 0, UINT startSlot = 0, Shader shader = Shader::PS);
	void BindMeshDisplacementTexture(UINT index = 0, UINT slot = 0, Shader shader = Shader::DS);
	void BindMeshMaterial(UINT index = 0, UINT slot = 0);
	void DrawMesh(UINT index = 0);
};