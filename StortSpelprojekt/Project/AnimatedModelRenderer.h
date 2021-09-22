#pragma once
#include "ShaderData.h"
#include "AnimatedModel.h"

class AnimatedModelRenderer : public Renderer
{
private:
	bool isLit;

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
	const std::string vs_path = "../x64/Debug/AnimatedModelVertexShader.cso";
	const std::string deferred_ps_path = "../x64/Debug/DeferredModelPixelShader.cso";
	const std::string forward_ps_path = "../x64/Debug/ForwardModelPixelShader.cso";

	const std::string unlit_vs_path = "../x64/Debug/UnlitAnimatedModelVertexShader.cso";
	const std::string unlit_forward_ps_path = "../x64/Debug/UnlitForwardModelPixelShader.cso";
#else
	const std::string vs_path = "../x64/Release/AnimatedModelVertexShader.cso";
	const std::string deferred_ps_path = "../x64/Release/DeferredModelPixelShader.cso";
	const std::string forward_ps_path = "../x64/Release/ForwardModelPixelShader.cso";

	const std::string unlit_vs_path = "../x64/Release/UnlitAnimatedModelVertexShader.cso";
	const std::string unlit_forward_ps_path = "../x64/Release/UnlitForwardModelPixelShader.cso";
#endif

	//SHADERS
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;

	//INPUT LAYOUT
	ID3D11InputLayout* inputLayout = nullptr;
public:
	AnimatedModelRenderer(RenderMethod method, bool isLit)
		:isLit(isLit)
	{
		//BUFFER
		CreateBuffer(matricesBuf, sizeof(Matrices));

		//SHADERS
		std::string byteCode;

		if (isLit)
		{
			if (!LoadShader(vertexShader, vs_path, byteCode))
				return;

			if (method == FORWARD)
			{
				if (!LoadShader(pixelShader, forward_ps_path))
					return;
			}

			else
			{
				if (!LoadShader(pixelShader, deferred_ps_path))
					return;
			}
		}

		else
		{
			if (!LoadShader(vertexShader, unlit_vs_path, byteCode))
				return;

			if (method == FORWARD)
			{
				if (!LoadShader(pixelShader, unlit_forward_ps_path))
					return;
			}

			else
			{
				Print("UNLIT MODEL RENDERER ONLY AVALIABLE IN FORWARD");
				return;
			}
		}
		Print("SUCCEEDED LOADING SHADERS", "ANIMATED MODEL RENDERER");

		//INPUT LAYOUT
		D3D11_INPUT_ELEMENT_DESC inputDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXTURECOORDS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BONEIDS", 0, DXGI_FORMAT_R32G32B32A32_SINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		HRESULT hr = Graphics::Inst().GetDevice().CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), byteCode.c_str(), byteCode.length(), &inputLayout);
		if FAILED(hr)
		{
			Print("FAILED TO CREATE INPUT LAYOUT", "ANIMATED MODEL RENDERER");
			return;
		}

		Print("SUCCEEDED TO CREATE INPUT LAYOUT", "ANIMATED MODEL RENDERER");

		Print("SUCCEEDED TO INITIALIZE ANIMATED MODEL RENDERER");
	}

	~AnimatedModelRenderer()
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

		//INPUT LAYOUT
		Graphics::Inst().GetContext().IASetInputLayout(inputLayout);

		//TOPOLOGY
		Graphics::Inst().GetContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//SHADERS
		BindShaders(vertexShader, nullptr, nullptr, nullptr, pixelShader);

		//SAVE SHADER DATA INSTANCE
		auto& shaderData = ShaderData::Inst();

		matrices.viewPerspective = shaderData.cameraMatrix;
		matrices.lightViewPerspective = shaderData.lightMatrix;

		for (auto& drawable : drawables)
		{
			auto model = std::dynamic_pointer_cast<AnimatedModel>(drawable);
			if (!model)
				continue;

			matrices.world = model->GetMatrix();

			UpdateBuffer(matricesBuf, matrices);
			BindBuffer(matricesBuf);

			if (isLit)
				model->Draw();
			else
				model->Draw(true, false);
		}
	}
};