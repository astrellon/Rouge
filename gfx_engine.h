#pragma once

namespace am {
namespace ui {

class GfxEngine {
public:
	GfxEngine();
	~GfxEngine();

	void init();
	void deinit();
	void setPerspective();
	void setOrthographic();
	
	void display();
	void reshape(int width, int height);

protected:

	int mScreenWidth;
	int mScreenHeight;
};

}
}
