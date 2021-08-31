#pragma once
#include "ModelRenderer.h"
#include "DeferredRenderer.h"
#include "ShadowRenderer.h"
#include "DisplacementRenderer.h"
#include "ParticleRenderer.h"

#include <map>

class RenderGraph
{
private:
	static std::map<RendererType, std::unique_ptr<Renderer>> renderers;
public:
	static void Initialize(UINT width, UINT height)
	{
		//DISPLACEMENT
		renderers.insert(std::make_pair(RendererType::DISPLACEMENT, std::make_unique<DisplacementRenderer>()));

		//MODEL
		renderers.insert(std::make_pair(RendererType::MODEL, std::make_unique<ModelRenderer>()));

		//PARTICLE
		renderers.insert(std::make_pair(RendererType::PARTICLE, std::make_unique<ParticleRenderer>()));

		//SHADOW
		renderers.insert(std::make_pair(RendererType::SHADOW, std::make_unique<ShadowRenderer>()));

		//DEFERRED
		renderers.insert(std::make_pair(RendererType::DEFERRED, std::make_unique<DeferredRenderer>(width, height)));

		static_cast<DeferredRenderer*>(renderers[RendererType::DEFERRED].get())->SetRenderTargets();
	}

	static void Bind(std::shared_ptr<Drawable> drawable, RendererType renderType)
	{
		if (renderers.find(renderType) != renderers.end())
			renderers[renderType]->Bind(drawable);
		else
			Print("TRIED TO BIND DRAWABLE TO NON EXISTING RENDERER");
	}

	static void Render()
	{
		for (auto& [type, renderer] : renderers)
			renderer->Render();
	}

	static void ShutDown() { renderers.clear(); }
};

inline std::map<RendererType, std::unique_ptr<Renderer>> RenderGraph::renderers;