#pragma once

#include <deque>

#include "graphics/renderers/renderer2d.h"
#include "graphics/renderables/staticsprite.h"

namespace pixel {
	namespace graphics {

		class SimpleRenderer2D : Renderer2D {
		private:
			std::deque<const StaticSprite*> _renderQueue;

		public:
			virtual void submit(const Renderable2D* renderable) override;
			virtual void flush() override;
		};

	}
}