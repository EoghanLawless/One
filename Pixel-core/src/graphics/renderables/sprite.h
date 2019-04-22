#pragma once

#include "renderable.h"

namespace pixel {
	namespace graphics {

		class Sprite : public Renderable {
		private:
			std::vector<Texture*> _texture_list;

		public:
			Sprite(float x, float y, float w, float h, const unsigned int colour);
			Sprite(float x, float y, float w, float h, Texture* texture);
			Sprite(float x, float y, float w, float h, std::vector<Texture*>& texture);

			void animate(unsigned int ups);
		};

	}
}