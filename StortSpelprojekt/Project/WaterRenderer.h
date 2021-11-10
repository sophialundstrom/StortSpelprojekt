#pragma once
#include "Renderer.h"
#include "Water.h"

class WaterRenderer : public Renderer
{
private:
	//BUFFERS
	ID3D11Buffer* matrixBuf = nullptr;
	ID3D11Buffer* lightBuf = nullptr;
	ID3D11Buffer* tesselationBuf = nullptr;
	ID3D11Buffer* timeBuf = nullptr;

	//SHADER PATHS
#ifdef _DEBUG
	const std::string vs_path = "../x64/Debug/TerrainVertexShader.cso";

	const std::string hs_path = "../x64/Debug/TerrainHullShader.cso";

	const std::string ds_path = "../x64/Debug/WaterDomainShader.cso";

	const std::string gs_path = "../x64/Debug/TerrainGeometryShader.cso";

	const std::string ps_path = "../x64/Debug/WaterPixelShader.cso";
#else
	const std::string vs_path = "../x64/Release/TerrainVertexShader.cso";

	const std::string hs_path = "../x64/Release/TerrainHullShader.cso";

	const std::string ds_path = "../x64/Release/WaterDomainShader.cso";

	const std::string gs_path = "../x64/Release/TerrainGeometryShader.cso";

	const std::string ps_path = "../x64/Release/WaterPixelShader.cso";
#endif

	//SHADERS
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11HullShader* hullShader = nullptr;
	ID3D11DomainShader* domainShader = nullptr;
	ID3D11GeometryShader* geometryShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;

	//INPUT LAYOUT
	ID3D11InputLayout* inputLayout = nullptr;
public:
	WaterRenderer(float tesselationAmount = 50);
	~WaterRenderer();

	virtual void Render() override;
	void Render(const Water& water);
};