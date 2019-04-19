#include "dynamicsprite.h"

namespace pixel {
	namespace graphics {

		DynamicSprite::DynamicSprite(float x, float y, float w, float h, const unsigned int colour) : Renderable(maths::vec3f(x, y, 0), maths::vec2f(w, h), colour) { }
		DynamicSprite::DynamicSprite(maths::vec2f pos, maths::vec2f size, const unsigned int colour) : Renderable(maths::vec3f(pos.x, pos.y, 0), maths::vec2f(size.x, size.y), colour) { }
		DynamicSprite::DynamicSprite(maths::vec4f xywh, const unsigned int colour) : Renderable(maths::vec3f(xywh.x, xywh.y, 0), maths::vec2f(xywh.z, xywh.w), colour) { }
		
		DynamicSprite::DynamicSprite(float x, float y, float w, float h, Texture* texture) : Renderable(maths::vec3f(x, y, 0), maths::vec2f(w, h), 0xFFFFFFFF) { _texture = texture; }
		DynamicSprite::DynamicSprite(maths::vec2f pos, maths::vec2f size, Texture* texture) : Renderable(maths::vec3f(pos.x, pos.y, 0), maths::vec2f(size.x, size.y), 0xFFFFFFFF) { _texture = texture; }
		DynamicSprite::DynamicSprite(maths::vec4f xywh, Texture* texture) : Renderable(maths::vec3f(xywh.x, xywh.y, 0), maths::vec2f(xywh.z, xywh.w), 0xFFFFFFFF) { _texture = texture; }



	}
}