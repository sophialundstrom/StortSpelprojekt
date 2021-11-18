#include "WaterRenderer.h"
#include "Water.h"

WaterRenderer::WaterRenderer(float tesselationAmount)
{
	//BUFFERS
	CreateBuffer(matrixBuf, sizeof(Matrix));
	CreateBuffer(lightBuf, sizeof(Matrix));
	CreateBuffer(timeBuf);

	//SHADERS
	std::string byteCode;
	if (!LoadShader(vertexShader, vs_path, byteCode))
		return;

	CreateBuffer(tesselationBuf);
	UpdateBuffer(tesselationBuf, tesselationAmount);

	if (!LoadShader(hullShader, hs_path))
		return;

	if (!LoadShader(domainShader, ds_path))
		return;

	if (!LoadShader(geometryShader, gs_path))
		return;

	if (!LoadShader(pixelShader, ps_path))
		return;

	Print("SUCCEEDED LOADING SHADERS", "WATER RENDERER");

	//INPUT LAYOUT
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXTURECOORDS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	HRESULT hr = Graphics::Inst().GetDevice().CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), byteCode.c_str(), byteCode.length(), &inputLayout);
	if FAILED(hr)
	{
		Print("FAILED TO CREATE INPUT LAYOUT", "WATER RENDERER");
		return;
	}
	Print("SUCCEEDED TO CREATE INPUT LAYOUT", "WATER RENDERER");

	Print("SUCCEEDED TO INITIALIZE WATER RENDERER");
	Print("=======================================");
}

WaterRenderer::~WaterRenderer()
{
	tesselationBuf->Release();
	matrixBuf->Release();
	timeBuf->Release();
	lightBuf->Release();
	vertexShader->Release();
	hullShader->Release();
	domainShader->Release();
	geometryShader->Release();
	pixelShader->Release();
	inputLayout->Release();
}

void WaterRenderer::Render()
{
	//INPUT LAYOUT
	Graphics::Inst().GetContext().IASetInputLayout(inputLayout);

	//TOPOLOGY
	Graphics::Inst().GetContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	//SHADERS
	BindShaders(vertexShader, hullShader, domainShader, geometryShader, pixelShader);

	//BUFFER(S)
	UpdateBuffer(lightBuf, ShaderData::Inst().lightMatrix);
	BindBuffer(lightBuf, Shader::PS);

	UpdateBuffer(matrixBuf, ShaderData::Inst().cameraMatrix);
	BindBuffer(matrixBuf, Shader::DS);

	BindBuffer(tesselationBuf, Shader::HS);

	UpdateBuffer(timeBuf, Time::Get());
	BindBuffer(timeBuf, Shader::DS, 1);

	//DRAW
	for (auto& drawable : drawables)
	{
		auto water = std::dynamic_pointer_cast<Water>(drawable);
		if (!water)
			continue;

		water->Draw();
	}
}

void WaterRenderer::Render(const Water& water)
{
	//INPUT LAYOUT
	Graphics::Inst().GetContext().IASetInputLayout(inputLayout);

	//TOPOLOGY
	Graphics::Inst().GetContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	//SHADERS
	BindShaders(vertexShader, hullShader, domainShader, geometryShader, pixelShader);

	//BUFFER(S)
	UpdateBuffer(lightBuf, ShaderData::Inst().lightMatrix);
	BindBuffer(lightBuf, Shader::PS);

	UpdateBuffer(matrixBuf, ShaderData::Inst().cameraMatrix);
	BindBuffer(matrixBuf, Shader::DS);

	BindBuffer(tesselationBuf, Shader::HS);

	UpdateBuffer(timeBuf, Time::Get());
	BindBuffer(timeBuf, Shader::DS, 1);

	//DRAW
	water.Draw();
}