-- Default tile set
do
	local set = am.tile_set.new("default")
		:full_name("Default Tiles")
	am.engine.add_tile_set(set)
	
	set:add_tile(am.tile.new("no_tile")
		:full_name("No tile")
		:asset("tiles/default:no_tile")
		:description("Someone should really replace this with a real tile.")
		:add_tile_type("land", "air"))	
end