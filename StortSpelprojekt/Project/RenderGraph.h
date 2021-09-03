#pragma once
#include "Singleton.h"
#include "ModelRenderer.h"
#include "DeferredRenderer.h"
#include "ShadowRenderer.h"
#include "DisplacementRenderer.h"
#include "ParticleRenderer.h"

#include <map>

class RenderGraph : public Singleton<RenderGraph>
{
private:
	bool firstFrame = true;
	std::map<RendererType, std::unique_ptr<Renderer>> renderers;
public:
	RenderGraph(std::unique_ptr<ShaderData>& shaderData, UINT width, UINT height)
		:Singleton(this)
	{
		//MODEL
		renderers.insert(std::make_pair(RendererType::MODEL, std::make_unique<ModelRenderer>(shaderData)));

		//DISPLACEMENT
		renderers.insert(std::make_pair(RendererType::DISPLACEMENT, std::make_unique<DisplacementRenderer>()));

		//PARTICLE
		renderers.insert(std::make_pair(RendererType::PARTICLE, std::make_unique<ParticleRenderer>()));

		//SHADOW
		renderers.insert(std::make_pair(RendererType::SHADOW, std::make_unique<ShadowRenderer>()));

		//DEFERRED
		renderers.insert(std::make_pair(RendererType::DEFERRED, std::make_unique<DeferredRenderer>(width, height)));
	}

	void Bind(std::shared_ptr<Drawable> drawable, RendererType renderType)
	{
		if (renderers.find(renderType) != renderers.end())
			renderers[renderType]->Bind(drawable);
		else
			Print("TRIED TO BIND DRAWABLE TO NON EXISTING RENDERER");
	}

	void Render()
	{
		if (firstFrame)
		{
			static_cast<DeferredRenderer*>(renderers[RendererType::DEFERRED].get())->SetRenderTargets();
			firstFrame = false;
		}

		for (auto& [type, renderer] : renderers)
			renderer->Render();
	}

	void Clear()
	{
		for (auto& [type, renderer] : renderers)
			renderer->Clear();
	}
};