//#pragma once
//
//#include "Box2D/Box2D.h"
//
//#include "maths/maths.h"
//
//namespace pixel {
//	namespace physics {
//
//		enum BodyType : unsigned char {
//			STATIC = b2_staticBody,
//			DYNAMIC,
//			KINEMATIC
//		};
//
//		struct BodyDef {
//		private:
//			b2BodyDef _bodyDef;
//
//		public:
//			BodyDef() = default;
//			BodyDef(float x, float y);
//			BodyDef(const maths::vec2f& pos);
//			~BodyDef();
//
//			BodyType type;
//			maths::vec2f position;
//			maths::vec2f linearVelocity;
//			float32 linearDamping;
//			float32 angle;
//			float32 angularVelocity;
//			float32 angularDamping;
//			bool fixedRotation;
//			bool bullet;
//			float32 gravityScale;
//
//			b2BodyDef* set();
//			const b2BodyDef* get();
//		};
//
//	}
//}