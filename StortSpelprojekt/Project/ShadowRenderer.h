#pragma once
#include "Renderer.h"

class ShadowRenderer : public Renderer
{
private:
	//BUFFER
	ID3D11Buffer* matrixBuf;

	//SHADERS
#ifdef _DEBUG
	const std::string vs_path = "../x64/Debug/ShadowVertexShader.cso";
#else
	const std::string vs_path = "../x64/Release/ShadowVertexShader.cso";
#endif
	ID3D11VertexShader* vertexShader;

	//INPUT LAYOUT
	ID3D11InputLayout* inputLayout = nullptr;
public:
	ShadowRenderer();
	~ShadowRenderer();

	virtual void Render() override;
};