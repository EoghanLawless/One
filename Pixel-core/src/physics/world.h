#pragma once

#include "pixel.h"

#include "Box2D/Box2D.h"

#include "body.h"
#include "bodydef.h"

namespace pixel {
	namespace physics {

		class World {
		private:
			maths::vec3f _gravity;

			b2World* _world;

			World();

		public:
			World(maths::vec2f gravity);
			~World();

			Body* createBody(BodyDef* def);
		};

	}
}