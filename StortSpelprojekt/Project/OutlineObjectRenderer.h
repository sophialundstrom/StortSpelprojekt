#pragma once
#include "ShaderData.h"
#include "Model.h"
#include "Stencil.h"

class OutlineObjectRenderer : public Renderer
{
private:
	//BUFFER
	ID3D11Buffer* matricesBuf = nullptr;
	struct Matrices
	{
		Matrix world;
		Matrix viewPerspective;
		Matrix lightViewPerspective;
	} matrices;

	//SHADER PATHS
#ifdef _DEBUG
	const std::string vs_path = "../x64/Debug/OutlineObjectVertexShader.cso";
	const std::string forward_ps_path = "../x64/Debug/OutlineObjectPixelShader.cso";
#else
	const std::string vs_path = "../x64/Release/OutlineObjectVertexShader.cso";
	const std::string forward_ps_path = "../x64/Release/OutlineObjectPixelShader.cso";
#endif

	//SHADERS
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;

	//INPUT LAYOUT
	ID3D11InputLayout* inputLayout = nullptr;
	Stencil stencil;
public:
	OutlineObjectRenderer()
		:stencil(Stencil::Mode::WRITE)
	{
		//BUFFER
		CreateBuffer(matricesBuf, sizeof(Matrices));

		//SHADERS
		std::string byteCode;


		if (!LoadShader(vertexShader, vs_path, byteCode))
			return;

		if (!LoadShader(pixelShader, forward_ps_path))
			return;

		Print("SUCCEEDED LOADING SHADERS", "OUTLINE OBJECT RENDERER");

		//INPUT LAYOUT
		D3D11_INPUT_ELEMENT_DESC inputDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXTURECOORDS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		HRESULT hr = Graphics::Inst().GetDevice().CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), byteCode.c_str(), byteCode.length(), &inputLayout);
		if FAILED(hr)
		{
			Print("FAILED TO CREATE INPUT LAYOUT", "OUTLINE OBJECT RENDERER");
			return;
		}
		Print("SUCCEEDED TO CREATE INPUT LAYOUT", "OUTLINE OBJECT RENDERER");

		Print("SUCCEEDED TO INITIALIZE OUTLINE OBJECT RENDERER");
		Print("=======================================");
	}

	~OutlineObjectRenderer()
	{
		matricesBuf->Release();
		vertexShader->Release();
		pixelShader->Release();
		inputLayout->Release();
	}

	virtual void Render() override
	{
		if (drawables.empty())
			return;

		stencil.Bind();

		//INPUT LAYOUT
		Graphics::Inst().GetContext().IASetInputLayout(inputLayout);

		//TOPOLOGY
		Graphics::Inst().GetContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//SHADERS
		BindShaders(vertexShader, nullptr, nullptr, nullptr, nullptr);

		//SAVE SHADER DATA INSTANCE
		auto& shaderData = ShaderData::Inst();

		matrices.viewPerspective = shaderData.cameraMatrix;
		matrices.lightViewPerspective = shaderData.lightMatrix;

		for (auto& drawable : drawables)
		{
			auto model = std::dynamic_pointer_cast<Model>(drawable);
			if (!model)
				continue;
			matrices.world = model->GetMatrix();
			UpdateBuffer(matricesBuf, matrices);
			BindBuffer(matricesBuf);

			model->Draw(false, false);
		}
	}
};
