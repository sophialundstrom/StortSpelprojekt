#pragma once
#include "Camera.h"
#include "Light.h"
#include "ShadowMap.h"
#include "Graphics.h"

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
	friend class VolumeRenderer;
	friend class SkeletonRenderer;
	friend class WaterRenderer;
	friend class SkyBoxRenderer;
	friend class InteractableRenderer;
	friend class PerformanceRenderer;
private:
	//CAMERA
	Matrix cameraMatrix;
	Matrix viewMatrix;
	Matrix projectionMatrix;
	Vector3 cameraPosition;
	ID3D11Buffer* cameraPositionBuf;

	//DIRECTIONAL LIGHT
	Matrix lightMatrix;
	DirectionalLight::Data lightData;
	ID3D11Buffer* lightDataBuf;

	//POINT LIGHTS
	UINT numPointLights = 0;
	PointLight* pointLightsData = nullptr;
	
	ID3D11ShaderResourceView* pointLightSRV = nullptr;
	ID3D11Buffer* pointLightBuffer = nullptr;
	ID3D11Buffer* numPointLightsBuffer = nullptr;
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
		CreateBuffer(lightDataBuf, sizeof(DirectionalLight::Data));
		CreateBuffer(numPointLightsBuffer);
		CreateStructuredBuffer(pointLightBuffer, pointLightSRV, sizeof(PointLight), sizeof(PointLight) * MAX_LIGHTS);
		shadowMap = ShadowMap(10000, 10);

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
		pointLightBuffer->Release();
		numPointLightsBuffer->Release();
		pointLightSRV->Release();
		matrices_buf->Release();
		if (inputLayout)
			inputLayout->Release();
		lightDataBuf->Release();
		wrapSampler->Release();
		shadowMap.ShutDown();
	}

	void Update(const Camera& camera, const DirectionalLight& directionalLight, const UINT& numPointLights, PointLight* pointLightsData)
	{
		//CAMERA
		cameraMatrix = camera.GetMatrix();
		viewMatrix = camera.GetViewMatrix();
		projectionMatrix = camera.GetProjectionMatrix();

		cameraPosition = camera.GetPosition();

		matrices.viewPerspective = cameraMatrix;
		
		UpdateBuffer(cameraPositionBuf, cameraPosition);

		//DIRECTIONAL LIGHT
		lightMatrix = directionalLight.GetMatrix();
		lightData = directionalLight.data;
		UpdateBuffer(lightDataBuf, lightData);
		//hej
		//POINT LIGHTS
		ShaderData::pointLightsData = pointLightsData;
		ShaderData::numPointLights = numPointLights;
		UpdateBuffer(numPointLightsBuffer, numPointLights);
		UpdateBuffer(pointLightBuffer, pointLightsData, sizeof(PointLight) * numPointLights);
	}

	void Update(const Camera& camera)
	{
		cameraMatrix = camera.GetMatrix();

		cameraPosition = camera.GetPosition();

		UpdateBuffer(cameraPositionBuf, cameraPosition);
	}

	void BindFrameConstants()
	{
		Graphics::Inst().GetContext().PSSetConstantBuffers(1, 1, &lightDataBuf);
		Graphics::Inst().GetContext().PSSetConstantBuffers(2, 1, &cameraPositionBuf);
		BindBuffer(numPointLightsBuffer, Shader::PS, 3);
		Graphics::Inst().GetContext().PSSetShaderResources(9, 1, &pointLightSRV);
		shadowMap.BindAsResource();
	}
};