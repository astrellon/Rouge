-- Nature tiles
set = {}
set.fullName = "Nature Tiles"
set.tiles = {}

local tile = {}
set.tiles.grass = tile
tile.assetName = "tiles/grass"
tile.fullName = "Grass"
tile.precedence = 10
tile.description = "Tis grass"
tile.tileTypes = {"Land", "Air"}

tile = {}
set.tiles.dirt = tile
tile.assetName = "tiles/dirt"
tile.fullName = "Dirt"
tile.description = "It's dirt"
tile.tileTypes = {"Land", "Air"}

tile = {}
set.tiles.brick = tile
tile.assetName = "tiles/brick"
tile.fullName = "Brick"
tile.description = "It's Brick"
tile.tileTypes = {"Wall"}

tile = {}
set.tiles.water = tile
tile.assetName = "tiles/water"
tile.fullName = "Water"
tile.description = "It's wet"
tile.tileTypes = {"Water", "Air"}

tile = {}
set.tiles.swamp = tile
tile.assetName = "tiles/swamp"
tile.fullName = "Swamp"
tile.description = "You can walk on it..."
tile.tileTypes = {"Land", "Water", "Air"}

tile = {}
set.tiles.test = tile
tile.assetName = "tiles/testTransition"
tile.fullName = "Test transitional tile."
tile.description = "TRANSITIONS!"
tile.tileType = {"Land"}
