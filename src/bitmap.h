#ifndef BITMAP_H_
#define BITMAP_H_

#include <string>

struct FIBITMAP;

class Bitmap
{
public:
	Bitmap(int width, int height);
	~Bitmap();
	Bitmap(const Bitmap& other) = delete;
	Bitmap& operator=(const Bitmap& other) = delete;
	void save(const std::string& path) const;
	
private:
	FIBITMAP *m_bitmap;	
};

#endif
