#pragma once

#include "renderable.h"
#include "graphics/fonts/fontmanager.h"

namespace pixel {
	namespace graphics {

		class Label : public Renderable {
		public:
			Font* font;
			std::string text;

			Label(std::string text, float x, float y, unsigned int colour);
			Label(std::string text, float x, float y, Font* font, unsigned int colour);
			Label(std::string text, float x, float y, const std::string& font, unsigned int colour);
			Label(std::string text, float x, float y, const std::string& font, unsigned int size, unsigned int colour);

			void submit(Renderer* renderer) const override;

			void validateFont(std::string alias, unsigned int size = -1);
		};

	}
}