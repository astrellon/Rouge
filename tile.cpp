#ifndef _ANDROID
#	include "tile.h"
#endif

namespace am {
namespace base {

Tile::Tile(const char *name) :
	mName(name),
	mFullName("No full name"),
	mGraphic(NULL)
{
	printf("Creating tile '%s'\n", name);
}
Tile::Tile(const char *name, const char *fullName) :
	mName(name),
	mFullName(fullName),
	mGraphic(NULL)
{
	printf("Creating tile '%s', '%s'\n", name, fullName);
}
Tile::~Tile() {
	printf("Deleting tile '%s'\n", mFullName.c_str());
}

string Tile::getName() {
	return mName;
}
const string &Tile::getName() const {
	return mName;
}

string Tile::getFullName() {
	return mFullName;
}
const string &Tile::getFullName() const {
	return mFullName;
}
void Tile::setFullName(const char *name) {
	mFullName = name;
}

Graphic *Tile::getGraphic() {
	return mGraphic;
}
void Tile::setGraphic(Graphic *graphic) {
	mGraphic = graphic;
}

void Tile::loadDef(JsonObject &value) {
	JsonObject::iterator iter;
	for (iter = value.begin(); iter != value.end(); ++iter) {
		if (iter->first.compare("__comment") == 0) {
			continue;
		}

		if (!parseDef(iter->first.c_str(), iter->second)) {
			printf("Unable to parse tile property: '%s'\n", iter->first.c_str());
		}
	}
}

bool Tile::parseDef(const char *name, JsonValue &value) {
	if (strcmp(name, "fullName") == 0 && value.getType() == JV_STR) {
		setFullName(value.getCStr());
		return true;
	}
	return false;
}

}
}
