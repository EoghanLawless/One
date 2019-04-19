#pragma once

#include "graphics/renderers/renderer.h"
#include "graphics/renderables/renderable.h"

#include "graphics/layers/layer.h"

namespace pixel {
	namespace graphics {

		class DynamicLayer : public Layer {
		protected:

		public:
			DynamicLayer(Renderer* renderer, Shader* shader, maths::mat4 projectionMatrix);
			~DynamicLayer() override;

			Renderable* get(unsigned int index);

			unsigned int remove(Renderable* renderable);

			Renderable* operator [] (unsigned int index);
		};

	}
}