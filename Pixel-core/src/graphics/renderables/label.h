#pragma once

#include "renderable2d.h"
#include "graphics/fontmanager.h"

namespace pixel {
	namespace graphics {

		class Label : public Renderable2D {
		public:
			Font* font;
			std::string text;
			maths::vec3f& position;
			float x, y;

			Label(std::string text, float x, float y, unsigned int colour);
			Label(std::string text, float x, float y, Font* font, unsigned int colour);
			Label(std::string text, float x, float y, const std::string& font, unsigned int colour);
			Label(std::string text, float x, float y, const std::string& font, unsigned int size, unsigned int colour);

			void submit(Renderer2D* renderer) const override;

			void validateFont(std::string alias, unsigned int size = -1);
		};

	}
}