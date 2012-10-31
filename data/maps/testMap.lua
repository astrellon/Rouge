-- Test map
map = {}
map.fullName = "The Region of Test"
map.usingTileSets = {"nature"}
map.tiles = {}
table.insert(map.tiles, {"brick:0", "brick:2", "grass:0", "grass:2", "grass:3", "dirt:3"})
table.insert(map.tiles, {"brick:1", "brick:0", "grass:1", "dirt:0", "dirt:2", "grass:1"})
table.insert(map.tiles, {"brick:0", "brick:2", "grass:2", "dirt:3", "dirt:0", "grass:3"})
table.insert(map.tiles, {"brick:2", "brick:1", "grass:3", "dirt:2", "dirt:1", "swamp"})
table.insert(map.tiles, {"brick:1", "brick:0", "grass:1", "grass:0", "swamp", "water"})
table.insert(map.tiles, {"brick:2", "brick:2", "grass:2", "grass:3", "water", "water"})
table.insert(map.tiles, {"brick:2", "brick:2", "grass:0", "swamp", "water", "water"})
table.insert(map.tiles, {"brick:2", "brick:2", "grass:3", "swamp", "water", "water"})