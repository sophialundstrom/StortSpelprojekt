#pragma once
#include "Renderer.h"
#include "Model.h"
#include "Settings.h"

class DisplacementRenderer : public Renderer
{
private:
	//BUFFERS
	ID3D11Buffer* matrices_buf = nullptr;
	struct Matrices
	{
		Matrix world;
		Matrix viewPerspective;
		Matrix lightViewPerspective;
	} matrices;

	ID3D11Buffer* tesselation_buf = nullptr;
	ID3D11Buffer* displacement_buf = nullptr;
	ID3D11Buffer* blending_buf = nullptr;

	//SHADERS
	const std::string vs_path = "../x64/Debug/DisplacementVertexShader.cso";
	ID3D11VertexShader* vertexShader;

	const std::string hs_path = "../x64/Debug/DisplacementHullShader.cso";
	ID3D11HullShader* hullShader;

	const std::string ds_path = "../x64/Debug/DisplacementDomainShader.cso";
	ID3D11DomainShader* domainShader;

	const std::string gs_path = "../x64/Debug/DisplacementGeometryShader.cso";
	ID3D11GeometryShader* geometryShader;

	const std::string ps_path = "../x64/Debug/DisplacementPixelShader.cso";
	ID3D11PixelShader* pixelShader;
public:
	DisplacementRenderer()
	{
		//BUFFERS
		CreateBuffer(matrices_buf, sizeof(Matrices));
		CreateBuffer(tesselation_buf);
		CreateBuffer(displacement_buf);
		CreateBuffer(blending_buf);

		//SHADERS
		LoadVertexShader(vertexShader, vs_path);
		LoadHullShader(hullShader, hs_path);
		LoadDomainShader(domainShader, ds_path);
		LoadGeometryShader(geometryShader, gs_path);
		LoadPixelShader(pixelShader, ps_path);
	}

	~DisplacementRenderer()
	{
		//BUFFERS
		matrices_buf->Release();
		tesselation_buf->Release();
		displacement_buf->Release();
		blending_buf->Release();

		//SHADERS
		vertexShader->Release();
		hullShader->Release();
		domainShader->Release();
		geometryShader->Release();
		pixelShader->Release();
	}

	void Render()
	{
		//SHADERS
		BindShaders(vertexShader, hullShader, domainShader, geometryShader, pixelShader, Settings::UseBackfaceCulling());

		//INPUT LAYOUT
		Graphics::GetContext().IASetInputLayout(ShaderData::inputLayout);

		//TOPOLOGY
		Graphics::GetContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

		//CAMERA & LIGHT MATRIX
		matrices.viewPerspective = ShaderData::cameraMatrix;
		matrices.lightViewPerspective = ShaderData::lightMatrix;

		//BUFFERS
		UpdateBuffer(tesselation_buf, Settings::TesselationFactor());
		BindBuffer(tesselation_buf, Shader::HS);

		UpdateBuffer(displacement_buf, Settings::DisplacementFactor());
		BindBuffer(displacement_buf, Shader::DS);

		UpdateBuffer(blending_buf, Settings::BlendingFactor());
		BindBuffer(blending_buf, Shader::PS, 1);

		//RENDER BOUND DRAWABLES
		for (auto& drawable : drawables)
		{
			auto model = std::dynamic_pointer_cast<Model>(drawable);
			if (!model)
				continue;

			matrices.world = model->GetWorldMatrix().Transpose();
			UpdateBuffer(matrices_buf, matrices);
			BindBuffer(matrices_buf, Shader::DS, 1);

			for (UINT i = 0; i < model->MeshCount(); ++i)
			{
				model->BindMeshTextures(i);
				model->BindMeshDisplacementTexture(i);
				model->BindMeshMaterial(i);
				model->BindMeshBuffer(i);
				model->DrawMesh(i);
			}
		}
	}
};
