#pragma once

#include "Box2D/Box2D.h"

#include "maths/maths.h"

namespace pixel {
	namespace physics {

		struct FixtureDef {
			FixtureDef() {
				shape = nullptr;
				friction = 0.2f;
				restitution = 0.0f;
				density = 0.0f;
				isSensor = false;
			}

			b2Shape* shape = nullptr;
			float32 friction = 0.2f;
			float32 restitution = 0.0f;
			float32 density = 0.0f;
			bool isSensor = false;

			b2FixtureDef* set() {
				_fixtureDef.shape = shape;
				_fixtureDef.friction = friction;
				_fixtureDef.restitution = restitution;
				_fixtureDef.density = density;
				_fixtureDef.isSensor = isSensor;

				return &_fixtureDef;
			}
			b2FixtureDef* get() {
				return set();
			}

		private:
			b2FixtureDef _fixtureDef;
		};

		class Fixture {
		private:
			b2Body* _body;

		public:
			Fixture();
			~Fixture();

		};

	}
}