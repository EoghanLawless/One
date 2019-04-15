#pragma once

#include <vector>

#include "font.h"

namespace pixel {
	namespace graphics {

		class FontManager {
		private:
			static std::vector<Font*> _fonts;

			FontManager();

		public:
			static void add(Font* font);
			static Font* get(const std::string& alias);
			static Font* get(const std::string& alias, unsigned int size);
			static void clean();
		};

	}
}