#pragma once

namespace am {
namespace gfx {

	class IGfxEngine;

	class IGfxComponent {
	public:
		virtual IGfxEngine *getGfxEngine() = 0;
		
	};

}
}
