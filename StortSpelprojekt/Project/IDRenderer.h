#pragma once
#include "Renderer.h"

class IDRenderer : public Renderer
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
	const std::string vs_path = "../x64/Debug/IDVertexShader.cso";
	const std::string volume_vs_path = "../x64/Debug/VolumeIDVertexShader.cso";
	const std::string ps_path = "../x64/Debug/IDPixelShader.cso";

	//SHADERS
	ID3D11VertexShader* volumeVertexShader = nullptr;
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;

	//INPUT LAYOUT
	ID3D11InputLayout* inputLayout = nullptr;
	ID3D11InputLayout* volumeInputLayout = nullptr;
	ID3D11InputLayout* currentLayout = nullptr;

	//RENDER TARGET
	ID3D11RenderTargetView* idRTV;
	ID3D11Texture2D* idTexture;
	ID3D11Texture2D* idTextureData;

	//BOUNDING VOLUMES
	ID3D11Buffer* sphereIndices = nullptr;
	ID3D11Buffer* sphereVertexBuffer = nullptr;

	ID3D11Buffer* boxIndices = nullptr;
	ID3D11Buffer* boxVertexBuffer = nullptr;
public:
	IDRenderer();
	~IDRenderer();

	virtual void Render() override;

	void UpdateViewport(UINT width, UINT height);
	void BeginFrame(ID3D11DepthStencilView* dsv, D3D11_VIEWPORT& viewport);
	int GetObjectID(int xPix, int yPix);
};