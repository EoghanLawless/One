#pragma once

#include <deque>

#include "renderer2d.h"

namespace pixel {
	namespace graphics {

		class SimpleRenderer2D : Renderer2D {
		private:
			std::deque<const Renderable2D*> _renderQueue;

		public:
			virtual void submit(const Renderable2D* renderable) override;
			virtual void flush() override;
		};
	}
}