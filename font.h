#pragma once

namespace am {
namespace ui {

class Texture;

class Font {
public:
	Font();
	Font(const char *filename);
	~Font();

	Texture *getTexture();

	void renderChar(char ch);

protected:
	Texture *mTexture;

};

}
}
