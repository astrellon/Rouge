#pragma once

#include <string>

using namespace std;

namespace am {
namespace base {

class Map;

class Game {
public:
	Game();
	~Game();

	void setMap(Map *map);
	Map *getMap();

	int loadMap(const string &filename);
	string getMapFilename() const;

protected:
	Map *mMap;

	string mMapFilename;
};

}
}
