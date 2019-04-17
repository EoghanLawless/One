#pragma once

#include <deque>

#include "graphics/renderers/renderer.h"
#include "graphics/renderables/staticsprite.h"

namespace pixel {
	namespace graphics {

		class SimpleRenderer : Renderer {
		private:
			std::deque<const StaticSprite*> _renderQueue;

		public:
			virtual void submit(const Renderable* renderable) override;
			virtual void flush() override;
		};

	}
}