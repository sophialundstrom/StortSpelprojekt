#pragma once
#include "Drawable.h"
#include "DirectXHelp.h"
#include "ShaderData.h"
#include <memory>
#include <map>

typedef enum { FORWARD, DEFERRED } RenderMethod;

class Renderer
{
protected:
	std::vector<std::shared_ptr<Drawable>> drawables;
	std::vector<std::shared_ptr<Drawable>> staticDrawables;
public:
	virtual ~Renderer() = default;

	void Bind(const std::map<std::string, std::shared_ptr<Drawable>>& drawables) { for (auto& [name, drawable] : drawables) this->drawables.emplace_back(drawable); }
	void Bind(std::shared_ptr<Drawable> drawable) 
	{ 
		if (!this)
			return;

		bool found = false;
		for (auto& d : drawables)
			if (d == drawable)
				found = true;

		if (!found)
			drawables.push_back(drawable); 
	}

	void BindStatic(std::shared_ptr<Drawable> drawable)
	{
		if (!this)
			return;

		bool found = false;
		for (auto& d : staticDrawables)
			if (d == drawable)
				found = true;

		if (!found)
			staticDrawables.push_back(drawable);
	}

	void Unbind(std::shared_ptr<Drawable> drawable)
	{
		if (!this)
			return;

		for (UINT i = 0; i < drawables.size(); ++i)
			if (drawables[i] == drawable)
			{
				auto it = drawables.begin() + i;
				drawables.erase(it);
				return;
			}
	}

	bool IsBound(std::shared_ptr<Drawable> drawable)
	{
		if (!this)
			return false;

		for (UINT i = 0; i < drawables.size(); ++i)
			if (drawables[i] == drawable)
				return true;
		return false;
	}

	void UnbindStatic(std::shared_ptr<Drawable> drawable)
	{
		if (!this)
			return;

		for (UINT i = 0; i < staticDrawables.size(); ++i)
			if (staticDrawables[i] == drawable)
			{
				auto it = staticDrawables.begin() + i;
				staticDrawables.erase(it);
				return;
			}
	}

	void Clear() { drawables.clear(); }
	void ClearStatic() { staticDrawables.clear(); }
	virtual void Render() = 0;
	virtual void OnResize(float width, float height) {}
};	