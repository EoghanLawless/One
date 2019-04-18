#include "layer.h";

namespace pixel {
	namespace graphics {

		Layer::Layer(Renderer* renderer, Shader* shader, maths::mat4 projectionMatrix) : _renderer(renderer), _shader(shader), _projectionMatrix(projectionMatrix) {
			_shader->enable();
			_shader->setUniformMat4("pr_matrix", projectionMatrix);


			GLint textureIds[] = {
				 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
				16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
			};
			_shader->setUniform1iv("textures", textureIds, 32);
			
			_shader->disable();
		}

		Layer::~Layer() {
			delete _renderer;

			while (!_renderables.empty()) {
				delete _renderables.back();
				_renderables.pop_back();
			}
		}

		void Layer::add(Renderable* renderable) {
			_renderables.push_back(renderable);
		}

		void Layer::render() {
			_shader->enable();

			_renderer->begin();

			for (const Renderable* renderable : _renderables)
				renderable->submit(_renderer);

			_renderer->end();
			_renderer->flush();
		}

	}
}