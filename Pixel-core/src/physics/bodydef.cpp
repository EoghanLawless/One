#pragma once

#include "bodydef.h"

namespace pixel {
	namespace physics {

		void BodyDef::defaults() {
			type = STATIC;
			position.set(0.0f, 0.0f);
			linearVelocity.set(0.0f, 0.0f);
			linearDamping = 0.0f;
			angle = 0.0f;
			angularVelocity = 0.0f;
			angularDamping = 0.0f;
			fixedRotation = false;
			bullet = false;
			gravityScale = 1.0f;
		}

		BodyDef::BodyDef() {
			defaults();
		}
		BodyDef::BodyDef(float x, float y) {
			defaults();
			position.set(x, y);
		}
		BodyDef::BodyDef(const maths::vec2f& pos) {
			defaults();
			position.set(pos);
		}
		BodyDef::~BodyDef() {

		}

		b2BodyDef* BodyDef::set() {
			_bodyDef->type = (b2BodyType)type;
			_bodyDef->position.Set(position.x, position.y);
			_bodyDef->linearVelocity.Set(position.x, position.y);
			_bodyDef->linearDamping = linearDamping;
			_bodyDef->angle = angle;
			_bodyDef->angularVelocity = angularVelocity;
			_bodyDef->angularDamping = angularDamping;
			_bodyDef->fixedRotation = fixedRotation;
			_bodyDef->bullet = bullet;
			_bodyDef->gravityScale = gravityScale;

			return _bodyDef;
		}
		b2BodyDef* BodyDef::get() {
			return set();
		}

	}
}