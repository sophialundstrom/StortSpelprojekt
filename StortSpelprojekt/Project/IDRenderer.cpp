#include "IDRenderer.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int IDRenderer::GetObjectID(int xPix, int yPix)
{
	D3D11_BOX box;
	box.left = xPix;
	box.top = yPix;
	box.front = 0;
	box.right = xPix + 1;
	box.bottom = yPix + 1;
	box.back = 1;

	long xCord = xPix;
	long yCord = yPix;
	/*Graphics::Inst().GetContext().CopyResource(idTextureData, idTexture);*/

	Graphics::Inst().GetContext().CopySubresourceRegion(idTextureData,0,0,0,0, idTexture, 0, &box);

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

	memcpy(&id, data, sizeof(UINT32));

	Graphics::Inst().GetContext().Unmap(idTextureData, 0);
	return id;
}