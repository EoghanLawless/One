#include "fontmanager.h"

#include <iostream>

namespace pixel {
	namespace graphics {

		std::vector<Font*> FontManager::_fonts;

		void FontManager::add(Font* font) {
			_fonts.push_back(font);
		}

		Font* FontManager::get(const std::string& alias) {
			for (Font* font : _fonts)
				if (font->getAlias() == alias)
					return font;

			return nullptr;
		}

		Font* FontManager::get(const std::string& alias, unsigned int size) {
			for (Font* font : _fonts)
				if (font->getSize() == size && font->getAlias() == alias)
					return font;

			return nullptr;
		}

		void FontManager::clean() {
			for (int index = 0; index < _fonts.size(); index++)
				delete _fonts[index];
		}

	}
}