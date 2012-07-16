#pragma once

namespace am {
namespace game {

	enum TileType {
		TILE_TYPE_LAND = 0x01,
		TILE_TYPE_AIR = 0x02,
		TILE_TYPE_WATER = 0x04,
		TILE_TYPE_LAVA = 0x04,
		TILE_TYPE_WALL = 0x08
	};

}
}
