#include "ParticleRenderer.h"

#include "ParticleSystem.h"

ParticleRenderer::ParticleRenderer(RenderMethod method)
{
	//BUFFERS
	CreateBuffer(extentsBuf);
	CreateBuffer(matrixBuf, sizeof(Matrix));
	CreateBuffer(lifeTimeBuffer);

	//SHADERS
	std::string byteCode;
	if (!LoadShader(vertexShader, vs_path, byteCode))
		return;

	if (!LoadShader(geometryShader, gs_path))
		return;

	if (method == FORWARD)
	{
		if (!LoadShader(pixelShader, forward_ps_path))
			return;
	}

	else
	{
		if (!LoadShader(pixelShader, deferred_ps_path))
			return;
	}
	Print("SUCCEEDED LOADING SHADERS", "PARTICLE RENDERER");

	//INPUT LAYOUT
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"DIRECTION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"LIFETIME", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"VELOCITY", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	HRESULT hr = Graphics::Inst().GetDevice().CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), byteCode.c_str(), byteCode.length(), &inputLayout);
	if FAILED(hr)
	{
		Print("FAILED TO CREATE INPUT LAYOUT", "PARTICLE RENDERER");
		return;
	}
	Print("SUCCEEDED TO CREATE INPUT LAYOUT", "PARTICLE RENDERER");

	Print("SUCCEEDED TO INITIALIZE PARTICLE RENDERER");
	Print("=======================================");
}

ParticleRenderer::~ParticleRenderer()
{
	extentsBuf->Release();
	matrixBuf->Release();
	vertexShader->Release();
	geometryShader->Release();
	pixelShader->Release();
	inputLayout->Release();
	lifeTimeBuffer->Release();
}

void ParticleRenderer::Render()
{
	if (drawables.empty())
		return;

	//INPUT LAYOUT
	Graphics::Inst().GetContext().IASetInputLayout(inputLayout);

	//TOPOLOGY
	Graphics::Inst().GetContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	//BLEND
	Graphics::Inst().EnableAlpha();

	//SHADERS
	BindShaders(vertexShader, nullptr, nullptr, geometryShader, pixelShader);

	//SAVE SHADER DATA INSTANCE
	auto& shaderData = ShaderData::Inst();

	//BUFFERS
	BindBuffer(shaderData.cameraPositionBuf, Shader::GS);

	UpdateBuffer(matrixBuf, shaderData.cameraMatrix);
	BindBuffer(matrixBuf, Shader::GS, 1);

	for (auto& drawable : drawables)
	{
		auto particleSystem = std::dynamic_pointer_cast<ParticleSystem>(drawable);
		if (!particleSystem)
			continue;

		UpdateBuffer(extentsBuf, particleSystem->GetParticleExtents());
		BindBuffer(extentsBuf, Shader::GS, 2);

		UpdateBuffer(lifeTimeBuffer, particleSystem->GetParticlesLifetime());
		BindBuffer(lifeTimeBuffer, Shader::VS);

		particleSystem->Draw();
	}
	Graphics::Inst().DisableAlpha();
}