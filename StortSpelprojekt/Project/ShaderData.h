#pragma once
#include "DirectXHelp.h"
#include "Camera.h"
#include "Light.h"
#include "ShadowMap.h"
#include "Singleton.h"

class ShaderData : public Singleton<ShaderData>
{
	//friend class ModelRenderer;
	friend class ShadowRenderer;
	friend class DisplacementRenderer;
	friend class ParticleRenderer;
	friend class DeferredRenderer;

	friend class ForwardParticleRenderer;
	friend class ForwardModelRenderer;
private:
	//CAMERA
	Matrix cameraMatrix;
	Vector3 cameraPosition;
	ID3D11Buffer* cameraPosition_buf;

	//DIRECTIONAL LIGHT
	Matrix lightMatrix;
	DirectionalLight::Data lightData;

	//POINT LIGHTS
	UINT numPointLights = 0;
	PointLight* pointLightsData = nullptr;

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
	ShaderData(std::string layoutByteCode)
		:Singleton(this)
	{
		CreateBuffer(cameraPosition_buf);
		CreateBuffer(matrices_buf, sizeof(Matrices));

		shadowMap = ShadowMap(4096);

		//INPUT LAYOUT
		D3D11_INPUT_ELEMENT_DESC inputDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXTURECOORDS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		HRESULT hr = Graphics::Inst().GetDevice().CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), layoutByteCode.c_str(), layoutByteCode.length(), &inputLayout);
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

		hr = Graphics::Inst().GetDevice().CreateSamplerState(&samplerDesc, &wrapSampler);
		if FAILED(hr)
		{
			Print("FAILED TO CREATE WRAP SAMPLER");
			return;
		}

		Graphics::Inst().GetContext().PSSetSamplers(0, 1, &wrapSampler);
		Graphics::Inst().GetContext().DSSetSamplers(0, 1, &wrapSampler);
	}

	~ShaderData()
	{
		cameraPosition_buf->Release();
		matrices_buf->Release();
		inputLayout->Release();
		wrapSampler->Release();
		shadowMap.ShutDown();
	}

	void Update(const Camera& camera, const DirectionalLight& directionalLight, const UINT& numPointLights, PointLight* pointLightsData)
	{
		//CAMERA
		cameraMatrix = camera.GetMatrix();
		cameraPosition = camera.GetPosition();

		matrices.viewPerspective = cameraMatrix;
		
		UpdateBuffer(cameraPosition_buf, cameraPosition);

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

		UpdateBuffer(cameraPosition_buf, cameraPosition);
	}
};