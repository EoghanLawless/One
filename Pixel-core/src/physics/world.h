#pragma once

#include "pixel.h"

#include "Box2D/Box2D.h"

#include "body.h"
#include "bodydef.h"

namespace pixel {
	namespace physics {

		class World {
		private:
			b2World* _world;

		public:
			World(maths::vec2f);
			~World();

			b2World& getB2World();

			void step(const double, const unsigned int, const unsigned int);
			void shiftOrigin(maths::vec2f);

			maths::vec2f getGravity();
			void setGravity(maths::vec2f);

			Body* createBody(BodyDef*);
			void destroyBody(Body*);

			//TODO add joints
			//Joint* createJoint(const JointDef*);
			//void destroyJoint(Joint*);
			//Joint* getJoints();

			static b2Vec2 convertVector(maths::vec2f);
			static b2Vec3 convertVector(maths::vec3f);
			static maths::vec2f convertVector(b2Vec2);
			static maths::vec3f convertVector(b2Vec3);
		};

	}
}