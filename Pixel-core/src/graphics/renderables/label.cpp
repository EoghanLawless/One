#include "label.h"


namespace pixel {
	namespace graphics {

		Label::Label(std::string text, float x, float y, unsigned int colour) : Renderable(maths::vec3f(x, y, 0.0f), maths::vec2f(0.0f, 0.0f), colour), text(text), font(FontManager::get()) { }

		Label::Label(std::string text, float x, float y, Font* font, unsigned int colour) : Renderable(maths::vec3f(x, y, 0.0f), maths::vec2f(0.0f, 0.0f), colour), text(text), font(font) { }

		Label::Label(std::string text, float x, float y, const std::string& font, unsigned int colour) : Renderable(maths::vec3f(x, y, 0.0f), maths::vec2f(0.0f, 0.0f), colour), text(text), font(FontManager::get(font)) {
			validateFont(font, -1);
		}

		Label::Label(std::string text, float x, float y, const std::string& font, unsigned int size, unsigned int colour) : Renderable(maths::vec3f(x, y, 0.0f), maths::vec2f(0.0f, 0.0f), colour), text(text), font(FontManager::get(font, size)) {
			validateFont(font, size);
		}

		void Label::submit(Renderer* renderer) const {
			renderer->drawString(text, position, *font, colour);
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