#pragma once

#include "Box2D/Box2D.h"

#include "fixture.h"

#include "maths/maths.h"

namespace pixel {
	namespace physics {

		enum BodyType : unsigned char {
			STATIC = b2_staticBody,
			KINEMATIC,
			DYNAMIC
		};

		struct BodyDef {
			BodyDef() {
				_bodyDef.type = b2_staticBody;
				_bodyDef.position.Set(0.0f, 0.0f);
				_bodyDef.linearVelocity.Set(0.0f, 0.0f);
				_bodyDef.linearDamping = 0.0f;
				_bodyDef.angle = 0.0f;
				_bodyDef.angularVelocity = 0.0f;
				_bodyDef.angularDamping = 0.0f;
				_bodyDef.gravityScale = 1.0f;
				_bodyDef.fixedRotation = false;
				_bodyDef.bullet = false;
			}

			BodyType type = STATIC;
			maths::vec2f position = maths::vec2f(0.0f, 0.0f);
			maths::vec2f linearVelocity = maths::vec2f(0.0f, 0.0f);;
			float32 linearDamping = 0.0f;
			float32 angle = 0.0f;
			float32 angularVelocity = 0.0f;
			float32 angularDamping = 0.0f;
			float32 gravityScale = 1.0f;
			bool fixedRotation = false;
			bool bullet = false;

			b2BodyDef* set() {
				_bodyDef.type = (b2BodyType)type;
				_bodyDef.position.Set(position.x, position.y);
				_bodyDef.linearVelocity.Set(linearVelocity.x, linearVelocity.y);
				_bodyDef.linearDamping = linearDamping;
				_bodyDef.angle = angle;
				_bodyDef.angularVelocity = angularVelocity;
				_bodyDef.angularDamping = angularDamping;
				_bodyDef.fixedRotation = fixedRotation;
				_bodyDef.bullet = bullet;
				_bodyDef.gravityScale = gravityScale;

				return &_bodyDef;
			}
			const b2BodyDef* get() {
				return set();
			}

		private:
			b2BodyDef _bodyDef;

		};

		class Body {
		private:
			b2Body* _body;

		public:
			Body(b2Body*);
			~Body();

			b2Fixture* createFixture(FixtureDef*);
			b2Fixture* createFixture(const b2Shape*, float);

			maths::vec2f getPosition() const;
			float getAngle() const;

			void setUserData(void*);
			void* getUserData() const;

			b2Body* get();
		};

	}
}