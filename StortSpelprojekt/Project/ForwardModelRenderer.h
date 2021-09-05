#pragma once
#include "Renderer.h"
#include "Model.h"

class ForwardModelRenderer : public Renderer
{
private:
	//SHADERS
	const std::string vs_path = "../x64/Debug/ForwardModelVertexShader.cso";
	ID3D11VertexShader* vertexShader = nullptr;

	const std::string ps_path = "../x64/Debug/ForwardModelPixelShader.cso";
	ID3D11PixelShader* pixelShader = nullptr;
public:
	ForwardModelRenderer()
	{
		LoadShader(vertexShader, vs_path);
		LoadShader(pixelShader, ps_path);
	}

	~ForwardModelRenderer()
	{
		vertexShader->Release();
		pixelShader->Release();
	}

	// Inherited via Renderer
	void Render()
	{
		//INPUT LAYOUT
		Graphics::Inst().GetContext().IASetInputLayout(ShaderData::Inst().inputLayout);

		//TOPOLOGY
		Graphics::Inst().GetContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//SHADERS
		BindShaders(vertexShader, nullptr, nullptr, nullptr, pixelShader);

		//SAVE SHADER DATA INSTANCE
		auto& shaderData = ShaderData::Inst();

		for (auto& drawable : drawables)
		{
			auto model = std::dynamic_pointer_cast<Model>(drawable);
			if (!model)
				continue;

			shaderData.matrices.world = model->GetMatrix();

			UpdateBuffer(shaderData.matrices_buf, shaderData.matrices);

			BindBuffer(ShaderData::Inst().matrices_buf);

			model->Draw();
		}
	}
};