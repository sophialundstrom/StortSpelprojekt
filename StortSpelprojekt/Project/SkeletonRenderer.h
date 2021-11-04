#pragma once
#include "Renderer.h"
#include "AnimatedModel.h"

class SkeletonRenderer : public Renderer
{
private:
	//BUFFER
	ID3D11Buffer* matricesBuf = nullptr;
	struct Matrices
	{
		Matrix world;
		Matrix viewPerspective;
	} matrices;

	//VERTEX BUFFER
	const UINT stride = sizeof(Vector3);
	const UINT offset = 0;
	ID3D11Buffer* vertexBuffer;

	//SHADER PATHS
	const std::string vs_path = "../x64/Debug/SkeletonVertexShader.cso";
	const std::string ps_path = "../x64/Debug/SkeletonPixelShader.cso";

	//SHADERS
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;

	//INPUT LAYOUT
	ID3D11InputLayout* inputLayout = nullptr;

	//DEPTH STENCIL STATE
	ID3D11DepthStencilState* depthStencilState = nullptr;
public:
	SkeletonRenderer()
	{
		//BUFFERS
		CreateBuffer(matricesBuf, sizeof(Matrices));
		CreateDynamicVertexBuffer(vertexBuffer, stride, stride * MAX_JOINTS);

		//DEPTH STENCIL STATE
		D3D11_DEPTH_STENCIL_DESC dssDesc = {};
		dssDesc.DepthEnable = false;
		Graphics::Inst().GetDevice().CreateDepthStencilState(&dssDesc, &depthStencilState);
		Print("SUCCEEDED CREATING DEPTH STENCIL STATE", "SKELETON RENDERER");

		//SHADERS
		std::string byteCode;
		if (!LoadShader(vertexShader, vs_path, byteCode))
			return;
		if (!LoadShader(pixelShader, ps_path))
			return;
		Print("SUCCEEDED LOADING SHADERS", "SKELETON RENDERER");

		//INPUT LAYOUT
		D3D11_INPUT_ELEMENT_DESC inputDesc = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };

		HRESULT hr = Graphics::Inst().GetDevice().CreateInputLayout(&inputDesc, 1, byteCode.c_str(), byteCode.length(), &inputLayout);
		if FAILED(hr)
		{
			Print("FAILED TO CREATE INPUT LAYOUT", "SKELETON RENDERER");
			return;
		}
		Print("SUCCEEEDED TO CREATE INPUT LAYOUT", "SKELETON RENDERER");

		Print("SUCCEEDED TO INITIALIZE SKELETON RENDERER");
		Print("=======================================");
	}

	~SkeletonRenderer()
	{
		vertexBuffer->Release();
		matricesBuf->Release();
		vertexShader->Release();
		pixelShader->Release();
		inputLayout->Release();
		depthStencilState->Release();
	}

	virtual void Render() override
	{
		//INPUT LAYOUT
		Graphics::Inst().GetContext().IASetInputLayout(inputLayout);

		//TOPOLOGY
		Graphics::Inst().GetContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		//SHADERS
		BindShaders(vertexShader, nullptr, nullptr, nullptr, pixelShader);

		//BUFFER
		matrices.viewPerspective = ShaderData::Inst().cameraMatrix;

		//DEPTH STENCIL STATE
		Graphics::Inst().GetContext().OMSetDepthStencilState(depthStencilState, 0);

		for (auto& drawable : drawables)
		{
			auto model = std::dynamic_pointer_cast<AnimatedModel>(drawable);
			if (!model)
				continue;

			matrices.world = model->GetMatrix();
			UpdateBuffer(matricesBuf, matrices);
			BindBuffer(matricesBuf);

			model->DrawSkeleton();
		}

		//RESET DEPTH STENCIL STATE
		Graphics::Inst().GetContext().OMSetDepthStencilState(nullptr, 0);
	}
};