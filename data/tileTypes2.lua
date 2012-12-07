-- Tile Types
TileType = import("TileType")

land = TileType.new("land", "Land")
TileType.add_tile_type(land)

water = TileType.new("water", "Water")
TileType.add_tile_type(water)

lava = TileType.new("lava", "Lava")
TileType.add_tile_type(lava)

air = TileType.new("air", "Air")
TileType.add_tile_type(air)

wall = TileType.new("wall", "Wall")
TileType.add_tile_type(wall)
