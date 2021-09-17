#pragma once
#include "ShaderData.h"
#include "Collision.h"

class ColliderRenderer : public Renderer
{
private:
	//INDEX BUFFERS
	const UINT SPHERE_INDICES = 48;
	ID3D11Buffer* sphereIndices;

	const UINT BOX_INDICES = 24;
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
			0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 0,
			8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 8,
			16, 17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 23, 16
		};
		CreateIndexBuffer(sphereIndices, SPHERE_INDICES, sIndices);

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
		CreateIndexBuffer(boxIndices, BOX_INDICES, bIndices);

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
		sphereIndices->Release();
		boxIndices->Release();
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

			collider->Bind();

			if (collider->Type() == ColliderType::BOX)
			{
				Graphics::Inst().GetContext().IASetIndexBuffer(boxIndices, DXGI_FORMAT_R32_UINT, 0);
				Graphics::Inst().GetContext().DrawIndexed(BOX_INDICES, 0, 0);
			}

			else
			{
				Graphics::Inst().GetContext().IASetIndexBuffer(sphereIndices, DXGI_FORMAT_R32_UINT, 0);
				Graphics::Inst().GetContext().DrawIndexed(SPHERE_INDICES, 0, 0);
			}
		}
	}
};