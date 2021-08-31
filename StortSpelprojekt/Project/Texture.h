#pragma once
#include "Graphics.h"
#include "DirectXHelp.h"

class Texture
{
private:
	std::string file;
	ID3D11ShaderResourceView* srv = nullptr;
public:
	Texture() = default;
	~Texture() { srv->Release(); }
	Texture(std::string path, std::string file);

	void Bind(int slot = 0, Shader shader = Shader::PS)
	{
		switch (shader)
		{
		case Shader::VS:
			Graphics::GetContext().VSSetShaderResources(slot, 1, &srv);
			break;

		case Shader::HS:
			Graphics::GetContext().HSSetShaderResources(slot, 1, &srv);
			break;

		case Shader::DS:
			Graphics::GetContext().DSSetShaderResources(slot, 1, &srv);
			break;

		case Shader::GS:
			Graphics::GetContext().GSSetShaderResources(slot, 1, &srv);
			break;

		case Shader::PS:
			Graphics::GetContext().PSSetShaderResources(slot, 1, &srv);
			break;
		}
	}
};