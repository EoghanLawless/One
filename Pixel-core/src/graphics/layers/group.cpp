#include "group.h"

namespace pixel {
	namespace graphics {

		Group::Group(const maths::mat4& matrix) : _transformationMatrix(matrix) {}
		Group::~Group() {
			for (int i = 0; i < _renderables.size(); i++)
				delete _renderables[i];
		}

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