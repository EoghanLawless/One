#pragma once

#include "graphics/renderables/renderable.h"

namespace pixel {
	namespace graphics {

		class Group : public Renderable {
		private:
			std::vector<Renderable*> _renderables;
			maths::mat4 _transformationMatrix;

		public:
			Group(const maths::mat4& matrix);
			~Group();

			void add(Renderable* renderable);
			void submit(Renderer* renderer) const override;
		};

	}
}