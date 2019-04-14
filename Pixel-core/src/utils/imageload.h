#pragma once

#include "GL/glew.h"
#include "FreeImage.h"

namespace pixel {

	static BYTE* load_image(const char* path, GLsizei* width, GLsizei* height) {
		FREE_IMAGE_FORMAT format = FIF_UNKNOWN;

		format = FreeImage_GetFileType(path, 0);

		if (format == FIF_UNKNOWN)
			format = FreeImage_GetFIFFromFilename(path);
		if (format == FIF_UNKNOWN)
			return nullptr;

		FIBITMAP* dib = nullptr;

		if (FreeImage_FIFSupportsReading(format))
			dib = FreeImage_Load(format, path);
		if (!dib)
			return nullptr;

		BYTE* result = FreeImage_GetBits(dib);

		*width = FreeImage_GetWidth(dib);
		*height = FreeImage_GetHeight(dib);

		return result;
	}

}