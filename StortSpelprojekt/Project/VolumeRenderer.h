#pragma once
#include "Renderer.h"

class VolumeRenderer : public Renderer
{
private:
	const UINT stride = sizeof(Vector3);
	const UINT offset = 0;

	//INDEX BUFFERS
	ID3D11Buffer* sphereIndices = nullptr;
	ID3D11Buffer* sphereVertexBuffer = nullptr;

	ID3D11Buffer* boxIndices = nullptr;
	ID3D11Buffer* boxVertexBuffer = nullptr;

	//BUFFER
	ID3D11Buffer* matricesBuf = nullptr;

	struct Matrices
	{
		Matrix world;
		Matrix viewPerspective;
	}matrices;

	//SHADER PATHS
#ifdef _DEBUG
	const std::string vs_path = "../x64/Debug/VolumeVertexShader.cso";
	const std::string forward_ps_path = "../x64/Debug/VolumePixelShader.cso";
#else
	const std::string vs_path = "../x64/Release/VolumeVertexShader.cso";
	const std::string forward_ps_path = "../x64/Release/VolumePixelShader.cso";
#endif

	//SHADERS
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;

	//INPUT LAYOUT
	ID3D11InputLayout* inputLayout = nullptr;

	//BLEND STATES
	ID3D11BlendState* blendState = nullptr; 

public:
	VolumeRenderer();
	~VolumeRenderer();

	virtual void Render() override;
};