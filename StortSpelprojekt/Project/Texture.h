#pragma once
#include "Graphics.h"
#include "DirectXHelp.h"

class Texture
{
private:
	std::string file;
	std::string path;
	ID3D11ShaderResourceView* srv = nullptr;
public:
	Texture() = default;
	~Texture() { srv->Release(); }
	Texture(const std::string& path, std::string file = "");

	ID3D11ShaderResourceView* Get()					                  { return srv; }
	std::string GetPath()										      { return this->path; }
	std::string GetFile()				     					      { return this->file; }

	void Bind(int slot = 0, Shader shader = Shader::PS) const
	{
		switch (shader)
		{
		case Shader::VS:
			Graphics::Inst().GetContext().VSSetShaderResources(slot, 1, &srv);
			break;

		case Shader::HS:
			Graphics::Inst().GetContext().HSSetShaderResources(slot, 1, &srv);
			break;

		case Shader::DS:
			Graphics::Inst().GetContext().DSSetShaderResources(slot, 1, &srv);
			break;

		case Shader::GS:
			Graphics::Inst().GetContext().GSSetShaderResources(slot, 1, &srv);
			break;

		case Shader::PS:
			Graphics::Inst().GetContext().PSSetShaderResources(slot, 1, &srv);
			break;
		}
	}
};