#pragma once

#include "pixel.h"

#include "Box2D/Box2D.h"

namespace pixel {
	namespace physics {

		class Body {
		private:
			b2Body* _body;

		public:
			Body(b2Body*);
			~Body();

			b2Fixture* createFixture(const b2FixtureDef*);
			b2Fixture* createFixture(const b2Shape*, float);

			maths::vec2f getPosition() const;
			float getAngle() const;

			b2Body* get();
		};

	}
}