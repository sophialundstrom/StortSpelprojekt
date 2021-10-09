#include "Image.h"
#include "UI.h"
#include "FileSystem.h"
//Image::Image()
//{
//	bounds.left = 0.0f;
//	bounds.top = 0.0f;
//	bounds.right = 50.0f;
//	bounds.bottom = 50.0f;
//	bitMap = 0;
//	scale = 1.0f;
//	opacity = 1.0f;
//	width = 0;
//	height = 0;
//	imageFactory = NULL;
//	decoder = NULL;
//	source = NULL;
//	converter = NULL;
//}

Image::~Image()
{
	bitMap->Release();
	imageFactory->Release();
	decoder->Release();
	source->Release();
	converter->Release();
}

Image::Image(const std::string& filename, D2D_VECTOR_2F position, float scale, float opacity)
{
	HRESULT hr;
	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&imageFactory);

	const std::wstring path = std::wstring(std::wstring(FileSystem::ProjectDirectory::path.begin(), FileSystem::ProjectDirectory::path.end()) + L"\\Images\\" + std::wstring(filename.begin(), filename.end())).c_str();
	hr = imageFactory->CreateDecoderFromFilename(path.c_str(), NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
	//Retrieve frame from image and store it in frame decoder
	if (SUCCEEDED(hr))
	{
		hr = decoder->GetFrame(0, &source);
	}
	//Convert bitmap to D2D format (32bppPBGRA)
	if (SUCCEEDED(hr))
	{
		hr = imageFactory->CreateFormatConverter(&converter);
	}
	if (SUCCEEDED(hr))
	{
		hr = converter->Initialize(source, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);
	}
	//Create ID2D1Bitmap object
	if (SUCCEEDED(hr))
	{
		hr = UI::Inst().GetRenderTarget()->CreateBitmapFromWicBitmap(converter, &bitMap);
	}

	this->scale = scale;
	this->width = scale * bitMap->GetSize().width;
	this->height = scale * bitMap->GetSize().height;
	this->bounds = {
		position.x - width / 2,
		position.y - height / 2,
		position.x + width / 2,
		position.y + height / 2
	};
}

void Image::Draw()
{
	UI::Inst().GetRenderTarget()->DrawBitmap(bitMap, bounds, opacity, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, D2D1::RectF(0.0f, 0.0f, bitMap->GetSize().width, bitMap->GetSize().height));
}
