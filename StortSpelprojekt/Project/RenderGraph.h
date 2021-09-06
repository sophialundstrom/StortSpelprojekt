#pragma once

#include "ModelRenderer.h"
#include "DeferredRenderer.h"
#include "ShadowRenderer.h"
#include "ParticleRenderer.h"

#include <map>

class RenderGraph : public Singleton<RenderGraph>
{
private:
	bool firstFrame = true;
	std::map<RendererType, std::unique_ptr<Renderer>> renderers;
	DeferredRenderer deferredRenderer;
public:
	RenderGraph(std::unique_ptr<ShaderData>& shaderData, UINT width, UINT height)
		:Singleton(this), deferredRenderer(width, height)
	{
		//----INSERT TERRAIN RENDERER----

		//MODEL
		//renderers.insert(std::make_pair(RendererType::MODEL, std::make_unique<ModelRenderer>(shaderData)));

		//PARTICLE
		//renderers.insert(std::make_pair(RendererType::PARTICLE, std::make_unique<ParticleRenderer>()));

		//SHADOW
		renderers.insert(std::make_pair(RendererType::SHADOW, std::make_unique<ShadowRenderer>()));
	}

	void Bind(std::shared_ptr<Drawable> drawable, RendererType renderType) { renderers[renderType]->Bind(drawable); }

	void Render()
	{
		if (firstFrame)
		{
			deferredRenderer.SetRenderTargets();
			firstFrame = false;
		}

		/*for (auto& [type, renderer] : renderers)
			renderer->Render();*/

		deferredRenderer.Render();
	}

	void Clear()
	{
		for (auto& [type, renderer] : renderers)
			renderer->Clear();
	}
};