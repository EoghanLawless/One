#pragma once

#include "graphics/renderables/renderable2d.h"

namespace pixel {
	namespace graphics {

		class Group : public Renderable2D {
		private:
			std::vector<Renderable2D*> _renderables;
			maths::mat4 _transformationMatrix;

		public:
			Group(const maths::mat4& matrix);
			~Group();

			void add(Renderable2D* renderable);
			void submit(Renderer2D* renderer) const override;
		};

	}
}