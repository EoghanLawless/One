#pragma once

#include <string>

#include "freetype-gl.h"

namespace pixel {
	namespace graphics {

		class Font {
		private:
			unsigned int _size;

			texture_font_t* _freeTypeFont;
			texture_atlas_t* _freeTypeAtlas;

			std::string _alias;
			std::string _file;

		public:
			Font(std::string alias, std::string file, unsigned int size);

			inline const std::string& getAlias() const { return _alias; }
			inline const std::string& getFile() const { return _file; }
			inline const unsigned int getSize() const { return _size; }

			inline texture_font_t* getFont() const { return _freeTypeFont; }
			inline texture_atlas_t* getAtlas() const { return _freeTypeAtlas; }

			inline const unsigned int getId() const { return _freeTypeAtlas->id; }

		};

	}
}