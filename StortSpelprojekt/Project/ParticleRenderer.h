#pragma once
#include "Renderer.h"

class ParticleRenderer : public Renderer
{
	//BUFFERS
	ID3D11Buffer* extentsBuf = nullptr;
	ID3D11Buffer* matrixBuf = nullptr;

	//SHADER PATHS
#ifdef _DEBUG
	const std::string vs_path = "../x64/Debug/ParticleVertexShader.cso";
	const std::string gs_path = "../x64/Debug/ParticleGeometryShader.cso";
	const std::string deferred_ps_path = "../x64/Debug/DeferredParticlePixelShader.cso";
	const std::string forward_ps_path = "../x64/Debug/ForwardParticlePixelShader.cso";
#else
	const std::string vs_path = "../x64/Release/ParticleVertexShader.cso";
	const std::string gs_path = "../x64/Release/ParticleGeometryShader.cso";
	const std::string deferred_ps_path = "../x64/Release/DeferredParticlePixelShader.cso";
	const std::string forward_ps_path = "../x64/Release/ForwardParticlePixelShader.cso";
#endif

	//SHADERS
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11GeometryShader* geometryShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;
	ID3D11Buffer* lifeTimeBuffer = nullptr;

	//INPUT LAYOUT
	ID3D11InputLayout* inputLayout = nullptr;
public:
	ParticleRenderer();
	~ParticleRenderer();

	virtual void Render() override;
};