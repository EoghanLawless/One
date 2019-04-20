#pragma once

#include "bodydef.h"

namespace pixel {
	namespace physics {
		
		BodyDef::BodyDef(float x, float y) {
			position.set(x, y);
		}
		BodyDef::BodyDef(const maths::vec2f& pos) {
			position.set(pos);
		}
		BodyDef::~BodyDef() {

		}

		b2BodyDef* BodyDef::set() {
			_bodyDef.position.Set(position.x, position.y);
			_bodyDef.type = type;
			_bodyDef.linearVelocity.Set(position.x, position.y);
			_bodyDef.linearDamping = linearDamping;
			_bodyDef.angle = angle;
			_bodyDef.angularVelocity = angularVelocity;
			_bodyDef.angularDamping = angularDamping;
			_bodyDef.fixedRotation = fixedRotation;
			_bodyDef.bullet = bullet;
			_bodyDef.gravityScale = gravityScale;

			return &_bodyDef;
		}
		const b2BodyDef* BodyDef::get() {
			return set();
		}

	}
}