#include "Image.h"

#include "FileSystem.h"
#include "UI.h"

Image::Image(const std::string& filename, D2D_VECTOR_2F position, float scale, float opacity, bool visible)
	:scale(1), opacity(1), sourceWidth(0), sourceHeight(0), bitMap(nullptr), UIComponent(0, 0, visible), filename(filename)
{
	HRESULT hr;
	IWICBitmapDecoder* decoder = nullptr;
	IWICBitmapFrameDecode* source = nullptr;
	IWICFormatConverter* converter = nullptr;
	IWICImagingFactory* factory = UI::Inst().GetImageFactory();

	const std::wstring path = std::wstring(std::wstring(FileSystem::ProjectDirectory::path.begin(), FileSystem::ProjectDirectory::path.end()) + L"\\Images\\" + std::wstring(filename.begin(), filename.end())).c_str();
	
	hr = factory->CreateDecoderFromFilename(path.c_str(), NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
	if FAILED(hr)
		return;

	//Retrieve frame from image and store it in frame decoder
	hr = decoder->GetFrame(0, &source);
	if FAILED(hr)
		return;

	//Convert bitmap to D2D format (32bppPBGRA)
	hr = factory->CreateFormatConverter(&converter);
	if FAILED(hr)
		return;

	hr = converter->Initialize(source, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);
	if FAILED(hr)
		return;

	//Create ID2D1Bitmap object
	hr = UI::Inst().GetRenderTarget()->CreateBitmapFromWicBitmap(converter, &bitMap);
	if FAILED(hr)
		return;

	this->scale = scale;
	this->sourceWidth = bitMap->GetSize().width;
	this->sourceHeight = bitMap->GetSize().height;
	this->width = scale * sourceWidth;
	this->height = scale * sourceHeight;
	this->bounds = {
		position.x - width / 2,
		position.y - height / 2,
		position.x + width / 2,
		position.y + height / 2
	};

	decoder->Release();
	source->Release();
	converter->Release();
}

Image::~Image()
{
	if (bitMap)
	{
		bitMap->Release();
		bitMap = nullptr;
	}
}

void Image::Draw()
{
	UI::Inst().GetRenderTarget()->DrawBitmap(bitMap, bounds, opacity, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, D2D1::RectF(0.0f, 0.0f, sourceWidth, sourceHeight));
}