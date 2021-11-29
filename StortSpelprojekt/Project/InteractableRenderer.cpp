#include "InteractableRenderer.h"

#include "Item.h"
#include "Target.h"

InteractableRenderer::InteractableRenderer()
{
	//BUFFER
	CreateBuffer(colorBuf);
	CreateBuffer(timeBuf);
	CreateBuffer(IDBuf);
	CreateBuffer(matricesBuf, sizeof(Matrices));

	//SHADERS
	std::string byteCode;
	if (!LoadShader(vertexShader, vs_path, byteCode))
		return;
	if (!LoadShader(pixelShader, ps_path))
		return;

	Print("SUCCEEDEDD LOADING SHADERS", "INTERACTABLE RENDERER");

	//INDEX BUFFER
	UINT indices[] { 0, 1, 2, 2, 1, 3 };
	CreateIndexBuffer(indexBuffer, ARRAYSIZE(indices), indices);

	//VERTEX BUFFER
	Vector3 vertices[]
	{
		{ -0.5f, 0.0f, 0.5f},
		{ 0.5f, 0.0f, 0.5f},
		{ -0.5f, 0.0f, -0.5f},
		{ 0.5f, 0.0f, -0.5f}
	};
	CreateVertexBuffer(vertexBuffer, stride, stride * ARRAYSIZE(vertices), vertices);

	//INPUT LAYOUT
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{ {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0} };

	HRESULT hr = Graphics::Inst().GetDevice().CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), byteCode.c_str(), byteCode.length(), &inputLayout);
	if FAILED(hr)
	{
		Print("FAILED TO CREATE INPUT LAYOUT", "INTERACTABLE RENDERER");
		return;
	}
	Print("SUCCEEDED TO CREATE INPUT LAYOUT", "INTERACTABLE RENDERER");

	//RASTERIZER STATE
	D3D11_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	hr = Graphics::Inst().GetDevice().CreateRasterizerState(&rasterizerDesc, &noCullState);
	if FAILED(hr)
	{
		Print("FAILED TO CREATE RASTERIZER STATE", "INTERACTABLE RENDERER");
		return;
	}
	Print("SUCCEEDED TO CREATE RASTERIZER STATE", "INTERACTABLE RENDERER");

	//MASKS
	circleMask = new Texture("Textures/CircleMask.png");

	Print("SUCCEEDED TO INITIALIZE INTERACTABLE RENDERER");
	Print("=======================================");
}

InteractableRenderer::~InteractableRenderer()
{
	delete circleMask;
	indexBuffer->Release();
	vertexBuffer->Release();
	colorBuf->Release();
	timeBuf->Release();
	IDBuf->Release();
	matricesBuf->Release();
	vertexShader->Release();
	pixelShader->Release();
	inputLayout->Release();
	noCullState->Release();
}

void InteractableRenderer::Render()
{
	if (drawables.empty())
		return;

	//INPUT LAYOUT
	Graphics::Inst().GetContext().IASetInputLayout(inputLayout);

	//BLEND STATE
	Graphics::Inst().EnableATCAlpha();

	//RASTERIZER STATE
	Graphics::Inst().GetContext().RSSetState(noCullState);

	//TOPOLOGY
	Graphics::Inst().GetContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//SHADERS
	BindShaders(vertexShader, nullptr, nullptr, nullptr, pixelShader);

	//BUFFERS
	BindBuffer(matricesBuf);
	BindBuffer(timeBuf, Shader::VS, 1);
	BindBuffer(IDBuf, Shader::VS, 2);
	BindBuffer(colorBuf, Shader::PS);

	matrices.viewPerspective = ShaderData::Inst().cameraMatrix;

	UpdateBuffer(timeBuf, Time::Get());

	//MASK
	circleMask->Bind();

	for (auto& drawable : drawables)
	{
		matrices.world = drawable->GetMatrix();

		auto item = std::dynamic_pointer_cast<Item>(drawable);
		if (item)
		{
			matrices.world = (Matrix::CreateScale(2.0f) * Matrix::CreateTranslation(drawable->GetPosition().x, drawable->GetPosition().y + 1.0f, drawable->GetPosition().z)).Transpose();
			UpdateBuffer(colorBuf, YELLOW);
		}
			
		auto target = std::dynamic_pointer_cast<Target>(drawable);
		if (target)
		{
			matrices.world *= Matrix::CreateScale(4.0f);
			matrices.world *= Matrix::CreateTranslation({ 0.0f, 0.5f, 0.0f }).Transpose();
			UpdateBuffer(colorBuf, RED);
		}

		Graphics::Inst().GetContext().IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		auto cache = matrices.world;
		for (UINT i = 0; i < NUM_PLANES; ++i)
		{
			matrices.world = cache * Matrix::CreateScale(0.75f + 0.5f * i, 1, 0.75f + 0.5f * i);

			UpdateBuffer(matricesBuf, matrices);
			UpdateBuffer(IDBuf, i);
			Graphics::Inst().GetContext().IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
			Graphics::Inst().GetContext().DrawIndexed(NUM_INDICES, 0, 0);
		}
	}

	Graphics::Inst().GetContext().RSSetState(nullptr);
	Graphics::Inst().DisableAlpha();
}