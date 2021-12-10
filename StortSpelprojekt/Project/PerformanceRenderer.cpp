#include "PerformanceRenderer.h"
#include "Model.h"

PerformanceRenderer::PerformanceRenderer()
{
	//BUFFER
	CreateBuffer(matricesBuf, sizeof(Matrices));
	CreateBuffer(modelMsBuffer);

	//DEPTH STENCIL STATE
	D3D11_DEPTH_STENCIL_DESC dssDesc = {};
	dssDesc.DepthEnable = false;
	Graphics::Inst().GetDevice().CreateDepthStencilState(&dssDesc, &depthStencilState);
	Print("SUCCEEDED CREATING DEPTH STENCIL STATE", "PERFORMANCE RENDERER");

	//SHADERS
	std::string byteCode;
	if (!LoadShader(vertexShader, vs_path, byteCode))
		return;

	if (!LoadShader(pixelShader, ps_path))
		return;

	Print("SUCCEEDED LOADING SHADERS", "PERFORMANCE RENDERER");

	//INPUT LAYOUT
	D3D11_INPUT_ELEMENT_DESC inputDesc =
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };

	HRESULT hr = Graphics::Inst().GetDevice().CreateInputLayout(&inputDesc, 1, byteCode.c_str(), byteCode.length(), &inputLayout);
	if FAILED(hr)
	{
		Print("FAILED TO CREATE INPUT LAYOUT", "PERFORMANCE RENDERER");
		return;
	}
	Print("SUCCEEDED TO CREATE INPUT LAYOUT", "PERFORMANCE RENDERER");

	Print("SUCCEEDED TO INITIALIZE PERFORMANCE RENDERER");
	Print("=======================================");
}

PerformanceRenderer::~PerformanceRenderer()
{
	matricesBuf->Release();
	vertexShader->Release();
	pixelShader->Release();
	inputLayout->Release();
	depthStencilState->Release();
}

void PerformanceRenderer::Render()
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

	Graphics::Inst().GetContext().OMSetDepthStencilState(depthStencilState, 0);
	int nrOBJ = 0;
	for (auto& drawable : drawables)
	{
		auto model = std::dynamic_pointer_cast<Model>(drawable);
		if (!model)
			continue;

		UpdateBuffer(modelMsBuffer, model->GetTTD());
		BindBuffer(modelMsBuffer, Shader::PS, 1);

		matrices.world = model->GetMatrix();

		UpdateBuffer(matricesBuf, matrices);
		BindBuffer(matricesBuf);

		model->Draw(false, false);
		nrOBJ++;
	}

	Graphics::Inst().GetContext().OMSetDepthStencilState(nullptr, 0);
}
