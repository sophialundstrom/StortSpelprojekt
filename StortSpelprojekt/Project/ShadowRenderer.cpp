#include "ShadowRenderer.h"

#include "Model.h"

ShadowRenderer::ShadowRenderer()
{
	//BUFFER
	CreateBuffer(matrixBuf, sizeof(Matrix));

	//SHADER
	std::string byteCode;
	if (!LoadShader(vertexShader, vs_path, byteCode))
		return;

	Print("SUCCEEDED LOADING SHADER", "SHADOW RENDERER");

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
		Print("FAILED TO CREATE INPUT LAYOUT", "SHADOW RENDERER");
		return;
	}
	Print("SUCCEEDED TO CREATE INPUT LAYOUT", "SHADOW RENDERER");

	Print("SUCCEEDED TO INITIALIZE SHADOW RENDERER");
	Print("=======================================");
}

ShadowRenderer::~ShadowRenderer()
{
	matrixBuf->Release();
	vertexShader->Release();
	inputLayout->Release();
}

void ShadowRenderer::Render()
{
	if (drawables.empty())
		return;

	//SET SHADOW MAP AS RENDER TARGET
	ShaderData::Inst().shadowMap.BindAsRenderTarget();

	//INPUT LAYOUT
	Graphics::Inst().GetContext().IASetInputLayout(inputLayout);

	//TOPOLOGY
	Graphics::Inst().GetContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//RENDER BOUND DRAWABLES
	for (auto& drawable : drawables)
	{
		auto model = std::dynamic_pointer_cast<Model>(drawable);
		if (!model)
			continue;

		//Matrices
		UpdateBuffer(matrixBuf, ShaderData::Inst().lightMatrix * model->GetMatrix());
		BindBuffer(matrixBuf);

		//SHADER
		BindShaders(vertexShader);

		model->Draw(false);
	}
}