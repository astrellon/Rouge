#ifndef _ANDROID
#	include "tile.h"
#endif

namespace am {
namespace base {

Tile::Tile(const char *name, Graphic *graphic) {
	mName = name;
	mGraphic = graphic;
}
Tile::~Tile() {
}
string Tile::getName() {
	return mName;
}
Graphic *Tile::getGraphic() {
	return mGraphic;
}

}
}
