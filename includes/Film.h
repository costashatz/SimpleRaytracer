#pragma once

#include "typeDefs.h"
#include "FreeImage.h"
#include <cstring>
using namespace std;

//Film class
class Film
{
private:
	BYTE* pixels;
	int width;
	int height;
public:
	Film() : pixels(nullptr),width(0),height(0) {}
	Film(int w, int h):width(w),height(h)
	{
		pixels = new BYTE[w*h*3];
		memset(pixels,0,w*h*3*sizeof(BYTE));
	}

	void Commit(unsigned int x, unsigned int y, Color color)
	{
		unsigned int _y = height-y-1;
		pixels[(width*_y+x)*3+2] = clamp((int)(color.x*255.0),0,255);
		pixels[(width*_y+x)*3+1] = clamp((int)(color.y*255.0),0,255);
		pixels[(width*_y+x)*3+0] = clamp((int)(color.z*255.0),0,255);
	}
	void WriteImage(string file)
	{
		FIBITMAP *bit = FreeImage_ConvertFromRawBits(pixels, width, height, width * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);
		FreeImage_Save(FIF_PNG, bit, file.c_str());
	}
};

//end of Film class