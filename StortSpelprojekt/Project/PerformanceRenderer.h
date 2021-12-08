#pragma once
#include "Renderer.h"

class PerformanceRenderer : public Renderer
{
private:
	const UINT stride = sizeof(Vector3);
	const UINT offset = 0;

	//BUFFER
	ID3D11Buffer* matricesBuf = nullptr;
	struct Matrices
	{
		Matrix world;
		Matrix viewPerspective;
	} matrices;
	ID3D11Buffer* idBuffer = nullptr;

	//SHADER PATHS
	const std::string vs_path = "../x64/Debug/PerformanceVertexShader.cso";
	const std::string ps_path = "../x64/Debug/PerformancePixelShader.cso";

	//SHADERS
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;

	//INPUT LAYOUT
	ID3D11InputLayout* inputLayout = nullptr;

public:
	PerformanceRenderer();
	~PerformanceRenderer();

	virtual void Render() override;
};