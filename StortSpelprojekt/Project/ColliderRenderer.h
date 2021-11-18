#pragma once
#include "Renderer.h"

class ColliderRenderer : public Renderer
{
private:
	//INDEX BUFFERS
	const UINT SPHERE_INDICES = 48;
	ID3D11Buffer* sphereIndices = nullptr;

	const UINT BOX_INDICES = 24;
	ID3D11Buffer* boxIndices = nullptr;

	const UINT RAY_INDICES = 2;
	ID3D11Buffer* rayIndices = nullptr;

	const UINT FRUSTUM_INDICES = 24;
	ID3D11Buffer* frustumIndices = nullptr;

	//BUFFER
	ID3D11Buffer* matrixBuf = nullptr;

	//SHADER PATHS
#ifdef _DEBUG
	const std::string vs_path = "../x64/Debug/ColliderVertexShader.cso";
	const std::string forward_ps_path = "../x64/Debug/ColliderPixelShader.cso";
	const std::string deferred_ps_path = "../x64/Debug/DeferredColliderPixelShader.cso";
#else
	const std::string vs_path = "../x64/Release/ColliderVertexShader.cso";
	const std::string forward_ps_path = "../x64/Release/ColliderPixelShader.cso";
	const std::string deferred_ps_path = "../x64/Release/DeferredColliderPixelShader.cso";
#endif

	//SHADERS
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;

	//INPUT LAYOUT
	ID3D11InputLayout* inputLayout = nullptr;
public:
	ColliderRenderer(RenderMethod method);
	~ColliderRenderer();

	virtual void Render() override;
};