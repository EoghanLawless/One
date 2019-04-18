#pragma once

#include <iostream>

#include "GL/glew.h"
#include "FreeImage.h"

namespace pixel {

	static BYTE* load_image(const char* path, GLsizei* width, GLsizei* height) {
		FREE_IMAGE_FORMAT format = FIF_UNKNOWN;

		format = FreeImage_GetFileType(path, 0);

		if (format == FIF_UNKNOWN) {
			format = FreeImage_GetFIFFromFilename(path);
		}
		if (format == FIF_UNKNOWN) {
			std::cout << "Error loading texture, unsupported format '" << path << "'" << std::endl;
			return nullptr;
		}

		FIBITMAP* dib = nullptr;

		if (FreeImage_FIFSupportsReading(format))
			dib = FreeImage_Load(format, path);
		if (!dib) {
			std::cout << "Error loading texture, unable to load file '" << path << "'" << std::endl;
			return nullptr;
		}


		if (FreeImage_GetBPP(dib) != 32)
			dib = FreeImage_ConvertTo32Bits(dib);

		*width = FreeImage_GetWidth(dib);
		*height = FreeImage_GetHeight(dib);

		BYTE* result = new BYTE[FreeImage_GetWidth(dib) * FreeImage_GetHeight(dib) * 4];
		BYTE* pixels = (BYTE*)FreeImage_GetBits(dib);

		for (int pixel = 0; pixel < FreeImage_GetWidth(dib) * FreeImage_GetHeight(dib); pixel++) {
			result[pixel * 4 + 0] = pixels[pixel * 4 + 2];
			result[pixel * 4 + 1] = pixels[pixel * 4 + 1];
			result[pixel * 4 + 2] = pixels[pixel * 4 + 0];
			result[pixel * 4 + 3] = pixels[pixel * 4 + 3];
		}


#define DEBUG 0
#if DEBUG
		unsigned int bpp = FreeImage_GetBPP(dib);
		unsigned int pitch = FreeImage_GetPitch(dib);

		unsigned int w = FreeImage_GetWidth(dib);
		unsigned int h = FreeImage_GetHeight(dib);

		for (int y = h; y > 0; y--)
		{
			BYTE *pixel = (BYTE*)pixels;
			for (int x = 0; x < w; x++)
			{
				std::cout << bpp << ": " << +pixel[FI_RGBA_RED] << " " << +pixel[FI_RGBA_GREEN] << " " << +pixel[FI_RGBA_BLUE] << " " << +pixel[FI_RGBA_ALPHA] << std::endl;
				pixel += 4;
			}
			pixels += pitch;
		}
#endif


		//BYTE* result = new BYTE[size];
		//memcpy(result, pixels, size);

		FreeImage_Unload(dib);

		return result;
	}

}