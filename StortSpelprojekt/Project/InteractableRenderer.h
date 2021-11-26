#pragma once
#include "Renderer.h"
#include "Texture.h"

class InteractableRenderer : public Renderer
{
private:
	const UINT stride = sizeof(Vector3);
	const UINT offset = 0;

	const UINT NUM_PLANES = 3;

	//INDEX BUFFER
	const UINT NUM_INDICES = 6;
	ID3D11Buffer* indexBuffer = nullptr;

	//VERTEX BUFFER
	ID3D11Buffer* vertexBuffer = nullptr;

	//COLORS
	const Vector3 RED = { 1.0f, 0.0f, 0.0f };
	const Vector3 BLUE = { 0.0f, 0.0f, 1.0f };

	//BUFFERS
	ID3D11Buffer* colorBuf = nullptr;
	ID3D11Buffer* timeBuf = nullptr;
	ID3D11Buffer* IDBuf = nullptr;

	ID3D11Buffer* matricesBuf = nullptr;
	struct Matrices
	{
		Matrix world;
		Matrix viewPerspective;
	} matrices;

	//SHADER PATHS
#ifdef _DEBUG
	const std::string vs_path = "../x64/Debug/InteractableVertexShader.cso";
	const std::string ps_path = "../x64/Debug/InteractablePixelShader.cso";
#else
	const std::string vs_path = "../x64/Release/InteractableVertexShader.cso";
	const std::string ps_path = "../x64/Release/InteractablePixelShader.cso";
#endif

	//SHADERS
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;

	//INPUT LAYOUT
	ID3D11InputLayout* inputLayout = nullptr;

	//RASTERIZER STATE
	ID3D11RasterizerState* noCullState = nullptr;

	//MASKS
	Texture* circleMask;
public:
	InteractableRenderer();
	~InteractableRenderer();

	virtual void Render() override;
};