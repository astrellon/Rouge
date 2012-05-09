#pragma once

#include <string>
#include "json_value.h"

using std::string;
using namespace am::util;

namespace am {
namespace base {

class Graphic;

class Tile {
public:
	Tile(const char *name);
	Tile(const char *name, const char *fullName);
	~Tile();

	string getName();
	const string &getName() const;

	string getFullName();
	const string &getFullName() const;
	void setFullName(const char *name);
	
	Graphic *getGraphic();
	void setGraphic(Graphic *graphic);

	void loadDef(JsonObject &value);
	
protected:
	
	string mName;
	string mFullName;
	Graphic *mGraphic;

	bool parseDef(const char *name, JsonValue &value);
};

}
}

#ifdef _ANDROID
#	include "tile.cpp"
#endif
