//#pragma once
//#include "Geometry.h"
//#include "Material.h"
//
//struct Bounds
//{
//	float Xmin = D3D11_FLOAT32_MAX;
//	float Ymin = D3D11_FLOAT32_MAX;
//	float Zmin = D3D11_FLOAT32_MAX;
//	float Xmax = -D3D11_FLOAT32_MAX;
//	float Ymax = -D3D11_FLOAT32_MAX;
//	float Zmax = -D3D11_FLOAT32_MAX;
//};
//
//struct Mesh
//{
//	const UINT stride = sizeof(Vertex);
//	const UINT offset = 0;
//
//	std::vector<Face> faces;
//
//	std::string mtllib = "";
//	int materialID = -1;
//
//	ID3D11Buffer* vertexBuffer = nullptr;
//
//	Mesh(std::string mtllib)
//		:mtllib(mtllib)
//	{}
//
//	Mesh(const Mesh& mesh)
//	{
//		this->faces = mesh.faces;
//		this->mtllib = mesh.mtllib;
//		this->materialID = mesh.materialID;
//		this->vertexBuffer = nullptr;
//	}
//
//	~Mesh() { vertexBuffer->Release(); }
//};