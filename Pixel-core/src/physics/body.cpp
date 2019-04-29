#include "body.h"

#include "world.h"

namespace pixel {
	namespace physics {

		Body::Body(b2Body* body) {
			_body = body;
		}
		Body::~Body() { }

		b2Fixture* Body::createFixture(FixtureDef* def) {
			return _body->CreateFixture(def->get());
		}
		b2Fixture* Body::createFixture(const b2Shape* shape, float density) {
			return _body->CreateFixture(shape, density);
		}

		maths::vec2f Body::getPosition() const {
			return World::convertVector(_body->GetPosition());
		}
		float Body::getAngle() const {
			return _body->GetAngle();
		}

		void Body::setUserData(void* data) {
			_body->SetUserData(data);
		}
		void* Body::getUserData() const {
			return _body->GetUserData();
		}

		b2Body* Body::get() {
			return _body;
		}

	}
}