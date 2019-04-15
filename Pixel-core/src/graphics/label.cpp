#include "label.h"


namespace pixel {
	namespace graphics {

		Label::Label(std::string text, float x, float y, Font* font, unsigned int colour) : Renderable2D(),	text(text), position(_position), font(font) {
			_position = maths::vec3f(x, y, 0.0f);
			_colour = colour;
		}

		void Label::submit(Renderer2D* renderer) const {
			renderer->drawString(text, position, *font, _colour);
		}

	}
}