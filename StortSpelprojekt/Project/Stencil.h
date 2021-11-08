#pragma once
#include "DirectXHelp.h"

class Stencil
{
private:
	ID3D11DepthStencilState* dStencilState;
public:
	enum class Mode
	{
		OFF, WRITE, MASK
	};
	Stencil(Mode mode)
	{
		D3D11_DEPTH_STENCIL_DESC dsDesc = CD3D11_DEPTH_STENCIL_DESC{ CD3D11_DEFAULT {} };
		if (mode == Mode::WRITE)
		{
			dsDesc.StencilEnable = true;
			dsDesc.StencilWriteMask = 0xFF;
			dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		}
		else if (mode == Mode::MASK)
		{
			dsDesc.DepthEnable = false;
			dsDesc.StencilEnable = true;
			dsDesc.StencilReadMask = 0xFF;
			dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
			dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		}
		Graphics::Inst().GetDevice().CreateDepthStencilState(&dsDesc, &dStencilState);
	}
	~Stencil()
	{
		dStencilState->Release();
	}
	void Bind()
	{
		Graphics::Inst().GetContext().OMSetDepthStencilState(dStencilState, 0xFF);
	}
};