#pragma once
#include "Renderer.h"
#include <DirectXTK/DDSTextureLoader.h>
#include <string>
#include "Camera.h"

class SkyBoxRenderer : public Renderer
{
private:
	const UINT stride = sizeof(Vector3);
	const UINT offset = 0;
	void BuildCubeMap(std::string skyboxFolderName);

public:
	SkyBoxRenderer();
	virtual void Render() override;

	//D3D11Resources
	ID3D11Texture2D* pTexture;
	ID3D11ShaderResourceView* pTextureView;

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
	const std::string skyboxTexturePath = "Skybox/";

	//Shaders
	ID3D11VertexShader* skyBoxVertexShader = nullptr;
	ID3D11PixelShader* skyBoxPixelShader = nullptr;

	//Input layout
	ID3D11InputLayout* inputLayout = nullptr;

	

};