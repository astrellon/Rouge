#pragma once

#include <string>

using std::string;

class Graphic;

class Tile {
public:
	Tile(const char *name, Graphic *graphic) {
		mName = name;
		mGraphic = graphic;
	}
	~Tile() {
	}
	string getName() {
		return mName;
	}
	Graphic *getGraphic() {
		return mGraphic;
	}
	
protected:
	
	string mName;
	Graphic *mGraphic;
};
