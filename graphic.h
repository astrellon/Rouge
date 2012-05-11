#pragma once

namespace am {
namespace base {

class Texture;

class Graphic {
public:
	Graphic();
	~Graphic();

	void loadTexture(const char *filename);

protected:

	Texture *mTexture;

private:

};

}
}
