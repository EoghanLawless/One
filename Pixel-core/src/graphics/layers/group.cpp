#include "group.h"

namespace pixel {
	namespace graphics {

		Group::Group(const maths::mat4& matrix) : _transformationMatrix(matrix) {}
		Group::~Group() { }

		void Group::add(Renderable* renderable) {
			_renderables.push_back(renderable);
		}

		void Group::submit(Renderer* renderer) const {
			renderer->push(_transformationMatrix);

			for (const Renderable* renderable : _renderables)
				renderable->submit(renderer);

			renderer->pop();
		}

	}
}