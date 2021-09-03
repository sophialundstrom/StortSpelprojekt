#include "Model.h"

#include "RenderGraph.h"

Model::Model(std::string file, Vector3 position, Vector3 rotation, Vector3 scale)
	:Drawable(position, rotation, scale), name(file)
{
	//OBJLoader::LoadModel(file, meshes, bounds);
	meshCount = (UINT)meshes.size();

	Update();
}

Model::Model(const Model& model)
{
	this->name = model.name;
	this->worldMatrix = model.worldMatrix;
	this->UVAnim = model.UVAnim;
	this->meshCount = model.meshCount;
	this->bounds = model.bounds;

	for (auto* mesh : model.meshes)
	{
		auto& newMesh = meshes.emplace_back(new Mesh(*mesh));
		CreateVertexBuffer(newMesh->vertexBuffer, newMesh->stride, newMesh->stride * (UINT)newMesh->faces.size() * 3, newMesh->faces.data());
	}
}

void Model::Update()
{
	Matrix s = Matrix::CreateScale(scale);

	Matrix t = Matrix::CreateTranslation(position);

	Matrix r = Matrix::CreateFromYawPitchRoll(rotation.y, rotation.x, rotation.z);

	this->worldMatrix = s * r * t;
}

void Model::BindToRenderGraph()
{
	if (HasDisplacement())
		RenderGraph::Inst().Bind(shared_from_this(), RendererType::DISPLACEMENT);
	else
		RenderGraph::Inst().Bind(shared_from_this(), RendererType::MODEL);
	RenderGraph::Inst().Bind(shared_from_this(), RendererType::SHADOW);
}

bool Model::HasDisplacement() const
{
	bool hasDisplacement = false;
	for (auto& mesh : meshes)
		if (Resources::MaterialHasDisplacementTexture(mesh->mtllib, mesh->materialID))
			return true;
	return false;
}

void Model::AddTexture(std::string file)
{
	for (auto& mesh : meshes)
		Resources::AddTexture(mesh->mtllib, name, file);
}

void Model::AddDisplacementTexture(std::string file)
{
	for (auto& mesh : meshes)
		Resources::AddDisplacementTexture(mesh->mtllib, name, file);
}

void Model::BindMeshBuffer(UINT index)
{
	if (index > meshCount)
		return;

	Graphics::Inst().GetContext().IASetVertexBuffers(0, 1, &meshes[index]->vertexBuffer, &meshes[index]->stride, &meshes[index]->offset);
}

void Model::BindMeshTextures(UINT index, UINT startSlot, Shader shader)
{
	if (index > meshCount)
		return;

	Resources::BindMaterialTextures(meshes[index]->mtllib, meshes[index]->materialID, startSlot, shader);
}

void Model::BindMeshDisplacementTexture(UINT index, UINT slot, Shader shader)
{
	if (index > meshCount)
		return;

	Resources::BindMaterialDisplacementTexture(meshes[index]->mtllib, meshes[index]->materialID, slot, shader);
}

void Model::BindMeshMaterial(UINT index, UINT slot)
{
	if (index > meshCount)
		return;

	Resources::BindMaterial(meshes[index]->mtllib, meshes[index]->materialID, slot);
}

void Model::DrawMesh(UINT index)
{
	if (index > meshCount)
		return;

	Graphics::Inst().GetContext().Draw((UINT)meshes[index]->faces.size() * 3, 0);
}