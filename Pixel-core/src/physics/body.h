#pragma once

#include "pixel.h"

#include "Box2D/Box2D.h"

namespace pixel {
	namespace physics {

		class Body {
		private:
			maths::vec3f _position;
			maths::vec2f _size;

			b2Body _body;

		public:
			Body(b2Body* body);
			~Body();
		};

	}
}