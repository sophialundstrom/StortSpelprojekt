//#pragma once
//#include "Renderer.h"
//#include "Model.h"
//
//class ModelRenderer : public Renderer
//{
//private:
//	//BUFFERS
//	ID3D11Buffer* matrices_buf = nullptr;
//	struct Matrices
//	{
//		Matrix world;
//		Matrix viewPerspective;
//		Matrix lightViewPerspective;
//	} matrices;
//
//	//SHADER PATHS
//#ifdef _DEBUG
//	const std::string vs_path = "../x64/Debug/ModelVertexShader.cso";
//	const std::string ps_path = "../x64/Debug/ModelPixelShader.cso";
//#else
//	const std::string vs_path = "../x64/Release/ModelVertexShader.cso";
//	const std::string ps_path = "../x64/Release/ModelPixelShader.cso";
//#endif
//	ID3D11VertexShader* vertexShader = nullptr;
//	ID3D11PixelShader* pixelShader = nullptr;
//public:
//	ModelRenderer(std::unique_ptr<ShaderData>& shaderData)
//		:matrices()
//	{
//		//BUFFERS
//		CreateBuffer(matrices_buf, sizeof(Matrices));
//
//		//SHADERS
//		std::string byteCode;
//		LoadShader(vertexShader, vs_path, byteCode);
//		LoadShader(pixelShader, ps_path);
//
//		shaderData = std::make_unique<ShaderData>(byteCode);
//	}
//
//	~ModelRenderer()
//	{
//		matrices_buf->Release();
//
//		vertexShader->Release();
//		pixelShader->Release();
//	}
//
//	void Render()
//	{
//		if (drawables.empty())
//			return;
//
//		//INPUT LAYOUT
//		Graphics::Inst().GetContext().IASetInputLayout(ShaderData::Inst().inputLayout);
//
//		//TOPOLOGY
//		Graphics::Inst().GetContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//		//CAMERA & LIGHT MATRIX
//		matrices.viewPerspective = ShaderData::Inst().cameraMatrix;
//		matrices.lightViewPerspective = ShaderData::Inst().lightMatrix;
//
//		//RENDER BOUND DRAWABLES
//		for (auto& drawable : drawables)
//		{
//			auto model = std::dynamic_pointer_cast<Model>(drawable);
//			if (!model)
//				continue;
//
//			//SHADERS
//			BindShaders(vertexShader, nullptr, nullptr, nullptr, pixelShader);
//
//			//BUFFER
//			matrices.world = model->GetMatrix().Transpose();
//			UpdateBuffer(matrices_buf, matrices);
//			BindBuffer(matrices_buf);
//
//			model->Draw();
//		}
//	}
//};