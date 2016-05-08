#include "bitmap.h"

#include "assert.h"
#include "FreeImage.h"

#define DEFAULT_BPP 24

Bitmap::Bitmap(int width, int height)
{
    assert(width > 0);
    assert(height > 0);
    
    m_bitmap = FreeImage_Allocate(width, height, DEFAULT_BPP);
}

Bitmap::~Bitmap()
{
    FreeImage_Unload(m_bitmap);
}

void Bitmap::save(const std::string& path) const
{
    FreeImage_Save(FIF_PNG, m_bitmap, path.c_str());
}