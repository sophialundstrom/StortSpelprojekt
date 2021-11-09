#include "IDRenderer.h"

#include "Model.h"
#include "BoundingVolumes.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

IDRenderer::IDRenderer()
{
	//INDEX BUFFERS
	CreateIndexBuffer(sphereIndices, SphereVolumeData::INDICES, SphereVolumeData::indices);
	CreateIndexBuffer(boxIndices, BoxVolumeData::INDICES, BoxVolumeData::indices);

	//VERTEX BUFFERS
	CreateVertexBuffer(sphereVertexBuffer, stride, SphereVolumeData::VERTICES * stride, SphereVolumeData::vertices);
	CreateVertexBuffer(boxVertexBuffer, stride, BoxVolumeData::VERTICES * stride, BoxVolumeData::vertices);

	//BUFFER
	CreateBuffer(matricesBuf, sizeof(Matrices));
	CreateBuffer(idBuffer);

	//SHADERS
	std::string byteCode;
	std::string vByteCode;

	if (!LoadShader(volumeVertexShader, volume_vs_path, vByteCode))
		return;
	if (!LoadShader(vertexShader, vs_path, byteCode))
		return;
	if (!LoadShader(pixelShader, ps_path))
		return;
	Print("SUCCEEDED LOADING SHADERS", "ID RENDERER");

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
		Print("FAILED TO CREATE INPUT LAYOUT", "ID RENDERER");
		return;
	}
	Print("SUCCEEDED TO CREATE INPUT LAYOUT", "ID RENDERER");

	D3D11_INPUT_ELEMENT_DESC vInputDesc[] =
	{ {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0} };

	hr = Graphics::Inst().GetDevice().CreateInputLayout(vInputDesc, ARRAYSIZE(vInputDesc), vByteCode.c_str(), vByteCode.length(), &volumeInputLayout);
	if FAILED(hr)
	{
		Print("FAILED TO CREATE INPUT LAYOUT", "ID RENDERER (VOLUME)");
		return;
	}
	Print("SUCCEEDED TO CREATE INPUT LAYOUT", "ID RENDERER (VOLUME)");

	D3D11_TEXTURE2D_DESC textDesc = {};
	textDesc.Width = (UINT)Graphics::Inst().GetViewport().Width;
	textDesc.Height = (UINT)Graphics::Inst().GetViewport().Height;
	textDesc.MipLevels = 1;
	textDesc.ArraySize = 1;
	textDesc.Format = DXGI_FORMAT_R32_UINT;
	textDesc.SampleDesc.Count = 1;
	textDesc.SampleDesc.Quality = 0;
	textDesc.Usage = D3D11_USAGE_DEFAULT;
	textDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
	textDesc.CPUAccessFlags = 0;
	textDesc.MiscFlags = 0;

	hr = Graphics::Inst().GetDevice().CreateTexture2D(&textDesc, nullptr, &idTexture);
	if FAILED(hr)
	{
		Print("FAILED TO CREATE 2D TEXTURE", "ID RENDERER");
	}

	textDesc.Width = 1;
	textDesc.Height = 1;
	textDesc.BindFlags = 0;
	textDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	textDesc.Usage = D3D11_USAGE_STAGING;

	hr = Graphics::Inst().GetDevice().CreateTexture2D(&textDesc, nullptr, &idTextureData);
	if FAILED(hr)
	{
		Print("FAILED TO CREATE 2D TEXTURE", "ID RENDERER");
	}

	hr = Graphics::Inst().GetDevice().CreateRenderTargetView(idTexture, nullptr, &idRTV);
	if FAILED(hr)
	{
		Print("FAILED TO CREATE RENDER TARGET VIEW", "ID RENDERER");
	}


	Print("SUCCEEDED TO INITIALIZE ID RENDERER");
	Print("=======================================");
}

IDRenderer::~IDRenderer()
{
	idBuffer->Release();
	matricesBuf->Release();
	volumeVertexShader->Release();
	vertexShader->Release();
	pixelShader->Release();
	volumeInputLayout->Release();
	inputLayout->Release();
	idRTV->Release();
	idTexture->Release();
	sphereIndices->Release();
	sphereVertexBuffer->Release();
	boxIndices->Release();
	boxVertexBuffer->Release();
	idTextureData->Release();
}

void IDRenderer::Render()
{
	if (drawables.empty())
		return;

	//TOPOLOGY
	Graphics::Inst().GetContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//SAVE SHADER DATA INSTANCE
	auto& shaderData = ShaderData::Inst();

	matrices.viewPerspective = shaderData.cameraMatrix;

	for (auto& drawable : drawables)
	{
		UpdateBuffer(idBuffer, drawable->GetID());
		BindBuffer(idBuffer, Shader::VS, 1);

		matrices.world = drawable->GetMatrix();

		UpdateBuffer(matricesBuf, matrices);
		BindBuffer(matricesBuf);

		auto model = std::dynamic_pointer_cast<Model>(drawable);
		if (model)
		{
			if (currentLayout != inputLayout)
			{
				currentLayout = inputLayout;
				Graphics::Inst().GetContext().IASetInputLayout(currentLayout);
				BindShaders(vertexShader, nullptr, nullptr, nullptr, pixelShader);
			}
			model->Draw(false, false);
			continue;
		}

		auto volume = std::dynamic_pointer_cast<BoundingVolume>(drawable);
		if (volume)
		{
			if (currentLayout != volumeInputLayout)
			{
				currentLayout = volumeInputLayout;
				Graphics::Inst().GetContext().IASetInputLayout(currentLayout);
				BindShaders(volumeVertexShader, nullptr, nullptr, nullptr, pixelShader);
			}

			switch (volume->type)
			{
			case VolumeType::BOX:
				Graphics::Inst().GetContext().IASetIndexBuffer(boxIndices, DXGI_FORMAT_R32_UINT, 0);
				Graphics::Inst().GetContext().IASetVertexBuffers(0, 1, &boxVertexBuffer, &stride, &offset);
				Graphics::Inst().GetContext().DrawIndexed(BoxVolumeData::INDICES, 0, 0);
				break;

			case VolumeType::SPHERE:
				Graphics::Inst().GetContext().IASetIndexBuffer(sphereIndices, DXGI_FORMAT_R32_UINT, 0);
				Graphics::Inst().GetContext().IASetVertexBuffers(0, 1, &sphereVertexBuffer, &stride, &offset);
				Graphics::Inst().GetContext().DrawIndexed(SphereVolumeData::INDICES, 0, 0);
				break;
			}
			continue;
		}

	}

	currentLayout = nullptr;
}

void IDRenderer::UpdateViewport(UINT width, UINT height)
{
	if (idTexture)
	{
		idTexture->Release();
		idTexture = nullptr;
	}

	if (idTextureData)
	{
		idTextureData->Release();
		idTextureData = nullptr;
	}

	if (idRTV)
	{
		idRTV->Release();
		idRTV = nullptr;
	}

	HRESULT hr;

	D3D11_TEXTURE2D_DESC textDesc = {};
	textDesc.Width = width;
	textDesc.Height = height;
	textDesc.MipLevels = 1;
	textDesc.ArraySize = 1;
	textDesc.Format = DXGI_FORMAT_R32_UINT;
	textDesc.SampleDesc.Count = 1;
	textDesc.SampleDesc.Quality = 0;
	textDesc.Usage = D3D11_USAGE_DEFAULT;
	textDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
	textDesc.CPUAccessFlags = 0;
	textDesc.MiscFlags = 0;

	hr = Graphics::Inst().GetDevice().CreateTexture2D(&textDesc, nullptr, &idTexture);
	if FAILED(hr)
	{
		Print("FAILED TO CREATE 2D TEXTURE", "ID RENDERER");
	}

	textDesc.Width = 1;
	textDesc.Height = 1;
	textDesc.BindFlags = 0;
	textDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	textDesc.Usage = D3D11_USAGE_STAGING;

	hr = Graphics::Inst().GetDevice().CreateTexture2D(&textDesc, nullptr, &idTextureData);
	if FAILED(hr)
	{
		Print("FAILED TO CREATE 2D TEXTURE", "ID RENDERER");
	}

	hr = Graphics::Inst().GetDevice().CreateRenderTargetView(idTexture, nullptr, &idRTV);
	if FAILED(hr)
	{
		Print("FAILED TO CREATE RENDER TARGET VIEW", "ID RENDERER");
	}
}

void IDRenderer::BeginFrame(ID3D11DepthStencilView* dsv, D3D11_VIEWPORT& viewport)
{
	auto& context = Graphics::Inst().GetContext();

	context.RSSetViewports(1, &viewport);
	context.OMSetRenderTargets(1, &idRTV, dsv);
	context.ClearRenderTargetView(idRTV, Graphics::Inst().GetBackgroundColor());
	context.ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

int IDRenderer::GetObjectID(int xPix, int yPix)
{
	D3D11_BOX box = {};
	box.left = xPix;
	box.top = yPix;
	box.front = 0;
	box.right = xPix + 1;
	box.bottom = yPix + 1;
	box.back = 1;

	Graphics::Inst().GetContext().CopySubresourceRegion(idTextureData,0,0,0,0, idTexture, 0, &box);

	D3D11_TEXTURE2D_DESC textureDesc;
	idTextureData->GetDesc(&textureDesc);

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	HRESULT hr = Graphics::Inst().GetContext().Map(idTextureData, 0, D3D11_MAP_READ, 0, &mappedResource);
	if FAILED(hr)
	{
		Print("FAILED TO MAP SUBRESOURCE", "ID RENDERER::COPYING TEXTURE");
		return 0;
	}

	int id = 0;
	UINT32* data = static_cast<UINT32*>(mappedResource.pData);

	memcpy(&id, data, sizeof(UINT32));

	Graphics::Inst().GetContext().Unmap(idTextureData, 0);
	return id;
}