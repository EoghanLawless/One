#include "sprite.h"

namespace pixel {
	namespace graphics {

		Sprite::Sprite(float x, float y, float w, float h, const maths::vec4f& colour) : Renderable2D(maths::vec3f(x, y, 0), maths::vec2f(w, h), colour) {

		}
	}
}