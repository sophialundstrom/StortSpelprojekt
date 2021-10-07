#include "IDRenderer.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int IDRenderer::GetObjectID(int xPix, int yPix)
{

	Graphics::Inst().GetContext().CopyResource(idTextureData, idTexture);

	D3D11_TEXTURE2D_DESC textureDesc;
	idTextureData->GetDesc(&textureDesc);
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	HRESULT hr = Graphics::Inst().GetContext().Map(idTextureData, 0, D3D11_MAP_READ, 0, &mappedResource);
	if FAILED(hr)
	{
		Print("FAILED TO MAP SUBRESOURCE", "ID RENDERER::COPYING TEXTURE");
	}


	int id = 0;
	UINT32* data = static_cast<UINT32*>(mappedResource.pData);


	UINT32* imageData = new UINT32[textureDesc.Width * textureDesc.Height];
	for (int w = 0; w < textureDesc.Width; w++)
	{
		for (int h = 0; h < textureDesc.Height; h++)
		{
			int currentPixel = h * textureDesc.Width + w;
			imageData[currentPixel] = *data;
			data += 1;
		}
	}

	stbi_write_png("objectID", textureDesc.Width, textureDesc.Height, 1, imageData, textureDesc.Width);
	delete[] imageData;

	Print(textureDesc.Width);
	Print(xPix);
	Print(yPix);

	//data += yPix * textureDesc.Width + xPix;
	//memcpy(&id, data, sizeof(UINT32));

	Graphics::Inst().GetContext().Unmap(idTextureData, 0);
	return id;
}