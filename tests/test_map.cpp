#include "test_map.h"

#include <game/engine.h>
#include <game/map.h>
#include <game/tile.h>
#include <game/game_object.h>
#include <game/tile_type.h>
#include <game/tile_instance.h>
using namespace am::game;

#include <tests/asserts.h>

namespace am {
namespace tests {

	bool TestMap::testSimple()
	{
		Handle<TileType> landType(new TileType("land"));
		Handle<TileType> waterType(new TileType("water"));

		Handle<Tile> land(new Tile("land"));
		Handle<Tile> water(new Tile("water"));
		Handle<Tile> swamp(new Tile("swamp"));
		land->addTileType(landType);
		water->addTileType(waterType);
		swamp->addTileType(landType);
		swamp->addTileType(waterType);

		Handle<Map> testMap(new Map("testMap", 3, 1));
		TileInstance *tiles = testMap->getTiles();
		tiles[0].setTile(land);
		tiles[1].setTile(water);
		tiles[2].setTile(swamp);

		am_equals(3, testMap->getMapWidth());
		am_equals(1, testMap->getMapHeight());

		am_equalsDelta(3.0f * Engine::getEngine()->getGridSize(), testMap->getWidth(), 0.001f);
		am_equalsDelta(1.0f * Engine::getEngine()->getGridSize(), testMap->getHeight(), 0.001f);

		assert(testMap->getTile(0, 0) == land.get());
		assert(testMap->getTile(1, 0) == water.get());
		assert(testMap->getTile(2, 0) == swamp.get());

		const Tile::TileTypeList &landTileTypes = land->getTileTypes();
		am_equals(1u, landTileTypes.size());
		assert(landTileTypes[0].get() == landType.get());

		const Tile::TileTypeList &waterTileTypes = water->getTileTypes();
		am_equals(1u, waterTileTypes.size());
		assert(waterTileTypes[0].get() == waterType.get());

		const Tile::TileTypeList &swampTileTypes = swamp->getTileTypes();
		am_equals(2u, swampTileTypes.size());
		assert(swampTileTypes[0].get() == landType.get());
		assert(swampTileTypes[1].get() == waterType.get());

		return true;
	}

	bool TestMap::testValidTiles() 
	{
		Handle<TileType> landType(new TileType("land"));
		Handle<TileType> waterType(new TileType("water"));

		Handle<Tile> land(new Tile("land"));
		Handle<Tile> water(new Tile("water"));
		Handle<Tile> swamp(new Tile("swamp"));
		land->addTileType(landType);
		water->addTileType(waterType);
		swamp->addTileType(landType);
		swamp->addTileType(waterType);

		Handle<Map> testMap(new Map("testMap", 3, 1));
		TileInstance *tiles = testMap->getTiles();
		tiles[0].setTile(land);
		tiles[1].setTile(swamp);
		tiles[2].setTile(water);

		Handle<GameObject> landObj(new GameObject());
		landObj->addPassibleType(landType);
		am_equals(true, testMap->isValidGridLocation(0, 0, landObj));
		am_equals(true, testMap->isValidGridLocation(1, 0, landObj));
		am_equals(false, testMap->isValidGridLocation(2, 0, landObj));

		Handle<GameObject> waterObj(new GameObject());
		waterObj->addPassibleType(waterType);
		am_equals(false, testMap->isValidGridLocation(0, 0, waterObj));
		am_equals(true, testMap->isValidGridLocation(1, 0, waterObj));
		am_equals(true, testMap->isValidGridLocation(2, 0, waterObj));

		Handle<GameObject> swampObj(new GameObject());
		swampObj->addPassibleType(landType);
		swampObj->addPassibleType(waterType);
		am_equals(true, testMap->isValidGridLocation(0, 0, swampObj));
		am_equals(true, testMap->isValidGridLocation(1, 0, swampObj));
		am_equals(true, testMap->isValidGridLocation(2, 0, swampObj));

		float tileSizeX = Engine::getEngine()->getGridSize();
		float tileSizeY = Engine::getEngine()->getGridSize();

		landObj->setWidth(tileSizeX * 0.5f);
		landObj->setHeight(tileSizeY * 0.5f);
		
		am_equals(true, testMap->isValidLocation(0.0f, 0.0f, landObj));
		am_equals(true, testMap->isValidLocation(0.4f * tileSizeX, 0.4f * tileSizeY, landObj));
		am_equals(false, testMap->isValidLocation(0.6f * tileSizeX, 0.6f * tileSizeY, landObj));

		am_equals(true, testMap->isValidLocation(1.4f * tileSizeX, 0.4f * tileSizeY, landObj));
		am_equals(false, testMap->isValidLocation(1.6f * tileSizeX, 0.4f * tileSizeY, landObj));
		am_equals(false, testMap->isValidLocation(2.4f * tileSizeX, 0.4f * tileSizeY, landObj));

		return true;
	}

	bool TestMap::testValidTilesBig() 
	{
		Handle<TileType> landType(new TileType("land"));
		Handle<TileType> waterType(new TileType("water"));

		Handle<Tile> land(new Tile("land"));
		Handle<Tile> water(new Tile("water"));
		Handle<Tile> swamp(new Tile("swamp"));
		land->addTileType(landType);
		water->addTileType(waterType);
		swamp->addTileType(landType);
		swamp->addTileType(waterType);

		Handle<Map> testMap(new Map("testMap", 3, 2));
		TileInstance *tiles = testMap->getTiles();
		tiles[0].setTile(land);
		tiles[1].setTile(swamp);
		tiles[2].setTile(water);
		tiles[3].setTile(land);
		tiles[4].setTile(swamp);
		tiles[5].setTile(water);

		float tileSizeX = Engine::getEngine()->getGridSize();
		float tileSizeY = Engine::getEngine()->getGridSize();

		Handle<GameObject> landObj(new GameObject());
		landObj->addPassibleType(landType);
		landObj->setSize(2.0f * tileSizeX, 2.0f * tileSizeY);

		am_equals(false, testMap->isValidLocation(1.5f * tileSizeX, 0.0f, landObj));
		am_equals(true, testMap->isValidGridLocation(0.0f, 0.0f, landObj));

		return true;
	}

}
}
