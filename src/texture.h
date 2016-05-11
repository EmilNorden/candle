#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "vector.h"
#include <string>

struct FIBITMAP;

class Texture
{
public:
    Texture(FIBITMAP *bmp);
    Texture(std::string& path);
    int width() const { return m_width; }
    int height() const { return m_height; }
    void sample(int x, int y, Color &result) const;
private:
    void init_from_freeimage_bitmap(FIBITMAP *bmp);
    Color *m_buffer;
    int m_width;
    int m_height;
};

#endif