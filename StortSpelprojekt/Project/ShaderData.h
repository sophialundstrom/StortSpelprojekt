#pragma once
#include "Renderer.h"
#include "Camera.h"
#include "Light.h"
#include "ShadowMap.h"
#include "Singleton.h"

class ShaderData : public Singleton<ShaderData>
{
	friend class ShadowRenderer;
	friend class DeferredRenderer;

	friend class ColliderRenderer;
	friend class AnimatedModelRenderer;
    friend class ModelRenderer;
	friend class TerrainRenderer;
    friend class ParticleRenderer;
	friend class IDRenderer;
private:
	//CAMERA
	Matrix cameraMatrix;
	Vector3 cameraPosition;
	ID3D11Buffer* cameraPositionBuf;

	//DIRECTIONAL LIGHT
	Matrix lightMatrix;
	DirectionalLight::Data lightData;

	//POINT LIGHTS
	UINT numPointLights = 0;
	PointLight::Data* pointLightsData = nullptr;

	//INPUT LAYOUT
	ID3D11InputLayout* inputLayout = nullptr;

	//SAMPLER
	ID3D11SamplerState* wrapSampler = nullptr;

	//MATRICES & MATRICES-BUFFER
	ID3D11Buffer* matrices_buf;
	struct Matrices
	{
		Matrix world;
		Matrix viewPerspective;
	} matrices;

	//SHADOW MAP
	ShadowMap shadowMap;
public:
	ShaderData()
		:Singleton(this)
	{
		CreateBuffer(cameraPositionBuf);
		CreateBuffer(matrices_buf, sizeof(Matrices));

		shadowMap = ShadowMap(4096, 7);

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

		HRESULT hr = Graphics::Inst().GetDevice().CreateSamplerState(&samplerDesc, &wrapSampler);
		if FAILED(hr)
		{
			Print("FAILED TO CREATE WRAP SAMPLER", "SHADER DATA");
			return;
		}
		Print("SUCCEEDED TO CREATE WRAP SAMPLER", "SHADER DATA");

		Graphics::Inst().GetContext().PSSetSamplers(0, 1, &wrapSampler);
		Graphics::Inst().GetContext().DSSetSamplers(0, 1, &wrapSampler);

		Print("SUCCEEDED TO INITIALIZE SHADER DATA");
		Print("=======================================");
	}

	~ShaderData()
	{
		cameraPositionBuf->Release();
		matrices_buf->Release();
		wrapSampler->Release();
		shadowMap.ShutDown();
	}

	void Update(const Camera& camera, const DirectionalLight& directionalLight, const UINT& numPointLights, PointLight::Data* pointLightsData)
	{
		//CAMERA
		cameraMatrix = camera.GetMatrix();
		cameraPosition = camera.GetPosition();

		matrices.viewPerspective = cameraMatrix;
		
		UpdateBuffer(cameraPositionBuf, cameraPosition);

		//DIRECTIONAL LIGHT
		lightMatrix = directionalLight.GetMatrix();
		lightData = directionalLight.data;

		//POINT LIGHTS
		ShaderData::pointLightsData = pointLightsData;
		ShaderData::numPointLights = numPointLights;
	}

	void Update(const Camera& camera)
	{
		cameraMatrix = camera.GetMatrix();

		cameraPosition = camera.GetPosition();

		UpdateBuffer(cameraPositionBuf, cameraPosition);
	}
};