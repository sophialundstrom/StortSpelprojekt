#pragma once
#include "Camera.h"
#include "Light.h"
#include "ShadowMap.h"

class ShaderData
{
	friend class ModelRenderer;
	friend class ShadowRenderer;
	friend class DisplacementRenderer;
	friend class ParticleRenderer;
	friend class DeferredRenderer;
private:
	//CAMERA
	static Matrix cameraMatrix;
	static Vector3 cameraPosition;

	//DIRECTIONAL LIGHT
	static Matrix lightMatrix;
	static DirectionalLight::Data lightData;

	//POINT LIGHTS
	static UINT numPointLights;
	static PointLight* pointLightsData;

	//INPUT LAYOUT
	static ID3D11InputLayout* inputLayout;

	//SAMPLER
	static ID3D11SamplerState* wrapSampler;

	//MATRICES & MATRICES-BUFFER
	static ID3D11Buffer* matrices_buf;
	struct TempMatrices
	{
		Matrix world;
		Matrix viewPerspective;
	} matrices;

	//SHADOW MAP
	static ShadowMap shadowMap;
public:
	static void Initialize(std::string layoutByteCode)
	{
		shadowMap = ShadowMap(4096);

		//INPUT LAYOUT
		D3D11_INPUT_ELEMENT_DESC inputDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXTURECOORDS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		HRESULT hr = Graphics::GetDevice().CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), layoutByteCode.c_str(), layoutByteCode.length(), &inputLayout);
		if FAILED(hr)
		{
			Print("FAILED TO CREATE INPUT LAYOUT");
			return;
		}

		//SAMPLER
		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		hr = Graphics::GetDevice().CreateSamplerState(&samplerDesc, &wrapSampler);
		if FAILED(hr)
		{
			Print("FAILED TO CREATE WRAP SAMPLER");
			return;
		}

		Graphics::GetContext().PSSetSamplers(0, 1, &wrapSampler);
		Graphics::GetContext().DSSetSamplers(0, 1, &wrapSampler);
	}

	static void Update(const Camera& camera, const DirectionalLight& directionalLight, const UINT& numPointLights, PointLight* pointLightsData)
	{
		//CAMERA
		cameraMatrix = camera.GetMatrix();
		cameraPosition = camera.GetPosition();

		//DIRECTIONAL LIGHT
		lightMatrix = directionalLight.GetMatrix();
		lightData = directionalLight.data;

		//POINT LIGHTS
		ShaderData::pointLightsData = pointLightsData;
		ShaderData::numPointLights = numPointLights;
	}

	static void ShutDown()
	{
		inputLayout->Release();
		wrapSampler->Release();
		shadowMap.ShutDown();
	}
};

inline Matrix ShaderData::cameraMatrix;
inline Vector3 ShaderData::cameraPosition;
inline Matrix ShaderData::lightMatrix;
inline UINT ShaderData::numPointLights;
inline PointLight* ShaderData::pointLightsData;
inline DirectionalLight::Data ShaderData::lightData;
inline ID3D11InputLayout* ShaderData::inputLayout;
inline ID3D11SamplerState* ShaderData::wrapSampler;
inline ShadowMap ShaderData::shadowMap;