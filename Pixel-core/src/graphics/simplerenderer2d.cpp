#include "simplerenderer2d.h"

namespace pixel {
	namespace graphics {

		void SimpleRenderer2D::submit(const Renderable2D* renderable) {
			_renderQueue.push_back(renderable);
		}
		void SimpleRenderer2D::flush() {
			while (!_renderQueue.empty()) {
				const Renderable2D* renderable = _renderQueue.front();

				renderable->getVAO()->bind();
				renderable->getIBO()->bind();

				renderable->getShader().setUniformMat4("ml_matrix", maths::mat4::translation(renderable->getPosition()));
				glDrawElements(GL_TRIANGLES, renderable->getIBO()->getCount(), GL_UNSIGNED_SHORT, 0);

				renderable->getIBO()->unbind();
				renderable->getVAO()->unbind();

				_renderQueue.pop_front();
			}
		}
	}
}