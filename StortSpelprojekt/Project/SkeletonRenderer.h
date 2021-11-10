#pragma once
#include "Renderer.h"

class SkeletonRenderer : public Renderer
{
private:
	//BUFFER
	ID3D11Buffer* matricesBuf = nullptr;
	struct Matrices
	{
		Matrix world;
		Matrix viewPerspective;
	} matrices;

	//VERTEX BUFFER
	const UINT stride = sizeof(Vector3);
	const UINT offset = 0;
	ID3D11Buffer* vertexBuffer;

	//SHADER PATHS
	const std::string vs_path = "../x64/Debug/SkeletonVertexShader.cso";
	const std::string ps_path = "../x64/Debug/SkeletonPixelShader.cso";

	//SHADERS
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;

	//INPUT LAYOUT
	ID3D11InputLayout* inputLayout = nullptr;

	//DEPTH STENCIL STATE
	ID3D11DepthStencilState* depthStencilState = nullptr;
public:
	SkeletonRenderer();
	~SkeletonRenderer();

	virtual void Render() override;
};