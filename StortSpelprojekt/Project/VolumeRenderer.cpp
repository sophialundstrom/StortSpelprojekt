#include "VolumeRenderer.h"

#include "BoundingVolumes.h"

VolumeRenderer::VolumeRenderer()
{
	//INDEX BUFFERS
	CreateIndexBuffer(sphereIndices, SphereVolumeData::INDICES, SphereVolumeData::indices);
	CreateIndexBuffer(boxIndices, BoxVolumeData::INDICES, BoxVolumeData::indices);

	//VERTEX BUFFERS
	CreateVertexBuffer(sphereVertexBuffer, stride, SphereVolumeData::VERTICES * stride, SphereVolumeData::vertices);
	CreateVertexBuffer(boxVertexBuffer, stride, BoxVolumeData::VERTICES * stride, BoxVolumeData::vertices);

	//BUFFER
	CreateBuffer(matricesBuf, sizeof(Matrices));

	//SHADERS
	std::string byteCode;

	if (!LoadShader(vertexShader, vs_path, byteCode))
		return;


	if (!LoadShader(pixelShader, forward_ps_path))
		return;

	Print("SUCCEEDED LOADING SHADERS", "VOLUME RENDERER");

	//INPUT LAYOUT
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{ {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0} };

	HRESULT hr = Graphics::Inst().GetDevice().CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), byteCode.c_str(), byteCode.length(), &inputLayout);
	if FAILED(hr)
	{
		Print("FAILED TO CREATE INPUT LAYOUT", "VOLUME RENDERER");
		return;
	}
	Print("SUCCEEDED TO CREATE INPUT LAYOUT", "VOLUME RENDERER");

	Print("SUCCEEDED TO INITIALIZE VOLUME RENDERER");
	Print("=======================================");
}

VolumeRenderer::~VolumeRenderer()
{
	sphereIndices->Release();
	boxIndices->Release();
	matricesBuf->Release();
	vertexShader->Release();
	pixelShader->Release();
	inputLayout->Release();
	sphereVertexBuffer->Release();
	boxVertexBuffer->Release();
}

void VolumeRenderer::Render()
{
	if (drawables.empty())
		return;

	//INPUT LAYOUT
	Graphics::Inst().GetContext().IASetInputLayout(inputLayout);

	//BLEND STATE
	Graphics::Inst().EnableAlpha();

	//TOPOLOGY
	Graphics::Inst().GetContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//SHADERS
	BindShaders(vertexShader, nullptr, nullptr, nullptr, pixelShader);

	//BUFFER
	BindBuffer(matricesBuf);

	matrices.viewPerspective = ShaderData::Inst().cameraMatrix;

	for (auto& drawable : drawables)
	{
		auto volume = std::dynamic_pointer_cast<BoundingVolume>(drawable);
		if (!volume)
			continue;


		matrices.world = drawable->GetMatrix();
		UpdateBuffer(matricesBuf, matrices);

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
	}

	Graphics::Inst().DisableAlpha();
}