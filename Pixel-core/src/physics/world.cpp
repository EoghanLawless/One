#include "world.h"

namespace pixel {
	namespace physics {
		
		World::World(maths::vec2f gravity = maths::vec2f(0.0f, 9.8f)) {
			_world = new b2World(convertVector(gravity));
		}
		World::~World() {
			_world->~b2World();
		}

		b2World& World::getB2World() {
			return *_world;
		}

		void World::step(const double interval, const unsigned int velocityChecks, const unsigned int positionChecks) {
			_world->Step(interval, velocityChecks, positionChecks);
		}
		void World::shiftOrigin(maths::vec2f offset) {
			_world->ShiftOrigin(convertVector(offset));
		}

		maths::vec2f World::getGravity() {
			return convertVector(_world->GetGravity());
		}
		void World::setGravity(maths::vec2f gravity) {
			_world->SetGravity(convertVector(gravity));
		}

		Body* World::createBody(BodyDef* def) {
			return new Body(_world->CreateBody(def->get()));
		}
		void World::destroyBody(Body* body) {
			_world->DestroyBody(body->get());
		}

		//TODO add joints
		//Joint* World::createJoint(const JointDef* def) {}
		//void World::destroyJoint(Joint* joint) {}
		//Joint* World::getJoints() {}


		b2Vec2 World::convertVector(maths::vec2f v) { return b2Vec2(v.x, v.y); }
		b2Vec3 World::convertVector(maths::vec3f v) { return b2Vec3(v.x, v.y, v.z); }
		maths::vec2f World::convertVector(b2Vec2 v) { return maths::vec2f(v.x, v.y); }
		maths::vec3f World::convertVector(b2Vec3 v) { return maths::vec3f(v.x, v.y, v.z); }

	}
}