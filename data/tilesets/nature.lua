-- Nature tiles
do
	local set = am.tile_set.new("nature")
		:full_name("Nature Tiles")
	am.engine.add_tile_set(set)
	
	set:add_tile(am.tile.new("grass")
		:full_name("Grass")
		:asset("tiles/nature:grass")
		:precedence(10)
		:description("Tis grass")
		:add_tile_type("land", "air"))
	
	set:add_tile(am.tile.new("dirt")
		:full_name("Dirt")
		:asset("tiles/nature:dirt")
		:precedence(5)
		:description("It's dirt")
		:add_tile_type("land", "air"))
		
	set:add_tile(am.tile.new("brick")
		:full_name("Brick")
		:asset("tiles/nature:bricks")
		:precedence(5)
		:description("It's brick")
		:add_tile_type("wall"))
		
	set:add_tile(am.tile.new("water")
		:full_name("Water")
		:asset("tiles/nature:water")
		:precedence(5)
		:description("It's wet")
		:add_tile_type("water", "air"))
		
	set:add_tile(am.tile.new("swamp")
		:full_name("Swamp")
		:asset("tiles/nature:swamp")
		:precedence(2)
		:description("You can walk on it")
		:add_tile_type("water", "land", "air"))
end