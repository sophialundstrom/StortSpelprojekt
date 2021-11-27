#pragma once
#include "Renderer.h"
#include <DirectXTK/DDSTextureLoader.h>
#include <string>
#include "Camera.h"

class SkyBoxRenderer : public Renderer
{
public:
	SkyBoxRenderer();
	virtual void Render() override;

private:
	const UINT stride = sizeof(Vector3);
	const UINT offset = 0;

	ID3D11Resource** texture = nullptr;
	ID3D11ShaderResourceView** textureView = nullptr;
	//Vbuffer
	ID3D11Buffer* skyboxMesh = nullptr;
	//Ibuffer
	ID3D11Buffer* skyBoxIndices = nullptr;
	//Cbuffer
	ID3D11Buffer* matricesBuf = nullptr;

	ID3D11DepthStencilState* skyboxDepthStencil = nullptr;


#ifdef _DEBUG
	const std::string vs_path = "../x64/Debug/skyBoxVertexShader.cso";
	const std::string ps_path = "../x64/Debug/skyBoxPixelShader.cso";
#else
	const std::string vs_path = "../x64/Release/skyBoxVertexShader.cso";
	const std::string ps_path = "../x64/Debug/skyBoxPixelShader.cso";
#endif
	//Shaders
	ID3D11VertexShader* skyBoxVertexShader = nullptr;
	ID3D11PixelShader* skyBoxPixelShader = nullptr;

	//Input layout
	ID3D11InputLayout* inputLayout = nullptr;

	

};