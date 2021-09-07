#pragma once

#include "Graphics.h"
#include <fstream>

inline void CreateBuffer(ID3D11Buffer*& buffer, const UINT& size = 16)
{
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	bufferDesc.ByteWidth = size;

	HRESULT hr = Graphics::Inst().GetDevice().CreateBuffer(&bufferDesc, nullptr, &buffer);
	if FAILED(hr)
		Print("FAILED TO CREATE BUFFER");
}

inline void CreateStructuredBuffer(ID3D11Buffer*& buffer, ID3D11ShaderResourceView*& srv, const UINT& stride, const UINT& maxSize)
{
	HRESULT hr;

	D3D11_BUFFER_DESC structuredBufferDesc = {};
	structuredBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	structuredBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	structuredBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	structuredBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	structuredBufferDesc.StructureByteStride = stride;
	structuredBufferDesc.ByteWidth = maxSize;

	hr = Graphics::Inst().GetDevice().CreateBuffer(&structuredBufferDesc, nullptr, &buffer);
	if FAILED(hr)
	{
		Print("FAILED TO CREATE STRUCTURED BUFFER");
		return;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = maxSize / stride;

	hr = Graphics::Inst().GetDevice().CreateShaderResourceView(buffer, &srvDesc, &srv);
	if FAILED(hr)
	{
		Print("FAILED TO CREATE STRUCTURED BUFFER SRV");
		return;
	}
}

inline void UpdateBuffer(ID3D11Buffer*& buffer, const void* data, const UINT& size = 4)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource = {};

	HRESULT hr = Graphics::Inst().GetContext().Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if FAILED(hr)
	{
		Print("FAILED TO UPDATE BUFFER");
		return;
	}

	memcpy(mappedResource.pData, data, size);
	Graphics::Inst().GetContext().Unmap(buffer, 0);
}

template<typename T>
inline void UpdateBuffer(ID3D11Buffer*& buffer, const T& data)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource = {};

	HRESULT hr = Graphics::Inst().GetContext().Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if FAILED(hr)
	{
		Print("FAILED TO UPDATE BUFFER");
		return;
	}

	memcpy(mappedResource.pData, &data, sizeof(data));
	Graphics::Inst().GetContext().Unmap(buffer, 0);
}

inline void CreateIndexBuffer(ID3D11Buffer*& buffer, const UINT& numIndices, const void* data)
{
	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(UINT) * numIndices;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA resourceData = {};
	resourceData.pSysMem = data;
	resourceData.SysMemPitch = 0;
	resourceData.SysMemSlicePitch = 0;

	HRESULT hr = Graphics::Inst().GetDevice().CreateBuffer(&desc, &resourceData, &buffer);
	if FAILED(hr)
	{
		Print("FAILED TO CREATE INDEX BUFFER");
		return;
	}
}

inline void CreateVertexBuffer(ID3D11Buffer*& buffer, const int& stride, const int& size, const void* data)
{
	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = size;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = stride;

	D3D11_SUBRESOURCE_DATA resourceData = {};
	resourceData.pSysMem = data;

	if FAILED(Graphics::Inst().GetDevice().CreateBuffer(&desc, &resourceData, &buffer))
		Print("FAILED TO CREATE VERTEX BUFFER");
}

inline void CreateDynamicVertexBuffer(ID3D11Buffer*& buffer, const int& stride, const int& size)
{
	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = size;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	HRESULT hr = Graphics::Inst().GetDevice().CreateBuffer(&desc, nullptr, &buffer);
	if FAILED(hr)
		Print("FAILED TO CREATE DYNAMIC VERTEX BUFFER");
}

inline void UpdateDynamicVertexBuffer(ID3D11Buffer*& buffer, const int& size, const void* data)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource = {};

	HRESULT hr = Graphics::Inst().GetContext().Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if FAILED(hr)
	{
		Print("FAILED TO UPDATE DYNAMIC VERTEX BUFFER");
		return;
	}

	memcpy(mappedResource.pData, data, size);
	Graphics::Inst().GetContext().Unmap(buffer, 0);
}

inline bool LoadShader(ID3D11VertexShader*& vertexShader, std::string path, std::string& vertexShaderByteCode)
{
	std::string shaderData;
	std::ifstream reader;

	reader.open(path, std::ios::binary | std::ios::beg);

	if (!reader.is_open())
	{
		Print("COULD NOT OPEN FILE: " + path);
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);
	shaderData.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

	HRESULT hr = Graphics::Inst().GetDevice().CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &vertexShader);
	if FAILED(hr)
	{
		Print("FAILED TO CREATE VERTEX SHADER");
		return false;
	}

	vertexShaderByteCode = shaderData;
	shaderData.clear();
	reader.close();
	return true;
}

inline bool LoadShader(ID3D11VertexShader*& vertexShader, std::string path)
{
	std::string shaderData;
	std::ifstream reader;

	reader.open(path, std::ios::binary | std::ios::beg);

	if (!reader.is_open())
	{
		Print("COULD NOT OPEN FILE: " + path);
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);
	shaderData.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

	HRESULT hr = Graphics::Inst().GetDevice().CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &vertexShader);
	if FAILED(hr)
	{
		Print("FAILED TO CREATE VERTEX SHADER");
		return false;
	}

	shaderData.clear();
	reader.close();
	return true;
}

inline bool LoadShader(ID3D11PixelShader*& pixelShader, std::string path)
{
	std::string shaderData;
	std::ifstream reader;

	reader.open(path, std::ios::binary | std::ios::beg);

	if (!reader.is_open())
	{
		Print("COULD NOT OPEN FILE: " + path);
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);
	shaderData.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

	HRESULT hr = Graphics::Inst().GetDevice().CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &pixelShader);
	if FAILED(hr)
	{
		Print("FAILED TO CREATE PIXEL SHADER");
		return false;
	}

	shaderData.clear();
	reader.close();
	return true;
}

inline bool LoadShader(ID3D11HullShader*& hullShader, std::string path)
{
	std::string shaderData;
	std::ifstream reader;

	reader.open(path, std::ios::binary | std::ios::beg);

	if (!reader.is_open())
	{
		Print("COULD NOT OPEN FILE: " + path);
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);
	shaderData.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

	HRESULT hr = Graphics::Inst().GetDevice().CreateHullShader(shaderData.c_str(), shaderData.length(), nullptr, &hullShader);
	if FAILED(hr)
	{
		Print("FAILED TO CREATE HULL SHADER");
		return false;
	}

	shaderData.clear();
	reader.close();
	return true;
}

inline bool LoadShader(ID3D11DomainShader*& domainShader, std::string path)
{
	std::string shaderData;
	std::ifstream reader;

	reader.open(path, std::ios::binary | std::ios::beg);

	if (!reader.is_open())
	{
		Print("COULD NOT OPEN FILE: " + path);
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);
	shaderData.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

	HRESULT hr = Graphics::Inst().GetDevice().CreateDomainShader(shaderData.c_str(), shaderData.length(), nullptr, &domainShader);
	if FAILED(hr)
	{
		Print("FAILED TO CREATE DOMAIN SHADER");
		return false;
	}

	shaderData.clear();
	reader.close();
	return true;
}

inline bool LoadShader(ID3D11GeometryShader*& geometryShader, std::string path)
{
	std::string shaderData;
	std::ifstream reader;

	reader.open(path, std::ios::binary | std::ios::beg);

	if (!reader.is_open())
	{
		Print("COULD NOT OPEN FILE: " + path);
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);
	shaderData.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

	HRESULT hr = Graphics::Inst().GetDevice().CreateGeometryShader(shaderData.c_str(), shaderData.length(), nullptr, &geometryShader);
	if FAILED(hr)
	{
		Print("FAILED TO CREATE GEOMETRY SHADER");
		return false;
	}

	shaderData.clear();
	reader.close();
	return true;
}

inline void BindShaders(ID3D11VertexShader* vertexShader = nullptr, ID3D11HullShader* hullShader = nullptr, ID3D11DomainShader* domainShader = nullptr, ID3D11GeometryShader* geometryShader = nullptr, ID3D11PixelShader* pixelShader = nullptr)
{
	Graphics::Inst().GetContext().VSSetShader(vertexShader, nullptr, 0);
	Graphics::Inst().GetContext().HSSetShader(hullShader, nullptr, 0);
	Graphics::Inst().GetContext().DSSetShader(domainShader, nullptr, 0);
	Graphics::Inst().GetContext().GSSetShader(geometryShader, nullptr, 0);
	Graphics::Inst().GetContext().PSSetShader(pixelShader, nullptr, 0);
}

enum class Shader { VS, HS, DS, GS, PS };
inline void BindBuffer(ID3D11Buffer* buffer, Shader shader = Shader::VS, UINT slot = 0)
{
	switch (shader)
	{
	case Shader::VS:
		Graphics::Inst().GetContext().VSSetConstantBuffers(slot, 1, &buffer);
		break;

	case Shader::HS:
		Graphics::Inst().GetContext().HSSetConstantBuffers(slot, 1, &buffer);
		break;

	case Shader::DS:
		Graphics::Inst().GetContext().DSSetConstantBuffers(slot, 1, &buffer);
		break;

	case Shader::GS:
		Graphics::Inst().GetContext().GSSetConstantBuffers(slot, 1, &buffer);
		break;

	case Shader::PS:
		Graphics::Inst().GetContext().PSSetConstantBuffers(slot, 1, &buffer);
		break;
	}
}