#pragma once
#include "Drawable.h"
#include "ShaderData.h"
#include "DirectXHelp.h"
#include "Model.h"
#include "ParticleSystem.h"
#include <memory>
#include <map>

class Forward;
class Deferred;

enum class RendererType { UNDEFINED = -1, TERRAIN, MODEL, PARTICLE, SHADOW };

class Renderer
{
protected:
	RendererType type = RendererType::UNDEFINED;
	std::vector<std::shared_ptr<Drawable>> drawables;
public:
	virtual ~Renderer() = default;

	template <typename T>
	void Bind(const std::map<std::string, std::shared_ptr<T>>& drawables) 
	{ 
		if ((std::is_same<T, ParticleSystem> && type == RendererType::PARTICLE) ||
			(std::is_same<T, Model> && (type == RendererType::MODEL || type == RendererType::SHADOW)))
			for (auto& [name, drawable] : drawables) this->drawables.emplace_back(drawable); 
	}
	
	void Bind(std::shared_ptr<Drawable> drawable) { drawables.push_back(drawable); }
	
	RendererType Type() { return type; }
	void Clear() { drawables.clear(); }
	virtual void Render() = 0;
};	