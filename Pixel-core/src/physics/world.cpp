#pragma once

#include "world.h"

namespace pixel {
	namespace physics {

		World::World() { }
		World::World(maths::vec2f gravity) {
			_world = &b2World(b2Vec2(gravity.x, gravity.y));
		}
		World::~World() { }

		Body* World::createBody(BodyDef* def) {
			return new Body((*_world).CreateBody((*def).get()));
		}

	}
}