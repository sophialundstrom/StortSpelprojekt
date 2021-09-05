#pragma once
#include "Drawable.h"
#include "ShaderData.h"
#include "DirectXHelp.h"
#include <memory>

class Forward;
class Deferred;

enum class RendererType { UNDEFINED = -1, TERRAIN, MODEL, DISPLACEMENT, PARTICLE, SHADOW };

class Renderer
{
protected:
	RendererType type = RendererType::UNDEFINED;
	std::vector<std::shared_ptr<Drawable>> drawables;
public:
	virtual ~Renderer() = default;
	void Bind(std::shared_ptr<Drawable> drawable) { drawables.push_back(drawable); }
	RendererType Type() { return type; }
	void Clear() { drawables.clear(); }
};	