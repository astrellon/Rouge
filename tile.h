#pragma once

#include <string>

using std::string;

namespace am {
namespace base {

class Graphic;

class Tile {
public:
	Tile(const char *name, Graphic *graphic);
	~Tile();
	string getName();
	Graphic *getGraphic();
	
protected:
	
	string mName;
	Graphic *mGraphic;
};

}
}

#ifdef _ANDROID
#	include "tile.cpp"
#endif
