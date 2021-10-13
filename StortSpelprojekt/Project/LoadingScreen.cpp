#include "LoadingScreen.h"
#include "Random.h"

LoadingScreen::LoadingScreen()
{
	std::string path = backgrounds[Random::Integer(0, backgrounds.size())];
	texture = std::move(new Texture("Textures/" + path, ""));

#ifdef _DEBUG
	const std::string vs_path = "../x64/Debug/LoadingVS.cso";
	const std::string ps_path = "../x64/Debug/LoadingPS.cso";

#else
	const std::string vs_path = "../x64/Release/LoadingVS.cso";
	const std::string ps_path = "../x64/Release/LoadingPS.cso";
#endif

	std::string byteCode;
	if (!LoadShader(vShader, vs_path, byteCode))
	{
		std::cout << "FAILED TO LOAD VSHADER FOR LOADING SCREEN\n";
	}

	if (!LoadShader(pShader, ps_path))
	{
		std::cout << "FAILED TO LOAD PSHADER FOR LOADING SCREEN\n";
	}

	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	HRESULT hr = Graphics::Inst().GetDevice().CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), byteCode.c_str(), byteCode.length(), &layout);

	if (FAILED(hr))
	{
		std::cout << "FAILED TO CREATE INPUT LAYOUT FOR LOADING SCREEN\n";
	}

	D3D11_BUFFER_DESC vDesc;
	vDesc.ByteWidth = sizeof(minVertex);
	vDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vDesc.CPUAccessFlags = 0;
	vDesc.MiscFlags = 0;
	vDesc.StructureByteStride = sizeof(minVertex);

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &quad;

	hr = Graphics::Inst().GetDevice().CreateBuffer(&vDesc, &data, &vBuffer);

	if (FAILED(hr))
	{
		std::cout << "FAILED TO CREATE VERTEX BUFFER FOR LOADING SCREEN\n";
	}

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	Graphics::Inst().GetDevice().CreateSamplerState(&samplerDesc, &sampler);

	Graphics::Inst().BeginFrame();
	Graphics::Inst().GetContext().IASetInputLayout(layout);
	Graphics::Inst().GetContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	UINT stride = sizeof(minVertex);
	UINT offset = 0;
	Graphics::Inst().GetContext().IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);

	Graphics::Inst().GetContext().VSSetShader(vShader, nullptr, 0);

	Graphics::Inst().GetContext().PSSetShader(pShader, nullptr, 0);
	Graphics::Inst().GetContext().PSSetShaderResources(0, 1, &texture->srv);
	Graphics::Inst().GetContext().PSSetSamplers(0, 1, &sampler);

	Graphics::Inst().GetContext().Draw(4, 0);
	Graphics::Inst().EndFrame();
	// GET SWAP CHAIN AND PRESENT
}