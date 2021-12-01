#include "SkyBoxRenderer.h"
#include "BoundingVolumes.h"
#include "stb_image.h"

void SkyBoxRenderer::BuildCubeMap(std::string skyboxFolderName, ID3D11Texture2D*& texture, ID3D11ShaderResourceView*& textureView)
{
	HRESULT hr;

	int imgWidth, imgHeight;
	unsigned char** image;
	image = new unsigned char*[6];

	std::string textures[] = { 
		{skyboxTexturePath + skyboxFolderName + "/right.png"},
		{skyboxTexturePath + skyboxFolderName + "/left.png"},
		{skyboxTexturePath + skyboxFolderName + "/up.png"},
		{skyboxTexturePath + skyboxFolderName + "/down.png"},
		{skyboxTexturePath + skyboxFolderName + "/front.png"},
		{skyboxTexturePath + skyboxFolderName + "/back.png"},
	};

	for (int i = 0; i < 6; i++)
	{
		image[i] = stbi_load((textures[i]).c_str(), &imgWidth, &imgHeight, nullptr, STBI_rgb_alpha);
	}


	// texture descriptor
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = imgWidth;
	textureDesc.Height = imgHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 6;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;


	// subresource data
	D3D11_SUBRESOURCE_DATA data[6];
	for (int i = 0; i < 6; i++)
	{
		data[i].pSysMem = image[i];
		data[i].SysMemPitch = imgWidth * STBI_rgb_alpha;
		data[i].SysMemSlicePitch = 0;
	}


	//Create texture resource
	hr = Graphics::Inst().GetDevice().CreateTexture2D(&textureDesc, data, &texture);
	if (FAILED(hr))
		std::cout << "FAILED TO CREATE TEXTURE2D\n";

	//Create the resourceview on the texture
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	hr = Graphics::Inst().GetDevice().CreateShaderResourceView(texture, &srvDesc, &textureView);

	if (FAILED(hr))
		std::cout << "FAILED TO CREATE SRV\n";
}

SkyBoxRenderer::SkyBoxRenderer()
{
	BuildCubeMap("DayTime", dayTexture, dayTextureView);
	BuildCubeMap("NightTIme", nightTexture, nightTextureView);

	std::string byteCode;
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
	CreateBuffer(fadeBuffer, sizeof(TransitionProperties));

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

void SkyBoxRenderer::PullDayNightSlider(float newValue)
{
	dayNightSlider = newValue;
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
	UpdateBuffer(matricesBuf, shaderData.cameraMatrix);
	BindBuffer(matricesBuf);
	TransitionProperties tp;

	//playTime += Time::GetDelta();
	//tp.fadeSlider = (0.5f * sin(playTime) + 0.5f);

	tp.fadeSlider = dayNightSlider;

	UpdateBuffer(fadeBuffer, tp);
	BindBuffer(fadeBuffer, Shader::PS, 0U);

	//SHADERS
	BindShaders(skyBoxVertexShader, nullptr, nullptr, nullptr, skyBoxPixelShader);

	Graphics::Inst().GetContext().OMSetDepthStencilState(skyboxDepthStencil, 1);
	Graphics::Inst().GetContext().PSSetShaderResources(0, 1, &dayTextureView);
	Graphics::Inst().GetContext().PSSetShaderResources(1, 1, &nightTextureView);
	Graphics::Inst().GetContext().IASetIndexBuffer(skyBoxIndices, DXGI_FORMAT_R32_UINT, 0u);
	Graphics::Inst().GetContext().IASetVertexBuffers(0, 1, &skyboxMesh, &stride, &offset);
	Graphics::Inst().GetContext().DrawIndexed(BoxVolumeData::INDICES, 0, 0);
	Graphics::Inst().GetContext().OMSetDepthStencilState(nullptr, 0);
}

