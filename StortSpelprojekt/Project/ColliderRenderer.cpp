#include "ColliderRenderer.h"

#include "Collision.h"

ColliderRenderer::ColliderRenderer()
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

	static const UINT rIndices[] =
	{ 0, 1 };
	CreateIndexBuffer(rayIndices, RAY_INDICES, rIndices);

	static const UINT fIndices[] =
	{
		0, 1, 1, 2, 2, 3, 3, 0, 4, 5, 5, 6, 6, 7, 7, 4, 0, 4, 1, 5, 2, 6, 3, 7
	};
	CreateIndexBuffer(frustumIndices, FRUSTUM_INDICES, fIndices);

	//BUFFER
	CreateBuffer(matrixBuf, sizeof(Matrix));

	//SHADERS
	std::string byteCode;

	if (!LoadShader(vertexShader, vs_path, byteCode))
		return;

	if (!LoadShader(pixelShader, forward_ps_path))
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
	Print("=======================================");
}

ColliderRenderer::~ColliderRenderer()
{
	rayIndices->Release();
	boxIndices->Release();
	frustumIndices->Release();
	sphereIndices->Release();
	matrixBuf->Release();
	vertexShader->Release();
	pixelShader->Release();
	inputLayout->Release();
}

void ColliderRenderer::Render()
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

		switch (collider->Type())
		{
		case ColliderType::BOX:
			Graphics::Inst().GetContext().IASetIndexBuffer(boxIndices, DXGI_FORMAT_R32_UINT, 0);
			Graphics::Inst().GetContext().DrawIndexed(BOX_INDICES, 0, 0);
			break;

		case ColliderType::SPHERE:
			Graphics::Inst().GetContext().IASetIndexBuffer(sphereIndices, DXGI_FORMAT_R32_UINT, 0);
			Graphics::Inst().GetContext().DrawIndexed(SPHERE_INDICES, 0, 0);
			break;

		case ColliderType::RAY:
			Graphics::Inst().GetContext().IASetIndexBuffer(rayIndices, DXGI_FORMAT_R32_UINT, 0);
			Graphics::Inst().GetContext().DrawIndexed(RAY_INDICES, 0, 0);
			break;

		case ColliderType::FRUSTUM:
			Graphics::Inst().GetContext().IASetIndexBuffer(frustumIndices, DXGI_FORMAT_R32_UINT, 0);
			Graphics::Inst().GetContext().DrawIndexed(FRUSTUM_INDICES, 0, 0);
			break;
		}
	}
}