#include "sprite.h"

//#include "pixel.h"

namespace pixel {
	namespace graphics {

		Sprite::Sprite(float x, float y, float w, float h, const unsigned int colour) : Renderable(maths::vec3f(x, y, 0), maths::vec2f(w, h), colour) { }
		Sprite::Sprite(float x, float y, float w, float h, Texture* texture) : Renderable(maths::vec3f(x, y, 0), maths::vec2f(w, h), 0xFFFFFFFF) {
			_texture = texture;
		}
		Sprite::Sprite(float x, float y, float w, float h, std::vector<Texture*>& textures) : Renderable(maths::vec3f(x, y, 0), maths::vec2f(w, h), 0xFFFFFFFF) {
			_texture = textures.at(0);
			_texture_list = textures;
		}

		void Sprite::animate(unsigned int ups) {
			static int texture_id = 0;
			static int updates = 0;

			if (updates >= 60 / ups) {

				if (texture_id < _texture_list.size()) {
					_texture = _texture_list.at(texture_id++);
				}
				if (texture_id == _texture_list.size()) texture_id = 0;

				updates = 0;
			} else updates++;
		}

	}
}