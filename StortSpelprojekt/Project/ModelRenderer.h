#pragma once
#include "Renderer.h"

class ModelRenderer : public Renderer
{
private:
	bool isLit;

	//BUFFER
	ID3D11Buffer* matricesBuf = nullptr;
	struct Matrices
	{
		Matrix world;
		Matrix viewPerspective;
		Matrix lightViewPerspective;
	} matrices;

	//SHADER PATHS
#ifdef _DEBUG
	const std::string vs_path = "../x64/Debug/ModelVertexShader.cso";
	const std::string deferred_ps_path = "../x64/Debug/DeferredModelPixelShader.cso";
	const std::string forward_ps_path = "../x64/Debug/ForwardModelPixelShader.cso";

	const std::string unlit_vs_path = "../x64/Debug/UnlitModelVertexShader.cso";
	const std::string unlit_forward_ps_path = "../x64/Debug/UnlitForwardModelPixelShader.cso";
#else
	const std::string vs_path = "../x64/Release/ModelVertexShader.cso";
	const std::string deferred_ps_path = "../x64/Release/DeferredModelPixelShader.cso";
	const std::string forward_ps_path = "../x64/Release/ForwardModelPixelShader.cso";

	const std::string unlit_vs_path = "../x64/Release/UnlitModelVertexShader.cso";
	const std::string unlit_forward_ps_path = "../x64/Release/UnlitForwardModelPixelShader.cso";
#endif

	//SHADERS
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;

	//INPUT LAYOUT
	ID3D11InputLayout* inputLayout = nullptr;
public:
	ModelRenderer(RenderMethod method, bool isLit);
	~ModelRenderer();

	virtual void Render() override;
};