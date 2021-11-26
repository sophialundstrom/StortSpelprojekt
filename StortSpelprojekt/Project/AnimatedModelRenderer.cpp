#include "AnimatedModelRenderer.h"

#include "AnimatedModel.h"

AnimatedModelRenderer::AnimatedModelRenderer()
{
	//BUFFER
	CreateBuffer(matricesBuf, sizeof(Matrices));

	//SHADERS
	std::string byteCode;

	if (!LoadShader(vertexShader, vs_path, byteCode))
		return;

	if (!LoadShader(pixelShader, forward_ps_path))
		return;

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
	Print("=======================================");
}

AnimatedModelRenderer::~AnimatedModelRenderer()
{
	matricesBuf->Release();
	vertexShader->Release();
	pixelShader->Release();
	inputLayout->Release();
}

void AnimatedModelRenderer::Render()
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