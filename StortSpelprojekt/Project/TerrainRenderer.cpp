#include "TerrainRenderer.h"

#include "Terrain.h"

TerrainRenderer::TerrainRenderer(float tesselationAmount)
{
	//BUFFER
	CreateBuffer(matrixBuf, sizeof(Matrices));
	CreateBuffer(lightBuf, sizeof(Matrix));

	BindBuffer(matrixBuf, Shader::DS);

	//SHADERS
	std::string byteCode;
	if (!LoadShader(vertexShader, vs_path, byteCode))
		return;

	CreateBuffer(tesselationBuf);
	UpdateBuffer(tesselationBuf, tesselationAmount);
	BindBuffer(tesselationBuf, Shader::HS);

	if (!LoadShader(hullShader, hs_path))
		return;

	if (!LoadShader(domainShader, ds_path))
		return;

	if (!LoadShader(geometryShader, gs_path))
		return;

	if (!LoadShader(pixelShader, forward_ps_path))
		return;

	Print("SUCCEEDED LOADING SHADERS", "TERRAIN RENDERER");

	//INPUT LAYOUT
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXTURECOORDS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	HRESULT hr = Graphics::Inst().GetDevice().CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), byteCode.c_str(), byteCode.length(), &inputLayout);
	if FAILED(hr)
	{
		Print("FAILED TO CREATE INPUT LAYOUT", "TERRAIN RENDERER");
		return;
	}
	Print("SUCCEEDED TO CREATE INPUT LAYOUT", "TERRAIN RENDERER");

	Print("SUCCEEDED TO INITIALIZE TERRAIN RENDERER");
	Print("=======================================");
}

TerrainRenderer::~TerrainRenderer()
{
	tesselationBuf->Release();
	matrixBuf->Release();
	lightBuf->Release();
	vertexShader->Release();
	hullShader->Release();
	domainShader->Release();
	geometryShader->Release();
	pixelShader->Release();
	inputLayout->Release();
}

void TerrainRenderer::Render(const Terrain& terrain)
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

	matrices.viewPerspective = ShaderData::Inst().cameraMatrix;
	matrices.lightViewPerspective = ShaderData::Inst().lightMatrix;

	UpdateBuffer(matrixBuf, matrices);
	BindBuffer(matrixBuf, Shader::DS);

	BindBuffer(tesselationBuf, Shader::HS);

	//DRAW
	terrain.Draw();
}