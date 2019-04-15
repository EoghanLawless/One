#include "font.h"

namespace pixel {
	namespace graphics {

		Font::Font(std::string alias, std::string file, unsigned int size) : _alias(alias), _file(file), _size(size) {

			_freeTypeAtlas = texture_atlas_new(512, 512, 2);
			_freeTypeFont = texture_font_new_from_file(_freeTypeAtlas, size, file.c_str());
		}

	}
}
