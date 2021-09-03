#pragma once
#include "Renderer.h"
#include "TempModel.h"

class ForwardModelRenderer : public Renderer
{
private:
	//SHADERS
#ifdef _DEBUG
	const std::string vs_path = "../x64/Debug/ForwardModelVertexShader.cso";
#else
	const std::string vs_path = "../x64/Release/ForwardModelVertexShader.cso";
#endif
	ID3D11VertexShader* vertexShader = nullptr;

#ifdef _DEBUG
	const std::string ps_path = "../x64/Debug/ForwardModelPixelShader.cso";
#else
	const std::string ps_path = "../x64/Release/ForwardModelPixelShader.cso";
#endif
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
	virtual void Render() override
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
			auto model = std::dynamic_pointer_cast<TempModel>(drawable);
			if (!model)
				continue;

			shaderData.matrices.world = model->Matrix();

			UpdateBuffer(shaderData.matrices_buf, shaderData.matrices);

			BindBuffer(ShaderData::Inst().matrices_buf);

			model->Draw();
		}
	}
};