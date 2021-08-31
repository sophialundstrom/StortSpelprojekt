#pragma once
#include "Drawable.h"
#include "ShaderData.h"
#include <memory>

enum class RendererType { MODEL, DISPLACEMENT, PARTICLE, SHADOW, DEFERRED };

class Renderer
{
protected:
	RendererType type;
	std::vector<std::shared_ptr<Drawable>> drawables;
public:
	virtual ~Renderer() = default;
	void Bind(std::shared_ptr<Drawable> drawable) { drawables.push_back(drawable); }
	RendererType Type() { return type; }
	virtual void Render() = 0;
};