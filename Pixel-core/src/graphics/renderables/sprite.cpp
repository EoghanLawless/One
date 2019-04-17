#include "sprite.h"

namespace pixel {
	namespace graphics {

		Sprite::Sprite(float x, float y, float w, float h, const unsigned int colour) : Renderable(maths::vec3f(x, y, 0), maths::vec2f(w, h), colour) { }
		Sprite::Sprite(float x, float y, float w, float h, Texture* texture) : Renderable(maths::vec3f(x, y, 0), maths::vec2f(w, h), 0xFFFFFFFF) {
			_texture = texture;
		}

	}
}