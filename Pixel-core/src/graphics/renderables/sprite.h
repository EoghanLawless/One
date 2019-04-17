#pragma once

#include "renderable.h"

namespace pixel {
	namespace graphics {

		class Sprite : public Renderable {
		private:

		public:
			Sprite(float x, float y, float w, float h, const unsigned int colour);
			Sprite(float x, float y, float w, float h, Texture* texture);
		};

	}
}