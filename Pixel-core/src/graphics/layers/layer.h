#pragma once

#include "graphics/renderers/renderer.h"
#include "graphics/renderables/renderable.h"

namespace pixel {
	namespace graphics {

		class Layer {
		protected:
			Layer(Renderer* renderer, Shader* shader, maths::mat4 projectionMatrix);

			Renderer* _renderer;
			std::vector<Renderable*> _renderables;
			Shader* _shader;
			maths::mat4 _projectionMatrix;

		public:
			virtual ~Layer();

			virtual void add(Renderable* renderable);
			virtual void render();

			inline int count() const { return _renderables.size(); }
			inline const std::vector<Renderable*>& getRenderables() const { return _renderables; }
		};

	}
}