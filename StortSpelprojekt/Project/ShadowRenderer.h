#pragma once
#include "Renderer.h"
#include "Model.h"

class ShadowRenderer : public Renderer
{
private:
	//BUFFERS
	ID3D11Buffer* matrices_buf;
	struct Matrices
	{
		Matrix world;
		Matrix WVP;
	} matrices;

	ID3D11Buffer* useDisplacement_buf;
	ID3D11Buffer* displcement_buf;
	ID3D11Buffer* tesselation_buf;

	//SHADERS
	const std::string vs_path = "../x64/Debug/ShadowVertexShader.cso";
	ID3D11VertexShader* vertexShader;

	const std::string hs_path = "../x64/Debug/ShadowHullShader.cso";
	ID3D11HullShader* hullShader;

	const std::string ds_path = "../x64/Debug/ShadowDomainShader.cso";
	ID3D11DomainShader* domainShader;
public:
	ShadowRenderer()
	{
		//BUFFERS
		CreateBuffer(matrices_buf, sizeof(Matrices));
		CreateBuffer(useDisplacement_buf);
		CreateBuffer(displcement_buf);
		CreateBuffer(tesselation_buf);

		//SHADERS
		LoadVertexShader(vertexShader, vs_path);
		LoadHullShader(hullShader, hs_path);
		LoadDomainShader(domainShader, ds_path);
	}

	~ShadowRenderer()
	{
		//BUFFERS
		matrices_buf->Release();
		useDisplacement_buf->Release();
		displcement_buf->Release();
		tesselation_buf->Release();

		//SHADERS
		vertexShader->Release();
		hullShader->Release();
		domainShader->Release();
	}

	void Render()
	{
		//SET SHADOW MAP AS RENDER TARGET
		ShaderData::shadowMap.BindAsRenderTarget();

		//INPUT LAYOUT
		Graphics::GetContext().IASetInputLayout(ShaderData::inputLayout);

		//RENDER BOUND DRAWABLES
		for (auto& drawable : drawables)
		{
			auto model = std::dynamic_pointer_cast<Model>(drawable);
			if (!model)
				continue;

			//Matrices
			matrices.world = model->GetWorldMatrix().Transpose();
			matrices.WVP = ShaderData::lightMatrix * matrices.world;
			UpdateBuffer(matrices_buf, matrices);
			BindBuffer(matrices_buf, Shader::VS, 1);

			//TO USE DISPLACEMENT OR NOT
			UpdateBuffer(useDisplacement_buf, model->HasDisplacement());
			BindBuffer(useDisplacement_buf);

			if (model->HasDisplacement())
			{
				//TOPOLOGY
				Graphics::GetContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

				//SHADER
				BindShaders(vertexShader, hullShader, domainShader);

				//BUFFERS
				UpdateBuffer(tesselation_buf, Settings::TesselationFactor());
				BindBuffer(tesselation_buf, Shader::HS);

				UpdateBuffer(displcement_buf, Settings::DisplacementFactor());
				BindBuffer(displcement_buf, Shader::DS);

				BindBuffer(matrices_buf, Shader::DS, 1);

				for (UINT i = 0; i < model->MeshCount(); ++i)
				{
					model->BindMeshDisplacementTexture(i);
					model->BindMeshBuffer(i);
					model->DrawMesh(i);
				}
			}

			else
			{
				//TOPOLOGY
				Graphics::GetContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				//SHADER
				BindShaders(vertexShader);

				for (UINT i = 0; i < model->MeshCount(); ++i)
				{
					model->BindMeshBuffer(i);
					model->DrawMesh(i);
				}
			}
		}
	}
};