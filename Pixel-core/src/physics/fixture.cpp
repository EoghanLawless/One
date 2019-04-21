#include "fixture.h"

namespace pixel {
	namespace physics {

		Fixture::Fixture() {
		}
		Fixture::~Fixture() { }

		b2Shape* shape;
		float friction;
		float restitution;
		float density;
		bool isSensor;

	}
}