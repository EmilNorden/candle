#include "texture.h"
#include <FreeImage.h>
#include <stdexcept>

using namespace std;

Texture::Texture(FIBITMAP *bmp)
{
    init_from_freeimage_bitmap(bmp);
}

Texture::Texture(string& path)
{
    FREE_IMAGE_FORMAT type = FreeImage_GetFileType(path.c_str());
    if(type == FIF_UNKNOWN) {
        type = FreeImage_GetFIFFromFilename(path.c_str());

        if(type == FIF_UNKNOWN)
            throw runtime_error("Unable to determine texture format.");
    }

    FIBITMAP *bitmap = FreeImage_Load(type, path.c_str());
	
	if(!bitmap)
		throw runtime_error("Unable to load texture.");
	
    init_from_freeimage_bitmap(bitmap);
    FreeImage_Unload(bitmap);
}

void Texture::init_from_freeimage_bitmap(FIBITMAP *bmp)
{
    m_width = FreeImage_GetWidth(bmp);
	m_height = FreeImage_GetHeight(bmp);
    m_buffer = new Vector3d[m_width * m_height];
    
	for(int y = 0 ; y < m_height; ++y) {
		for(int x = 0; x < m_width; ++x) {
			RGBQUAD rgb;
			FreeImage_GetPixelColor(bmp, x, y, &rgb);

			Vector3d &pixel = m_buffer[(y * m_width) + x];
			pixel.x() = rgb.rgbRed / 255.0;
			pixel.y() = rgb.rgbGreen / 255.0;
			pixel.z() = rgb.rgbBlue / 255.0;
		}
	}
}

void Texture::sample(int x, int y, Vector3d &result) const
{
    const Vector3d &pixel = m_buffer[(y * m_width) + x];
	result.x() = pixel.x();
	result.y() = pixel.y();
	result.z() = pixel.z();
}