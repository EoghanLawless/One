#include "simplerenderer.h"

#include "graphics/renderables/renderable.h"

namespace pixel {
	namespace graphics {

		void SimpleRenderer::submit(const Renderable* renderable) {
			_renderQueue.push_back((StaticSprite*) renderable);
		}
		void SimpleRenderer::flush() {
			while (!_renderQueue.empty()) {
				const StaticSprite* sprite = _renderQueue.front();

				sprite->getVertexArray()->bind();
				sprite->getIndexBuffer()->bind();

				sprite->getShader().setUniformMat4("ml_matrix", maths::mat4::translation(sprite->getPosition()));
				glDrawElements(GL_TRIANGLES, sprite->getIndexBuffer()->getCount(), GL_UNSIGNED_SHORT, 0);

				sprite->getIndexBuffer()->unbind();
				sprite->getVertexArray()->unbind();

				_renderQueue.pop_front();
			}
		}

	}
}