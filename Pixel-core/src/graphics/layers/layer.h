#pragma once

#include "../renderer2d.h"
#include "../renderable2d.h"

namespace pixel {
	namespace graphics {

		class Layer {
		protected:
			Layer(Renderer2D* renderer, Shader* shader, maths::mat4 projectionMatrix);

			Renderer2D* _renderer;
			std::vector<Renderable2D*> _renderables;
			Shader* _shader;
			maths::mat4 _projectionMatrix;

		public:
			virtual ~Layer();

			virtual void add(Renderable2D* renderable);
			virtual void render();

			int count() const { return _renderables.size(); }
		};

	}
}