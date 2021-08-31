#pragma once
#include "Renderer.h"
#include "Model.h"
#include "Settings.h"

class ModelRenderer : public Renderer
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

	float UVOffset = 0.0f;
	ID3D11Buffer* UVOffset_buf = nullptr;
	ID3D11Buffer* time_buf = nullptr;

	//SHADERS
	const std::string vs_path = "../x64/Debug/ModelVertexShader.cso";
	ID3D11VertexShader* vertexShader = nullptr;

	const std::string anim_vs_path = "../x64/Debug/UVAnimationVertexShader.cso";
	ID3D11VertexShader* animVertexShader = nullptr;

	const std::string gs_path = "../x64/Debug/ModelGeometryShader.cso";
	ID3D11GeometryShader* geometryShader = nullptr;

	const std::string ps_path = "../x64/Debug/ModelPixelShader.cso";
	ID3D11PixelShader* pixelShader = nullptr;

	const std::string anim_ps_path = "../x64/Debug/UVAnimationPixelShader.cso";
	ID3D11PixelShader* animPixelShader = nullptr;
public:
	ModelRenderer()
		:matrices()
	{
		//BUFFERS
		CreateBuffer(matrices_buf, sizeof(Matrices));
		CreateBuffer(UVOffset_buf);
		CreateBuffer(time_buf);

		//SHADERS
		std::string byteCode;
		LoadVertexShader(vertexShader, vs_path, byteCode);
		LoadVertexShader(animVertexShader, anim_vs_path);
		LoadGeometryShader(geometryShader, gs_path);
		LoadPixelShader(pixelShader, ps_path);
		LoadPixelShader(animPixelShader, anim_ps_path);

		ShaderData::Initialize(byteCode);
	}

	~ModelRenderer()
	{
		//BUFFERS
		matrices_buf->Release();
		UVOffset_buf->Release();
		time_buf->Release();

		//SHADRES
		vertexShader->Release();
		animVertexShader->Release();
		geometryShader->Release();
		pixelShader->Release();
		animPixelShader->Release();
	}

	void Render()
	{
		//INPUT LAYOUT
		Graphics::GetContext().IASetInputLayout(ShaderData::inputLayout);

		//TOPOLOGY
		Graphics::GetContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//CAMERA & LIGHT MATRIX
		matrices.viewPerspective = ShaderData::cameraMatrix;
		matrices.lightViewPerspective = ShaderData::lightMatrix;

		//RENDER BOUND DRAWABLES
		for (auto& drawable : drawables)
		{
			auto model = std::dynamic_pointer_cast<Model>(drawable);
			if (!model)
				continue;

			//SHADERS
			BindShaders(vertexShader, nullptr, nullptr, geometryShader, pixelShader, Settings::UseBackfaceCulling());

			if (model->HasUVAnimation())
			{
				//IF UV ANIM SWAP PIXEL SHADER
				BindShaders(animVertexShader, nullptr, nullptr, geometryShader, animPixelShader, Settings::UseBackfaceCulling());

				UVOffset += Time::GetDelta() * 0.1f;
				if (UVOffset > 1.0f)
					UVOffset = UVOffset - 1.0f;

				UpdateBuffer(time_buf, Time::Get());
				BindBuffer(time_buf, Shader::VS, 1);

				UpdateBuffer(UVOffset_buf, UVOffset);
				BindBuffer(UVOffset_buf, Shader::PS, 1);
			}

			matrices.world = model->GetWorldMatrix().Transpose();
			UpdateBuffer(matrices_buf, matrices);
			BindBuffer(matrices_buf);

			for (UINT i = 0; i < model->MeshCount(); ++i)
			{
				model->BindMeshTextures(i);
				model->BindMeshMaterial(i);
				model->BindMeshBuffer(i);
				model->DrawMesh(i);
			}
		}
	}
};