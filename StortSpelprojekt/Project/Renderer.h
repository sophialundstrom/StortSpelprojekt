#pragma once
#include "Drawable.h"
#include "ShaderData.h"
#include <memory>

enum class RendererType { UNDEFINED = -1, MODEL, DISPLACEMENT, PARTICLE, SHADOW, DEFERRED };

class Renderer
{
protected:
	RendererType type = RendererType::UNDEFINED;
	std::vector<std::shared_ptr<Drawable>> drawables;
public:
	virtual ~Renderer() = default;
	void Bind(std::shared_ptr<Drawable> drawable) { drawables.push_back(drawable); }
	RendererType Type() { return type; }
	virtual void Render() = 0;
};