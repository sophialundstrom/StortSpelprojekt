#pragma once
#include "Renderer.h"

class ModelRenderer : public Renderer
{
private:

	void RenderForward() { Print("FORWARD"); }
	void RenderDeferred() { Print("DEFERRED"); }
public:
	template <typename T>
	void Render()
	{
		if constexpr(std::is_same_v<Forward, T>)
			RenderForward();
		else
			RenderDeferred();
	}
};