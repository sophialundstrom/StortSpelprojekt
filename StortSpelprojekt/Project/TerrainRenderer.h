#pragma once
#include "Renderer.h"
#include "Terrain.h"

class TerrainRenderer
{
private:

	struct Matrices
	{
		Matrix viewPerspective;
		Matrix lightViewPerspective;
	}matrices; 
	//BUFFERS
	ID3D11Buffer* matrixBuf = nullptr;
	ID3D11Buffer* lightBuf = nullptr;

	ID3D11Buffer* tesselationBuf = nullptr;

	//SHADER PATHS
#ifdef _DEBUG
	const std::string vs_path = "../x64/Debug/TerrainVertexShader.cso";

	const std::string hs_path = "../x64/Debug/TerrainHullShader.cso";
	const std::string LOD_hs_path = "../x64/Debug/LODTerrainHullShader.cso";

	const std::string ds_path = "../x64/Debug/TerrainDomainShader.cso";
	const std::string LOD_ds_path = "../x64/Debug/LODTerrainDomainShader.cso";

	const std::string gs_path = "../x64/Debug/TerrainGeometryShader.cso";

	const std::string deferred_ps_path = "../x64/Debug/DeferredTerrainPixelShader.cso";
	const std::string forward_ps_path = "../x64/Debug/ForwardTerrainPixelShader.cso";
#else
	const std::string vs_path = "../x64/Release/TerrainVertexShader.cso";

	const std::string hs_path = "../x64/Release/TerrainHullShader.cso";
	const std::string LOD_hs_path = "../x64/Release/LODTerrainHullShader.cso";

	const std::string ds_path = "../x64/Release/TerrainDomainShader.cso";

	const std::string gs_path = "../x64/Release/TerrainGeometryShader.cso";

	const std::string deferred_ps_path = "../x64/Release/DeferredTerrainPixelShader.cso";
	const std::string forward_ps_path = "../x64/Release/ForwardTerrainPixelShader.cso";
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
	TerrainRenderer(RenderMethod method, float tesselationAmount = 63);
	~TerrainRenderer();

	void Render(const Terrain& terrain);
};