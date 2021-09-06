#pragma once
#include "Renderer.h"
#include "Model.h"

//AVAILABLE COMBINATIONS
//ModelRenderer<Deferred, true>		<=> Lit deferred renderer with shadows, directionallight & X amount of pointlights
//ModelRenderer<Forward, true>		<=> Lit forward rendering (only one directionallight & one pointlight, material preview reason)
//ModelRenderer<Forward, false>		<=> Unlit forward (only textures)

template <typename T, bool isLit>
class ModelRenderer : public Renderer
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
	const std::string vs_path = "../x64/Debug/ModelVertexShader.cso";
	const std::string deferred_ps_path = "../x64/Debug/DeferredModelPixelShader.cso";
	const std::string forward_ps_path = "../x64/Debug/ForwardModelPixelShader.cso";

	const std::string unlit_vs_path = "../x64/Debug/UnlitModelVertexShader.cso";
	const std::string unlit_forward_ps_path = "../x64/Debug/UnlitForwardModelPixelShader.cso";
#else
	const std::string vs_path = "../x64/Release/ModelVertexShader.cso";
	const std::string deferred_ps_path = "../x64/Release/DeferredModelPixelShader.cso";
	const std::string forward_ps_path = "../x64/Release/ForwardModelPixelShader.cso";

	const std::string unlit_vs_path = "../x64/Release/UnlitModelVertexShader.cso";
	const std::string unlit_forward_ps_path = "../x64/Release/UnlitForwardModelPixelShader.cso";
#endif

	//SHADERS
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;

	//INPUT LAYOUT
	ID3D11InputLayout* inputLayout = nullptr;
public:
	ModelRenderer()
	{
		//BUFFER
		CreateBuffer(matricesBuf, sizeof(Matrices));

		//SHADERS
		std::string byteCode;

		if (isLit)
		{
			LoadShader(vertexShader, vs_path, byteCode);

			if constexpr (std::is_same_v<Forward, T>)
				LoadShader(pixelShader, forward_ps_path);

			else if constexpr (std::is_same_v<Deferred, T>)
				LoadShader(pixelShader, deferred_ps_path);

			else
			{
				Print("TEMPLATE FIRST PARAM INVALID");
				return;
			}
		}

		else
		{
			LoadShader(vertexShader, unlit_vs_path, byteCode);

			if constexpr (std::is_same_v<Forward, T>)
				LoadShader(pixelShader, unlit_forward_ps_path);

			else if constexpr (std::is_same_v<Deferred, T>)
			{
				Print("UNLIT MODEL RENDERER ONLY AVALIABLE IN FORWARD");
				return;
			}

			else
			{
				Print("TEMPLATE FIRST PARAM INVALID");
				return;
			}
		}

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
			Print("FAILED TO CREATE INPUT LAYOUT");
			return;
		}
	}

	~ModelRenderer()
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
			auto model = std::dynamic_pointer_cast<Model>(drawable);
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