#pragma once
#include "ShaderData.h"
#include "Model.h"

class ShadowRenderer : public Renderer
{
private:
	//BUFFER
	ID3D11Buffer* matrices_buf;
	struct Matrices
	{
		Matrix world;
		Matrix WVP;
	} matrices;

	//SHADERS
	const std::string vs_path = "../x64/Debug/ShadowVertexShader.cso";
	ID3D11VertexShader* vertexShader;
public:
	ShadowRenderer()
	{
		//BUFFER
		CreateBuffer(matrices_buf, sizeof(Matrices));

		//SHADER
		LoadShader(vertexShader, vs_path);
	}

	~ShadowRenderer()
	{
		matrices_buf->Release();
		vertexShader->Release();
	}

	void Render()
	{
		if (drawables.empty())
			return;

		//SET SHADOW MAP AS RENDER TARGET
		ShaderData::Inst().shadowMap.BindAsRenderTarget();

		//INPUT LAYOUT
		Graphics::Inst().GetContext().IASetInputLayout(ShaderData::Inst().inputLayout);

		//RENDER BOUND DRAWABLES
		for (auto& drawable : drawables)
		{
			auto model = std::dynamic_pointer_cast<Model>(drawable);
			if (!model)
				continue;

			//Matrices
			matrices.world = model->GetMatrix().Transpose();
			matrices.WVP = ShaderData::Inst().lightMatrix * matrices.world;
			UpdateBuffer(matrices_buf, matrices);
			BindBuffer(matrices_buf, Shader::VS, 1);

			//TOPOLOGY
			Graphics::Inst().GetContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			//SHADER
			BindShaders(vertexShader);

			model->Draw(false);
		}
	}
};