#include "layer.h";

namespace pixel {
	namespace graphics {
		Layer::Layer(Renderer2D* renderer, Shader* shader, maths::mat4 projectionMatrix)
		: _renderer(renderer), _shader(shader), _projectionMatrix(projectionMatrix) {
			_shader->enable();

			_shader->setUniformMat4("pr_matrix", projectionMatrix);
		}

		Layer::~Layer() {
			delete _renderer;
			delete _shader;

			for (int i = 0; i < _renderables.size(); i++) {
				delete _renderables[i];
			}
		}

		void Layer::add(Renderable2D* renderable) {
			_renderables.push_back(renderable);
		}

		void Layer::render() {
			_shader->enable();

			_renderer->begin();

			for (const Renderable2D* renderable : _renderables)
				_renderer->submit(renderable);

			_renderer->end();
			_renderer->flush();
		}
	}
}