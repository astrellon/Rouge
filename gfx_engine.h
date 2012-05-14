#pragma once

namespace am {
namespace ui {

class GfxEngine {
public:
	GfxEngine();
	~GfxEngine();

	void init();
	void perspective();
	void orthographic();

protected:

};

}
}
