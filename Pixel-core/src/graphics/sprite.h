#pragma once

#include "renderable2d.h"

namespace pixel {
	namespace graphics {
		class Sprite : public Renderable2D {
		private:

		public:
			Sprite(float x, float y, float w, float h, const maths::vec4f& colour);
			Sprite(float x, float y, float w, float h, Texture* texture);
		};
	}
}