-- Test Lua Map
do
	-- Map
	local map = am.map.new("testMap", 18, 8)
	map:full_name("The Region of Test")

	am.engine.clear_using_tile_set()
	am.engine.using_tile_set("nature:nature")

	map:tiles({
		"brick:0", "brick:2", "grass:1", "grass", "grass", "dirt:3","grass","grass","grass","grass","grass","grass","grass","grass","grass","grass","grass","grass",
		"grass", "grass", "grass:2", "grass", "dirt:2", "grass", "grass","water","grass","grass","grass","grass","grass","grass","grass","grass","grass","grass",
		"dirt:0", "brick:2",  "grass:3", "dirt:3", "dirt:0", "grass", "grass","water","grass","water","water","water","grass","grass","grass","grass","grass","grass",
		"brick:2", "brick:1", "grass:4", "dirt:2", "dirt:1", "swamp",   "grass","water","grass","water","water","water","grass","grass","grass","grass","grass","grass",
		"brick:1", "brick:0", "grass:5", "grass", "swamp", "water",   "grass","water","grass","water","water","water","grass","grass","grass","grass","grass","grass",
		"brick:2", "brick:2", "grass:6", "grass", "water", "water",   "grass","water","grass","water","water","water","grass","grass","grass","grass","grass","grass",
		"brick:2", "brick:2", "grass:7", "swamp", "water", "water",     "grass","water","grass","grass","grass","grass","grass","grass","grass","grass","grass","grass",
		"brick:2", "brick:2", "grass:8", "swamp", "water", "water",     "grass","grass","grass","grass","grass","grass","grass","grass","grass","grass","grass","grass"
	})
	
	return map
end