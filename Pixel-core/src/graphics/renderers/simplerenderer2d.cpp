#include "simplerenderer2d.h"

#include "graphics/renderables/renderable2d.h"

namespace pixel {
	namespace graphics {

		void SimpleRenderer2D::submit(const Renderable2D* renderable) {
			_renderQueue.push_back((StaticSprite*) renderable);
		}
		void SimpleRenderer2D::flush() {
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