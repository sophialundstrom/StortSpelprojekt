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
	ID3D11Buffer* translationBuf = nullptr;
	ID3D11Buffer* thetaBuf = nullptr;

	//VARIABLES
	struct textureTranslation
	{
		float offset;
		float padding1;
		float padding2;
		float padding3;
	}translationOffset;

	struct theta
	{
		float offset;
		float padding1;
		float padding2;
		float padding;
	}thetaOffset;

	//SHADER PATHS
#ifdef _DEBUG
	const std::string vs_path = "../x64/Debug/WaterVertexShader.cso";

	const std::string hs_path = "../x64/Debug/WaterHullShader.cso";

	const std::string ds_path = "../x64/Debug/WaterDomainShader.cso";

	const std::string gs_path = "../x64/Debug/WaterGeometryShader.cso";

	const std::string ps_path = "../x64/Debug/WaterPixelShader.cso";
#else
	const std::string vs_path = "../x64/Release/WaterVertexShader.cso";

	const std::string hs_path = "../x64/Release/WaterHullShader.cso";

	const std::string ds_path = "../x64/Release/WaterDomainShader.cso";

	const std::string gs_path = "../x64/Release/WaterGeometryShader.cso";

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
	WaterRenderer(float tesselationAmount = 1000);
	~WaterRenderer();

	virtual void Render() override;
	void Render(const Water& water);
};