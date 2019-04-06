#pragma once

#include <vector>
#include <GL/glew.h>

#include "../maths/maths.h"


namespace pixel {
	namespace graphics {

		class Renderable2D;

		class Renderer2D {
		protected:
			// Add const
			std::vector<maths::mat4> _transformationStack;
			const maths::mat4* _currentTransformation;

			Renderer2D() {
				_transformationStack.push_back(maths::mat4::identity());
				_currentTransformation = &_transformationStack.back();
			}

		public:
			void push(const maths::mat4& matrix, bool override = false) {
				if(override)
					_transformationStack.push_back(matrix);
				else
					_transformationStack.push_back(_transformationStack.back() * matrix);

				_currentTransformation = &_transformationStack.back();
			}
			void pop() {
				if(_transformationStack.size() > 1)
					_transformationStack.pop_back();

				_currentTransformation = &_transformationStack.back();
			}

			virtual void begin() {}
			virtual void submit(const Renderable2D* renderable) = 0;
			virtual void end() {}
			virtual void flush() = 0;
		};
	}
}