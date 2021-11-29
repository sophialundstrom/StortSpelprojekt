#include "SkyBoxRenderer.h"
#include "BoundingVolumes.h"
#include "stb_image.h"

void SkyBoxRenderer::BuildCubeMap()
{

}

SkyBoxRenderer::SkyBoxRenderer()
{
	std::string byteCode;
	//DDSFileVariant
	/*
	std::wstring cubemapPath = L"SkyBox.dds";
	DirectX::CreateDDSTextureFromFile(&Graphics::Inst().GetDevice(), cubemapPath.c_str(), texture, textureView, 0, nullptr);
	*/

	//Shaders
	if (!LoadShader(skyBoxVertexShader, vs_path, byteCode))
		return;
	
	if (!LoadShader(skyBoxPixelShader, ps_path))
		return;
	Print("SUCCEEDED LOADING SHADERS", "SKYBOX RENDERER");

	//BUFFERS
	CreateIndexBuffer(skyBoxIndices, BoxVolumeData::INDICES, BoxVolumeData::reversedindices);
	CreateVertexBuffer(skyboxMesh, stride, BoxVolumeData::VERTICES * stride, BoxVolumeData::vertices);

	CreateBuffer(matricesBuf, sizeof(Matrix));

	//INPUT LAYOUT
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	HRESULT hr = Graphics::Inst().GetDevice().CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), byteCode.c_str(), byteCode.length(), &inputLayout);
	if FAILED(hr)
	{
		Print("FAILED TO CREATE INPUT LAYOUT", "SKYBOX RENDERER");
		return;
	}

	//DepthStencilState
	D3D11_DEPTH_STENCIL_DESC dsDesc = CD3D11_DEPTH_STENCIL_DESC{ CD3D11_DEFAULT{} };
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	Graphics::Inst().GetDevice().CreateDepthStencilState(&dsDesc, &skyboxDepthStencil);

	Graphics::Inst();

	Print("SUCCEEDED TO CREATE INPUT LAYOUT", "SKYBOX RENDERER");

	Print("SUCCEEDED TO INITIALIZE SKYBOX RENDERER");
	Print("=======================================");
}

void SkyBoxRenderer::Render()
{
	//INPUT LAYOUT
	Graphics::Inst().GetContext().IASetInputLayout(inputLayout);

	//TOPOLOGY
	Graphics::Inst().GetContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//SAVE SHADER DATA INSTANCE
	auto& shaderData = ShaderData::Inst();

	//BUFFER

	//PorjectionMatrixClippar bort geometri wtf!?
	UpdateBuffer(matricesBuf, shaderData.cameraMatrix);
	BindBuffer(matricesBuf);

	//SHADERS
	BindShaders(skyBoxVertexShader, nullptr, nullptr, nullptr, skyBoxPixelShader);

	Graphics::Inst().GetContext().OMSetDepthStencilState(skyboxDepthStencil, 1);
	
	Graphics::Inst().GetContext().IASetIndexBuffer(skyBoxIndices, DXGI_FORMAT_R32_UINT, 0);
	Graphics::Inst().GetContext().IASetVertexBuffers(0, 1, &skyboxMesh, &stride, &offset);
	Graphics::Inst().GetContext().DrawIndexed(BoxVolumeData::INDICES, 0, 0);
	
	Graphics::Inst().GetContext().OMSetDepthStencilState(nullptr, 0);
}

