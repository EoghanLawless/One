#include "dynamiclayer.h";

namespace pixel {
	namespace graphics {

		DynamicLayer::DynamicLayer(Renderer* renderer, Shader* shader, maths::mat4 projectionMatrix) : Layer(renderer, shader, projectionMatrix) {
			_shader->enable();
			_shader->setUniformMat4("pr_matrix", projectionMatrix);


			GLint textureIds[] = {
				 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
				16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
			};
			_shader->setUniform1iv("textures", textureIds, 32);

			_shader->disable();
		}

		DynamicLayer::~DynamicLayer() {
			delete _renderer;

			while (!_renderables.empty()) {
				delete _renderables.back();
				_renderables.pop_back();
			}
		}

		Renderable* DynamicLayer::get(unsigned int index) {
			return _renderables.at(index);
		}

		unsigned int DynamicLayer::remove(Renderable* renderable) {
			std::vector<Renderable*>::iterator element = std::find(_renderables.begin(), _renderables.end(), renderable);

			if(element != _renderables.end())
				return _renderables.erase(element) - _renderables.begin();

			return -1;
		}


		Renderable* DynamicLayer::operator [] (unsigned int index) {
			return _renderables.at(index);
		}

	}
}