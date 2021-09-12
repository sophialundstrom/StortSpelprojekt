#pragma once
#include "ShaderData.h"
#include "Collision.h"

class ColliderRenderer : public Renderer
{
private:
	//INDEX BUFFERS
	ID3D11Buffer* sphereIndices;
	ID3D11Buffer* boxIndices;

	//BUFFER
	ID3D11Buffer* matrixBuf = nullptr;

	//SHADER PATHS
#ifdef _DEBUG
	const std::string vs_path = "../x64/Debug/ColliderVertexShader.cso";
	const std::string ps_path = "../x64/Debug/ColliderPixelShader.cso";
#else
	const std::string vs_path = "../x64/Release/ColliderVertexShader.cso";
	const std::string ps_path = "../x64/Release/ColliderPixelShader.cso";
#endif

	//SHADERS
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;

	//INPUT LAYOUT
	ID3D11InputLayout* inputLayout = nullptr;
public:
	ColliderRenderer()
	{
		//INDEX BUFFERS
		static const UINT sIndices[] =
		{
			1, 2
		};
		CreateIndexBuffer(sphereIndices, ARRAYSIZE(sIndices), sIndices);

		static const UINT bIndices[] =
		{
			0, 1,
			1, 2,
			2, 3,
			3, 0,
			4, 5,
			5, 6,
			6, 7,
			7, 4,
			0, 4,
			1, 5,
			2, 6,
			3, 7
		};
		CreateIndexBuffer(boxIndices, ARRAYSIZE(bIndices), bIndices);

		//BUFFER
		CreateBuffer(matrixBuf, sizeof(Matrix));

		//SHADERS
		std::string byteCode;

		if (!LoadShader(vertexShader, vs_path, byteCode))
			return;
		if (!LoadShader(pixelShader, ps_path))
			return;

		Print("SUCCEEDED LOADING SHADERS", "COLLIDER RENDERER");

		//INPUT LAYOUT
		D3D11_INPUT_ELEMENT_DESC inputDesc[] =
		{ {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0} };

		HRESULT hr = Graphics::Inst().GetDevice().CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), byteCode.c_str(), byteCode.length(), &inputLayout);
		if FAILED(hr)
		{
			Print("FAILED TO CREATE INPUT LAYOUT", "COLLIDER RENDERER");
			return;
		}
		Print("SUCCEEDED TO CREATE INPUT LAYOUT", "COLLIDER RENDERER");

		Print("SUCCEEDED TO INITIALIZE COLLIDER RENDERER");
	}

	~ColliderRenderer()
	{
		matrixBuf->Release();
		vertexShader->Release();
		pixelShader->Release();
		inputLayout->Release();
	}

	virtual void Render() override
	{
		if (drawables.empty())

		//INPUT LAYOUT
		Graphics::Inst().GetContext().IASetInputLayout(inputLayout);

		//TOPOLOGY
		Graphics::Inst().GetContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		//SHADERS
		BindShaders(vertexShader, nullptr, nullptr, nullptr, pixelShader);

		//BUFFER
		UpdateBuffer(matrixBuf, ShaderData::Inst().cameraMatrix);
		BindBuffer(matrixBuf);

		for (auto& drawable : drawables)
		{
			auto collider = std::dynamic_pointer_cast<Collider>(drawable);
			if (!collider)
				continue;

			if (collider->Type() == ColliderType::BOX)
			{
				collider->Bind();
				Graphics::Inst().GetContext().IASetIndexBuffer(boxIndices, DXGI_FORMAT_R32_UINT, 0);
				Graphics::Inst().GetContext().DrawIndexed(24, 0, 0);
			}

			else
			{

			}

			
		}
	}
};