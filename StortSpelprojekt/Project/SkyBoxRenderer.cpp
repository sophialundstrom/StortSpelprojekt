#include "SkyBoxRenderer.h"
#include "BoundingVolumes.h"

SkyBoxRenderer::SkyBoxRenderer()
{
	std::string byteCode;

	std::wstring cubemapPath = L"SkyBox.dds";
	DirectX::CreateDDSTextureFromFile(&Graphics::Inst().GetDevice(), cubemapPath.c_str(), texture, textureView, 0, nullptr);


	if (!LoadShader(skyBoxVertexShader, vs_path, byteCode))
		return;
	
	if (!LoadShader(skyBoxPixelShader, ps_path))
		return;
	Print("SUCCEEDED LOADING SHADERS", "SKYBOX RENDERER");


	//BUFFERS
	CreateIndexBuffer(skyBoxIndices, BoxVolumeData::INDICES, BoxVolumeData::reversedindices);
	CreateVertexBuffer(skyboxMesh, stride, BoxVolumeData::VERTICES * stride, BoxVolumeData::vertices);

	CreateBuffer(matricesBuf, sizeof(Matrices));


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


	//BUFFER
	matrices.viewPerspective = gameCamera->GetProjectionMatrix().Transpose();
	UpdateBuffer(matricesBuf, matrices);
	BindBuffer(matricesBuf);

	//SHADERS
	BindShaders(skyBoxVertexShader, nullptr, nullptr, nullptr, skyBoxPixelShader);

	//SAVE SHADER DATA INSTANCE
	auto& shaderData = ShaderData::Inst();

	Graphics::Inst().GetContext().IASetIndexBuffer(skyBoxIndices, DXGI_FORMAT_R32_UINT, 0);
	Graphics::Inst().GetContext().IASetVertexBuffers(0, 1, &skyboxMesh, &stride, &offset);
	Graphics::Inst().GetContext().DrawIndexed(BoxVolumeData::INDICES, 0, 0);
}
