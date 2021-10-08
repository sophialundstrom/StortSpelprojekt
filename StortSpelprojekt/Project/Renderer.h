#pragma once
#include "Drawable.h"
#include "DirectXHelp.h"
#include <memory>
#include <map>

typedef enum { FORWARD, DEFERRED } RenderMethod;

class Renderer
{
protected:
	std::vector<std::shared_ptr<Drawable>> drawables;
public:
	virtual ~Renderer() = default;

	void Bind(const std::map<std::string, std::shared_ptr<Drawable>>& drawables) { for (auto& [name, drawable] : drawables) this->drawables.emplace_back(drawable); }
	void Bind(std::shared_ptr<Drawable> drawable) { drawables.push_back(drawable); }

	void Unbind(std::shared_ptr<Drawable> drawable)
	{
		for (UINT i = 0; i < drawables.size(); ++i)
			if (drawables[i] == drawable)
			{
				auto it = drawables.begin() + i;
				drawables.erase(it);
				return;
			}
	}

	void Clear() { drawables.clear(); }
	virtual void Render() = 0;
};	