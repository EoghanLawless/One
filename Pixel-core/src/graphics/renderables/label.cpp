#include "label.h"


namespace pixel {
	namespace graphics {

		Label::Label(std::string text, float x, float y, unsigned int colour) : Renderable(), text(text), position(_position), font(FontManager::get("Courier New", 16)) {
			_position = maths::vec3f(x, y, 0.0f);
			_colour = colour;
		}

		Label::Label(std::string text, float x, float y, Font* font, unsigned int colour) : Renderable(), text(text), position(_position), font(font) {
			_position = maths::vec3f(x, y, 0.0f);
			_colour = colour;
		}

		Label::Label(std::string text, float x, float y, const std::string& font, unsigned int colour) : Renderable(), text(text), position(_position), font(FontManager::get(font)) {
			_position = maths::vec3f(x, y, 0.0f);
			_colour = colour;

			validateFont(font, -1);
		}

		Label::Label(std::string text, float x, float y, const std::string& font, unsigned int size, unsigned int colour) : Renderable(), text(text), position(_position), font(FontManager::get(font, size)) {
			_position = maths::vec3f(x, y, 0.0f);
			_colour = colour;

			validateFont(font, size);
		}

		void Label::submit(Renderer* renderer) const {
			renderer->drawString(text, position, *font, _colour);
		}

		void Label::validateFont(std::string alias, unsigned int size) {
			if (font != nullptr) return;

			std::cerr << "Font '" << alias;
			if (size > 0) std::cerr << " " << size;
			std::cerr << "' not found." << std::endl;

			font = FontManager::get("Courier New", 16);
		}

	}
}