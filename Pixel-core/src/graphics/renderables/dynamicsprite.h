#pragma once

#include "pixel.h"

namespace pixel {
	namespace graphics {

		class DynamicSprite : public Renderable {
		private:

		public:
			DynamicSprite(float x, float y, float w, float h, const unsigned int colour);
			DynamicSprite(maths::vec2f pos, maths::vec2f size, const unsigned int colour);
			DynamicSprite(maths::vec4f xywh, const unsigned int colour);

			DynamicSprite(float x, float y, float w, float h, Texture* texture);
			DynamicSprite(maths::vec2f pos, maths::vec2f size, Texture* texture);
			DynamicSprite(maths::vec4f xywh, Texture* texture);
		};

	}
}